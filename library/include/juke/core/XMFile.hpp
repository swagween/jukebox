#pragma once

#include <juke/core/MediaFile.hpp>
#include <juke/core/XMStream.hpp>

namespace juke {

class XMFile : public IMediaFile {
  public:
	static bool is_xm(std::filesystem::path const& path) { return XMStream::is_xm(path); }

	explicit XMFile(std::filesystem::path const& path);

	bool bind_to(capo::ISource& source) final { return source.bind_to(m_stream); }
	void write_to_wav() override;

	void set_cutoff(FilterType type, float to_cutoff, float sample_rate) override { m_stream->set_cutoff(type, to_cutoff, sample_rate); }

  private:
	std::shared_ptr<XMStream> m_stream{};
};

} // namespace juke
