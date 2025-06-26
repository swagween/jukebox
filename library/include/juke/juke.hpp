
#pragma once

#include <capo/engine.hpp>
#include <juke/core/MediaFile.hpp>
#include <juke/core/XMStream.hpp>
#include <filesystem>
#include <memory>

namespace juke {

class Jukebox {
  public:
	explicit Jukebox(capo::IEngine& audio_engine);

	/// \brief Load audio file.
	/// \param path Path to file.
	/// \returns True if file loaded successfully, false otherwise.
	bool load_media(std::filesystem::path const& path);

	/// \brief Play audio.
	/// \param looping Whether or not the audio loops indefinitely.
	void play(bool looping = true);

	/// \brief Pause audio.
	void pause();

	/// \brief Stop audio and set cursor to beginning. This does not work for XM format.
	void stop();

	/// \brief Get the name of the loaded file as a string.
	[[nodiscard]] auto get_filename() const& -> std::string { return m_media_file->get_filename(); }

	[[nodiscard]] auto has_file() const -> bool { return static_cast<bool>(m_media_file); }
	[[nodiscard]] auto is_playing() const -> bool { return m_source->is_playing(); }
	[[nodiscard]] auto is_stopped() const -> bool { return !is_playing(); }

  private:
	std::unique_ptr<IMediaFile> m_media_file{};
	std::unique_ptr<capo::ISource> m_source{};
};

} // namespace juke
