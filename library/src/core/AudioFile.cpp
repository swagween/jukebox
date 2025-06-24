#include <juke/core/AudioFile.hpp>
#include <juke/core/MediaError.hpp>

namespace juke {

AudioFile::AudioFile(std::filesystem::path const& path) : IMediaFile(path) {
	auto const path_str = path.string();
	if (!m_buffer->decode_file(path_str.c_str())) { throw MediaError{"Failed to decode file: " + path_str}; }
}

} // namespace juke
