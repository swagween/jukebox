#pragma once
#include <filesystem>
#include <vector>

namespace juke::xm {

/// \returns true iff file at path is valid XM and JUKE_USE_LIBXMP is defined.
[[nodiscard]] bool is_xm(std::filesystem::path const& path);
/// \returns Stereo samples at 48kHz. Empty vector on failure or if JUKE_USE_LIBXMP is not defined.
std::vector<float> decode(std::filesystem::path const& xm_path);

} // namespace juke::xm
