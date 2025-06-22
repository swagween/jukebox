
#include <imgui.h>
#include <capo/engine.hpp>
#include <juke/core/MediaPlayer.hpp>
#include <juke/graphics/Colors.hpp>
#include <algorithm>
#include <print>

namespace juke {

MediaPlayer::MediaPlayer(capo::IEngine& audio_engine) : m_source(audio_engine.create_source()), m_status{MediaStatus::stopped} {
	if (!m_source) { throw std::runtime_error{"Failed to create Audio Source"}; }
}

bool MediaPlayer::load_media(std::filesystem::path const& path) {
	try {
		auto file = MediaFile(path);
		m_source->stop(); // if file didn't throw, stop playback as m_file will be replaced
		m_file = std::move(file);
	} catch (std::exception const& e) {
		std::println("ERROR: {}", e.what());
		return false;
	} catch (...) {
		std::println("ERROR: UNKNOWN");
		return false;
	}
	if (!m_source->bind_to(&m_file->get_buffer())) {
		std::println("ERROR: Failed to bind Source to PCM");
		return false;
	}
	std::println("Media file loaded: {}", path.string());

	m_source->play();
	m_status = m_source->is_playing() ? MediaStatus::playing : MediaStatus::stopped;

	return true;
}

void MediaPlayer::handle_input() {
	ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	if (ImGui::Begin("Player", nullptr, flags)) {

		/* Progress Bar */
		auto fraction = 0.f;
		if (m_file) { fraction = m_source->get_cursor() / m_source->get_duration(); }
		ImGui::ProgressBar(fraction);

		/* Control Buttons */
		ImGui::Separator();
		if (ImGui::Button("play")) {
			m_source->play();
			m_status = MediaStatus::playing;
		}
		ImGui::SameLine();
		if (ImGui::Button("pause")) {
			m_source->stop();
			m_status = MediaStatus::paused;
		}
		ImGui::SameLine();
		if (ImGui::Button("stop")) {
			m_source->stop();
			m_source->set_cursor({}); // seek to start
			m_status = MediaStatus::stopped;
		}
		ImGui::Separator();
		ImGui::Text("File Name: ");
		ImGui::SameLine();
		m_file ? ImGui::TextColored(colors::blue, m_file->get_filename().c_str()) : ImGui::TextColored(colors::grey, "<no file>");

		m_status_string.clear();
		std::format_to(std::back_inserter(m_status_string), "{}", playing() ? "playing" : paused() ? "paused" : "stopped");
		ImGui::Text("Media Status: %s", m_status_string.c_str());
	}
	ImGui::End();
}

void MediaPlayer::update([[maybe_unused]] std::chrono::duration<float> const dt) {
	if (m_status == MediaStatus::playing && !m_source->is_playing()) { m_status = MediaStatus::stopped; }
}

} // namespace juke
