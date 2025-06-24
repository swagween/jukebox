#pragma once

#include <stdexcept>

namespace juke {

struct MediaError : std::runtime_error {
	using std::runtime_error::runtime_error;
};

} // namespace juke
