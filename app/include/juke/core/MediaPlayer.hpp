#pragma once

#include <SFML/Graphics.hpp>
#include <juke/core/MediaFile.hpp>
#include <optional>

namespace juke {

enum class MediaStatus { playing, paused, stopped };

class MediaPlayer {
  public:
	MediaPlayer();
	int load_media(std::filesystem const path);
	void handle_input();
	void update(std::chrono::duration<float> const dt);

	[[nodiscard]] auto playing() const -> bool { return m_status == MediaStatus::playing; }
	[[nodiscard]] auto paused() const -> bool { return m_status == MediaStatus::paused; }
	[[nodiscard]] auto stopped() const -> bool { return m_status == MediaStatus::stopped; }

  private:
	MediaStatus m_status;
	sf::RectangleShape m_backdrop;
	bool m_trigger{};

	std::optional<MediaFile> m_file{};
};

} // namespace juke
