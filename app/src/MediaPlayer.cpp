
#include <imgui.h>
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

bool MediaPlayer::load_media(std::filesystem::path const& path) { return m_jukebox.load_media(path); }

void MediaPlayer::handle_input() {
	ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	if (ImGui::Begin("Player", nullptr, flags)) {

		/* ImGui variables */
		static auto looping{true};
		static auto spatialized{false};
		static auto gain{1.f};
		static auto pan{0.f};
		static auto pitch{1.f};
		static float pos[] = {0.f, 0.f, 0.f};

		/* Metadata & Info */
		ImGui::Text("File Name: ");
		ImGui::SameLine();
		m_jukebox.has_file() ? ImGui::TextColored(colors::blue, "%s", m_jukebox.get_filename().c_str()) : ImGui::TextColored(colors::grey, "<no file>");
		ImGui::Text("Media Status: %s", m_jukebox.is_playing() ? "playing" : "paused");

		ImGui::Separator();

		/* Control Buttons */
		if (ImGui::Button("play")) { m_jukebox.play(looping); }
		ImGui::SameLine();
		if (ImGui::Button("pause")) { m_jukebox.pause(); }
		ImGui::SameLine();
		if (ImGui::Button("stop")) { m_jukebox.stop(); }
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
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

} // namespace juke
