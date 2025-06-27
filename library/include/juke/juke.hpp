
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

	/* capo API wrapper functions */

	// setters
	void set_gain(float gain) { m_source->set_gain(gain); }
	void set_pitch(float pitch) { m_source->set_pitch(pitch); }
	void set_pan(float pan) { m_source->set_pan(pan); }
	void set_fade_in(std::chrono::duration<float> duration, float gain) { m_source->set_fade_in(duration, gain); }
	void set_fade_out(std::chrono::duration<float> duration) { m_source->set_fade_out(duration); }
	void set_position(capo::Vec3f pos) { m_source->set_position(pos); }
	void set_spatialized(bool spatialized) { m_source->set_spatialized(spatialized); }

	//getters
	[[nodiscard]] auto get_gain() const -> float { return m_source->get_gain(); }
	[[nodiscard]] auto get_pitch() const -> float { return m_source->get_pitch(); }
	[[nodiscard]] auto get_pan() const -> float { return m_source->get_pan(); }
	[[nodiscard]] auto get_position() const -> capo::Vec3f { return m_source->get_position(); }
	[[nodiscard]] auto is_spatialized() const -> bool { return m_source->is_spatialized(); }

	/* -------------------------- */

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
