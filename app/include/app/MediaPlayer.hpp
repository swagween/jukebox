#pragma once

#include <capo/engine.hpp>
#include <juke/juke.hpp>
#include <filesystem>
#include <memory>
#include <string>

namespace juke {

class MediaPlayer {
  public:
	explicit MediaPlayer(capo::IEngine& audio_engine);
	bool load_media(std::filesystem::path const& path);
	void handle_input();

  private:
	Jukebox m_jukebox;
};

} // namespace juke
