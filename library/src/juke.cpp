
#include <capo/engine.hpp>
#include <juke/core/AudioFile.hpp>
#include <juke/core/MediaError.hpp>
#include <juke/core/XMFile.hpp>
#include <juke/juke.hpp>
#include <print>

namespace juke {

using namespace std::chrono_literals;

Jukebox::Jukebox(capo::IEngine& audio_engine) : m_source(audio_engine.create_source()) {
	if (!m_source) { throw std::runtime_error{"Failed to create Audio Source"}; }
}

bool Jukebox::load_media(std::filesystem::path const& path) {
	try {
		auto media_file = std::unique_ptr<IMediaFile>{};
		if (XMFile::is_xm(path)) {
			media_file = std::make_unique<XMFile>(path);
		} else {
			media_file = std::make_unique<AudioFile>(path);
		}
		m_source->stop();
		if (!media_file->bind_to(*m_source)) {
			std::println("ERROR: Failed to bind Source to PCM");
			return false;
		}
		m_media_file = std::move(media_file);
	} catch (MediaError const& e) { // only MediaErrors are caught here, others are raised all the way to main.
		std::println("ERROR: {}", e.what());
		return false;
	}

	return true;
}

void Jukebox::play(bool looping) {
	m_source->set_looping(looping);
	m_source->play();
}

void Jukebox::pause() { m_source->stop(); }

void Jukebox::stop() {
	m_source->stop();
	m_source->set_cursor({}); // seek to start. this will do nothing for XM streams
}

} // namespace juke
