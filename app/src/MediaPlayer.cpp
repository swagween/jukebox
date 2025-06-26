
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

		static auto looping{true};
		/* Control Buttons */
		if (ImGui::Button("play")) { m_jukebox.play(looping); }
		ImGui::SameLine();
		if (ImGui::Button("pause")) { m_jukebox.pause(); }
		ImGui::SameLine();
		if (ImGui::Button("stop")) { m_jukebox.stop(); }
		ImGui::SameLine();
		ImGui::Checkbox("loop", &looping);
		ImGui::Separator();

		/* Metadata & Info */
		ImGui::Text("File Name: ");
		ImGui::SameLine();
		m_jukebox.has_file() ? ImGui::TextColored(colors::blue, "%s", m_jukebox.get_filename().c_str()) : ImGui::TextColored(colors::grey, "<no file>");
		ImGui::Text("Media Status: %s", m_jukebox.is_playing() ? "playing" : "paused");
	}
	ImGui::End();
}

} // namespace juke
