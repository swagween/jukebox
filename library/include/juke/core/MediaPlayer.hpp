#pragma once

#include <capo/engine.hpp>
#include <juke/core/MediaFile.hpp>
#include <juke/core/XMStream.hpp>
#include <chrono>
#include <filesystem>
#include <memory>
#include <string>

namespace juke {

enum class MediaStatus { playing, paused, stopped };

class MediaPlayer {
  public:
	explicit MediaPlayer(capo::IEngine& audio_engine);
	bool load_media(std::filesystem::path const& path);
	void handle_input();
	void update(std::chrono::duration<float> dt);

	[[nodiscard]] auto playing() const -> bool { return m_status == MediaStatus::playing; }
	[[nodiscard]] auto paused() const -> bool { return m_status == MediaStatus::paused; }
	[[nodiscard]] auto stopped() const -> bool { return m_status == MediaStatus::stopped; }

  private:
	MediaStatus m_status{MediaStatus::stopped};
	[[maybe_unused]] bool m_trigger{};

	std::unique_ptr<IMediaFile> m_media_file{};
	std::unique_ptr<capo::ISource> m_source{};

	std::string m_status_string{"stopped"};
};

} // namespace juke
