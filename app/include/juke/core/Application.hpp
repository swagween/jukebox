
#pragma once

#include <SFML/Graphics.hpp>
#include <juke/core/MediaPlayer.hpp>

namespace juke {

class Application {
  public:
	Application();
	~Application();
	void run();

  private:
	sf::RenderWindow m_window;
	MediaPlayer m_player;
};

} // namespace juke
