
#include <imgui.h>
#include <capo/engine.hpp>
#include <juke/core/MediaPlayer.hpp>
#include <juke/graphics/Colors.hpp>
#include <print>

namespace juke {

MediaPlayer::MediaPlayer() : m_status{MediaStatus::stopped}, m_backdrop{} { m_backdrop.setFillColor(colors::grey); }

int MediaPlayer::load_media(std::filesystem::path const& path) {
	try {
		m_file = MediaFile(path);
	} catch (std::exception const& e) {
		std::println("ERROR: {}", e.what());
		return EXIT_FAILURE;
	} catch (...) {
		std::println("ERROR: UNKNOWN");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void MediaPlayer::handle_input() {
	static std::string status_string{"stopped"};
	ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	if (ImGui::Begin("Player", nullptr, flags)) {
		if (ImGui::Button("play")) { m_status = MediaStatus::playing; };
		ImGui::SameLine();
		if (ImGui::Button("pause")) { m_status = MediaStatus::paused; };
		ImGui::SameLine();
		if (ImGui::Button("stop")) { m_status = MediaStatus::stopped; };
		ImGui::Separator();
		status_string = playing() ? "playing" : paused() ? "paused" : "stopped";
		ImGui::Text("Media Status: %s", status_string.c_str());
		ImGui::End();
	}
}

void MediaPlayer::update([[maybe_unused]] std::chrono::duration<float> const dt) {}

} // namespace juke
