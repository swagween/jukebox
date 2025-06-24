#include <capo/buffer.hpp>
#include <juke/core/MediaError.hpp>
#include <juke/core/XMStream.hpp>
#include <cassert>

#if defined(JUKE_USE_LIBXMP)
#include <xmp.h>
#endif

namespace juke {

#if defined(JUKE_USE_LIBXMP)
struct XMStream::Impl {
	[[nodiscard]] bool load_module(std::filesystem::path const& path) const { return xmp_load_module(context, path.string().c_str()) == 0; }

	xmp_context context{xmp_create_context()};
	bool looping{}; // if true, ignore xmp_frame_info::loop_count
};

void XMStream::Deleter::operator()(Impl* ptr) const noexcept {
	xmp_end_player(ptr->context);
	xmp_release_module(ptr->context);
	xmp_free_context(ptr->context);
	std::default_delete<Impl>{}(ptr);
}

XMStream::XMStream(std::filesystem::path const& path) : m_impl(new Impl) {
	if (!m_impl->load_module(path)) { throw MediaError{"Failed to open XM file: " + path.generic_string()}; }
	xmp_start_player(m_impl->context, static_cast<int>(capo::Buffer::sample_rate_v), 0);
}

bool XMStream::is_xm(std::filesystem::path const& path) {
	auto info = xmp_test_info{};
	return xmp_test_module(path.string().c_str(), &info) == 0;
}

void XMStream::push_samples(std::vector<float>& out) {
	auto frame_info = xmp_frame_info{};
	if (xmp_play_frame(m_impl->context) != 0) { return; }
	xmp_get_frame_info(m_impl->context, &frame_info);
	if (!m_impl->looping && frame_info.loop_count > 0) { return; }
	assert(frame_info.buffer_size % sizeof(std::int16_t) == 0);
	// wrap buffer into strongly-typed span for easy iteration and normalization.
	auto const src = std::span{static_cast<std::int16_t const*>(frame_info.buffer), static_cast<std::size_t>(frame_info.buffer_size) / sizeof(std::int16_t)};
	// need to normalize each u16 sample [-32k,32k] to f32 [-1,1].
	static constexpr auto sample_max_v = static_cast<float>(std::numeric_limits<std::int16_t>::max());
	for (std::int16_t const sample : src) { out.push_back(static_cast<float>(sample) / sample_max_v); }
}

auto XMStream::set_looping(bool const looping) -> bool {
	m_impl->looping = looping;
	return true;
}

#else

void XMStream::Deleter::operator()(Impl* /*ptr*/) const noexcept {}

XMStream::XMStream(std::filesystem::path const& /*path*/) { throw std::runtime_error{"JUKE_USE_LIBXMP not enabled"}; }

bool XMStream::is_xm(std::filesystem::path const& /*path*/) { return false; }

void XMStream::push_samples(std::vector<float>& /*out*/) {}

auto XMStream::set_looping(bool const /*looping*/) -> bool { return false; }

#endif
} // namespace juke
