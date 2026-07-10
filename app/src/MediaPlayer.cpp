
#include <app/Colors.hpp>
#include <app/MediaPlayer.hpp>
#include <capo/engine.hpp>
#include <juke/core/AudioFile.hpp>
#include <juke/core/MediaError.hpp>
#include <juke/core/XMFile.hpp>
#include <print>

namespace juke {

using namespace std::chrono_literals;

MediaPlayer::MediaPlayer(capo::IEngine& audio_engine) : m_jukebox(audio_engine) {}

bool MediaPlayer::load_media(std::filesystem::path const& path) {
	m_source_folder = path.parent_path();
	return m_jukebox.load_media(path);
}

void MediaPlayer::handle_input() {
	ImGuiViewport* vp = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(vp->WorkPos);
	ImGui::SetNextWindowSize(vp->WorkSize);
	ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	if (ImGui::Begin("Player", nullptr, flags)) {

		/* ImGui variables */
		static auto looping{true};
		static auto spatialized{false};
		static auto gain{1.f};
		static auto pan{0.f};
		static auto pitch{1.f};
		static auto lo_cutoff{sample_rate_v};
		static auto hi_cutoff{0.f};
		static float pos[] = {0.f, 0.f, 0.f};

		/* Metadata & Info */
		ImGui::Text("Folder: %s", m_source_folder.string().c_str());
		ImGui::Text("File Name: ");
		ImGui::SameLine();
		m_jukebox.has_file() ? ImGui::TextColored(colors::blue, "%s", m_jukebox.get_filename().c_str()) : ImGui::TextColored(colors::grey, "<no file>");
		if (m_jukebox.has_file()) {
			ImGui::Text("Media Status: %s", m_jukebox.is_playing() ? "playing" : "paused");
			ImGui::ProgressBar(m_jukebox.get_percent_complete(), {300.f, 4.f}, "");
		}

		ImGui::Separator();

		/* Control Buttons */
		if (ImGui::Button("play")) { m_jukebox.play(looping); }
		ImGui::SameLine();
		if (ImGui::Button("pause")) { m_jukebox.pause(); }
		ImGui::SameLine();
		if (ImGui::Button("stop")) { m_jukebox.stop(); }
		ImGui::Separator();
		if (m_jukebox.has_file()) {
			if (ImGui::TreeNode("advanced", "advanced")) {

				if (ImGui::Button("reset")) {
					gain = 1.f;
					pitch = 1.f;
					pan = 0.f;
				}

				ImGui::Checkbox("loop", &looping);

				ImGui::SliderFloat("volume", &gain, 0.f, 1.f);
				ImGui::SliderFloat("pitch", &pitch, 0.f, 2.f);
				ImGui::SliderFloat("pan", &pan, -1.f, 1.f);
				m_jukebox.set_gain(gain);
				m_jukebox.set_pitch(pitch);
				m_jukebox.set_pan(pan);

				ImGui::Checkbox("spatialized", &spatialized);
				m_jukebox.set_spatialized(spatialized);
				if (m_jukebox.is_spatialized()) {
					ImGui::Separator();
					ImGui::SliderFloat3("position", pos, -1.f, 1.f, "%.1f");
					m_jukebox.set_position(capo::Vec3f{pos[0], pos[1], pos[2]});
					if (ImGui::Button("reset")) { pos[0] = pos[1] = pos[2] = 0.f; }
				}

				ImGui::SliderFloat("Low Pass", &lo_cutoff, 0.f, sample_rate_v, "%.1f", ImGuiSliderFlags_Logarithmic);
				m_jukebox.set_cutoff(FilterType::low, lo_cutoff, sample_rate_v);
				ImGui::SliderFloat("High Pass:", &hi_cutoff, 0.f, sample_rate_v, "%.1f", ImGuiSliderFlags_Logarithmic);
				m_jukebox.set_cutoff(FilterType::high, hi_cutoff, sample_rate_v);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("export", "export")) {
				if (ImGui::Button("write to .wav")) { m_jukebox.write_to_wav(); }
				if (ImGui::Button("generate soundtrack")) { generate_soundtrack(); }
				if (m_loading) {
					ImGui::TextColored(colors::grey, "generating soundtrack...");
					ImGui::ProgressBar(m_progress);
				}
				ImGui::TreePop();
			}
		}
	}
	ImVec2 required = ImGui::GetCursorPos();
	ImGuiStyle const& style = ImGui::GetStyle();

	required.x += style.WindowPadding.x;
	required.y += style.WindowPadding.y;
	auto size = ImVec2{std::ceil(required.x), std::ceil(required.y)};
	m_previous_content_size = m_content_size;
	m_content_size = size;
	ImGui::End();
}

void MediaPlayer::generate_soundtrack() {
	m_loading = true;
	m_loader_thread = std::jthread([this](std::stop_token stop) {
		auto total = std::ranges::distance(std::filesystem::recursive_directory_iterator(m_source_folder));
		int current = 0;
		for (auto const& file : std::filesystem::recursive_directory_iterator(m_source_folder)) {
			if (m_jukebox.load_media(file)) { m_jukebox.write_to_wav(); }
			m_progress = static_cast<float>(++current) / total;
		}
		m_loading = false;
	});
}

} // namespace juke
