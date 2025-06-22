
#pragma once

#include <capo/engine.hpp>
#include <filesystem>

namespace juke {

class MediaFile {
  public:
	explicit MediaFile(std::filesystem::path const& path);
	[[nodiscard]] capo::Buffer const& get_buffer() const { return m_buffer; }
	[[nodiscard]] std::string const& get_filename() const { return m_filename; }

  private:
	capo::Buffer m_buffer{};
	std::string m_filename{};
};

} // namespace juke
