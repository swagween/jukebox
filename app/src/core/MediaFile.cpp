
#include <juke/core/MediaFile.hpp>
#include <exception>

namespace juke {

MediaFile::MediaFile(std::filesystem path const) : m_engine{capo::create_engine()} {
	if (!m_engine) { throw std::runtime_error{"Failed to create Engine"}; }

	m_source = m_engine->create_source();
	if (!m_source) { throw std::runtime_error{"Failed to create Source"}; }

	m_buffer.decode_file(path.filename().string(), nullptr);
	if (!m_source->bind_to(&m_buffer)) { throw std::runtime_error{"Failed to bind Source to PCM"}; }
}

void MediaFile::play() { m_source->play(); }

} // namespace juke
