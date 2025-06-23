#include <capo/buffer.hpp>
#include <juke/core/XMDecode.hpp>
#include <cassert>
#include <limits>
#include <span>

#if defined(JUKE_USE_LIBXMP)
#include <xmp.h>
#endif

// Such an approach keeps all the ugly ifdef logic in one file.

namespace juke {

bool xm::is_xm([[maybe_unused]] std::filesystem::path const& path) {
#if defined(JUKE_USE_LIBXMP)
	auto info = xmp_test_info{};
	return xmp_test_module(path.string().c_str(), &info) == 0;
#else
	return false;
#endif
}

#if defined(JUKE_USE_LIBXMP)
namespace {

struct Deleter {
	void operator()(xmp_context context) const noexcept {
		xmp_end_player(context);
		xmp_release_module(context);
		xmp_free_context(context);
	}
};

using UniqueContext = std::unique_ptr<char, Deleter>; // xmp_context is char*

[[nodiscard]] UniqueContext create_context() { return UniqueContext{xmp_create_context()}; }

} // namespace
#endif

std::vector<float> xm::decode(std::filesystem::path const& xm_path) {
	auto context = create_context();
	// unique_ptr is used not for new/delete but to ensure custom deinitialization
	// of the XM module: to end its player and release it at scope exit.
	// the Deleter does not modify the passed context, it's safe to reuse.
	if (xmp_load_module(context.get(), xm_path.string().c_str()) != 0) { return {}; }

	// happy path begins (no more failures possible).
	// there may still be bugs in the code which might trigger an exception.
	// the RAII context takes care of freeing XMP allocations even if that happens.
	xmp_start_player(context.get(), static_cast<int>(capo::Buffer::sample_rate_v), 0);

	auto ret = std::vector<float>{};
	// since the total size is unknown, we cannot pre-reserve any meaningful capacity.
	// relying on vector's exponential growth, and malloc's eventual full-page allocations, is acceptable here.

	auto frame_info = xmp_frame_info{}; // one XM frame contains numerous PCM frames.
	while (xmp_play_frame(context.get()) == 0) {
		xmp_get_frame_info(context.get(), &frame_info);
		if (frame_info.loop_count > 0) { break; } // per docs.
		// XM framebuffer contains 16-bit signed samples.
		assert(frame_info.buffer_size % sizeof(std::int16_t) == 0);
		// wrap buffer into strongly-typed span for easy iteration and normalization.
		auto const src = std::span{static_cast<std::int16_t const*>(frame_info.buffer), static_cast<std::size_t>(frame_info.buffer_size) / sizeof(std::int16_t)};
		// need to normalize each u16 sample [-32k,32k] to f32 [-1,1].
		static constexpr auto sample_max_v = static_cast<float>(std::numeric_limits<std::int16_t>::max());
		for (std::int16_t const sample : src) { ret.push_back(static_cast<float>(sample) / sample_max_v); }
	}

	return ret;
}

} // namespace juke
