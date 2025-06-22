
#include <juke/core/MediaFile.hpp>
#include <stdexcept>

namespace juke {

MediaFile::MediaFile(std::filesystem::path const& path) {
	auto const path_str = path.string();
	if (!m_buffer.decode_file(path_str.c_str())) { throw std::runtime_error{"Failed to decode file: " + path_str}; }
	m_filename = path.filename().string();
}

} // namespace juke
