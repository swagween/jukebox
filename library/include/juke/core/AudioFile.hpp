#pragma once

#include <capo/buffer.hpp>
#include <juke/core/MediaFile.hpp>

namespace juke {

class AudioFile : public IMediaFile {
  public:
	explicit AudioFile(std::filesystem::path const& path);

	bool bind_to(capo::ISource& source) final { return source.bind_to(m_buffer); }

  private:
	std::shared_ptr<capo::Buffer> m_buffer{std::make_shared<capo::Buffer>()};
};

} // namespace juke
