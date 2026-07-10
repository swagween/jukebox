#include <juke/core/XMFile.hpp>

namespace juke {

XMFile::XMFile(std::filesystem::path const& path) : IMediaFile(path), m_stream(std::make_shared<XMStream>(path)) {}

void XMFile::write_to_wav() { m_stream->write_to_wav(); }

} // namespace juke
