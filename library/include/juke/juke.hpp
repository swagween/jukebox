
#pragma once

#include <capo/engine.hpp>
#include <juke/core/MediaFile.hpp>
#include <juke/core/XMStream.hpp>
#include <filesystem>
#include <memory>

namespace juke {

enum class MediaStatus { playing, paused, stopped };

class Jukebox {
  public:
	explicit Jukebox(capo::IEngine& audio_engine);
	bool load_media(std::filesystem::path const& path);
	void play(bool looping = true);
	void pause();
	void stop();
	void update();

	[[nodiscard]] auto has_file() const -> bool { return static_cast<bool>(m_media_file); }
	[[nodiscard]] auto playing() const -> bool { return m_status == MediaStatus::playing; }
	[[nodiscard]] auto paused() const -> bool { return m_status == MediaStatus::paused; }
	[[nodiscard]] auto stopped() const -> bool { return m_status == MediaStatus::stopped; }
	[[nodiscard]] auto get_filename() const& -> std::string { return m_media_file->get_filename(); }

  private:
	MediaStatus m_status{MediaStatus::stopped};

	std::unique_ptr<IMediaFile> m_media_file{};
	std::unique_ptr<capo::ISource> m_source{};
};

} // namespace juke
