
#include <juke/core/MediaFile.hpp>
#include <stdexcept>

namespace juke {

MediaFile::MediaFile(std::filesystem::path const& path) : m_engine{capo::create_engine()} {
	if (!m_engine) { throw std::runtime_error{"Failed to create Engine"}; }

	m_source = m_engine->create_source();
	if (!m_source) { throw std::runtime_error{"Failed to create Source"}; }

	if (!m_buffer.decode_file(path.filename().string().c_str())) { throw std::runtime_error{"Failed to decode file: " + path.string()}; }
	if (!m_source->bind_to(&m_buffer)) { throw std::runtime_error{"Failed to bind Source to PCM"}; }
}

void MediaFile::play() { m_source->play(); }

} // namespace juke
