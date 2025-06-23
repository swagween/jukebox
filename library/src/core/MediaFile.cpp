
#include <juke/core/MediaFile.hpp>
#include <juke/core/XMDecode.hpp>
#include <stdexcept>

namespace juke {

MediaFile::MediaFile(std::filesystem::path const& path) {
	auto const path_str = path.string();
	if (xm::is_xm(path)) {
		auto pcm_frames = xm::decode(path);
		if (pcm_frames.empty()) { throw std::runtime_error{"Failed to decode XM file: " + path_str}; }
		m_buffer.set_frames(std::move(pcm_frames), 2);
	} else if (!m_buffer.decode_file(path_str.c_str())) {
		throw std::runtime_error{"Failed to decode file: " + path_str};
	}
	m_filename = path.filename().string();
}

} // namespace juke
