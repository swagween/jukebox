
#pragma once

#include <capo/engine.hpp>
#include <filesystem>

namespace juke {

class MediaFile {
  public:
	explicit MediaFile(std::filesystem::path const& path);
	void play();

  private:
	std::unique_ptr<capo::IEngine> m_engine;
	std::unique_ptr<capo::ISource> m_source;
	capo::Buffer m_buffer{};
};

} // namespace juke
