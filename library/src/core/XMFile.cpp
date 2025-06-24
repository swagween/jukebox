#include <juke/core/XMFile.hpp>

namespace juke {

XMFile::XMFile(std::filesystem::path const& path) : IMediaFile(path), m_stream(std::make_shared<XMStream>(path)) {}

} // namespace juke
