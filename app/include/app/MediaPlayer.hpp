#pragma once

#include <imgui.h>
#include <capo/engine.hpp>
#include <juke/juke.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <thread>

namespace juke {

class MediaPlayer {
  public:
	explicit MediaPlayer(capo::IEngine& audio_engine);
	bool load_media(std::filesystem::path const& path);
	void handle_input();

	[[nodiscard]] auto get_content_size() const -> ImVec2 { return m_content_size; }
	[[nodiscard]] auto has_size_changed() const -> bool { return m_content_size.x != m_previous_content_size.x || m_content_size.y != m_previous_content_size.y; }

  private:
	void generate_soundtrack();

  private:
	Jukebox m_jukebox;
	ImVec2 m_content_size{};
	ImVec2 m_previous_content_size{};
	std::filesystem::path m_source_folder{};

	// threaded soundtrack variables
	std::jthread m_loader_thread;
	std::atomic<float> m_progress = 0.0f;
	std::atomic<bool> m_loading = false;
};

} // namespace juke
