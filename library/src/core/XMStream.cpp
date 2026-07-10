#include <capo/buffer.hpp>
#include <juke/core/MediaError.hpp>
#include <juke/core/XMStream.hpp>
#include <cassert>

#if defined(JUKE_USE_LIBXMP)
#include <dr_wav.h>
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

XMStream::XMStream(std::filesystem::path const& path) : m_impl(new Impl), m_lowpass{sample_rate_v, sample_rate_v}, m_highpass{0.f, sample_rate_v}, m_path{path} {
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

	// calculate module progress
	auto mi = xmp_module_info{};
	xmp_get_module_info(m_impl->context, &mi);
	float total_seconds = mi.seq_data->duration / 1000.0f;
	float current_seconds = frame_info.time / 1000.0f;
	m_progress = current_seconds / total_seconds;

	if (!m_impl->looping && frame_info.loop_count > 0) { return; }
	assert(frame_info.buffer_size % sizeof(std::int16_t) == 0);
	// wrap buffer into strongly-typed span for easy iteration and normalization.
	auto const src = std::span{static_cast<std::int16_t const*>(frame_info.buffer), static_cast<std::size_t>(frame_info.buffer_size) / sizeof(std::int16_t)};
	// need to normalize each u16 sample [-32k,32k] to f32 [-1,1].

	static constexpr auto sample_max_v = static_cast<float>(std::numeric_limits<std::int16_t>::max());
	for (std::int16_t const sample : src) {
		auto normalized = static_cast<float>(sample) / sample_max_v;
		auto high_passed = m_highpass.process(normalized);
		auto band_passed = m_lowpass.process(high_passed);
		out.push_back(band_passed);
	}
}

auto XMStream::set_looping(bool const looping) -> bool {
	m_impl->looping = looping;
	return true;
}

void XMStream::set_cutoff(FilterType type, float to_cutoff, float sample_rate) {
	switch (type) {
	case FilterType::high: m_highpass.set_cutoff(to_cutoff, sample_rate); break;
	case FilterType::low: m_lowpass.set_cutoff(to_cutoff, sample_rate); break;
	default: break;
	}
}

void XMStream::write_to_wav() {

	std::filesystem::create_directories("soundtrack");
	auto output_path = std::filesystem::path("soundtrack") / (m_path.stem().string() + ".wav");

	drwav_data_format format{};
	format.container = drwav_container_riff;
	format.format = DR_WAVE_FORMAT_PCM;
	format.channels = get_channels();
	format.sampleRate = get_sample_rate();
	format.bitsPerSample = 16;

	drwav wav{};
	if (!drwav_init_file_write(&wav, output_path.string().c_str(), &format, nullptr)) { return; }

	constexpr std::uint32_t fade_seconds = 10;
	std::uint64_t const fade_samples = static_cast<std::uint64_t>(format.sampleRate) * format.channels * fade_seconds;

	bool fading = false;
	std::uint64_t fade_pos = 0;

	xmp_frame_info info{};

	while (xmp_play_frame(m_impl->context) == 0) {
		xmp_get_frame_info(m_impl->context, &info);

		// Start fading after the first loop.
		if (!fading && info.loop_count > 0) { fading = true; }

		auto const* src = static_cast<std::int16_t const*>(info.buffer);
		auto const sample_count = info.buffer_size / sizeof(std::int16_t);

		if (!fading) {
			auto const frames = sample_count / format.channels;
			drwav_write_pcm_frames(&wav, frames, src);
			continue;
		}

		// Apply fade to a temporary buffer.
		std::vector<std::int16_t> faded(src, src + sample_count);

		std::size_t samples_to_write = sample_count;

		for (std::size_t i = 0; i < sample_count; ++i) {
			if (fade_pos >= fade_samples) {
				samples_to_write = i;
				break;
			}

			float t = static_cast<float>(fade_pos) / static_cast<float>(fade_samples);

			// Cosine fade (sounds smoother than linear).
			float gain = std::cos(t * std::numbers::pi_v<float> * 0.5f);

			faded[i] = static_cast<std::int16_t>(static_cast<float>(faded[i]) * gain);

			++fade_pos;
		}

		if (samples_to_write > 0) { drwav_write_pcm_frames(&wav, samples_to_write / format.channels, faded.data()); }

		if (fade_pos >= fade_samples) { break; }
	}

	drwav_uninit(&wav);
}

#else

void XMStream::Deleter::operator()(Impl* /*ptr*/) const noexcept {}

XMStream::XMStream(std::filesystem::path const& /*path*/) { throw std::runtime_error{"JUKE_USE_LIBXMP not enabled"}; }

bool XMStream::is_xm(std::filesystem::path const& /*path*/) { return false; }

void XMStream::push_samples(std::vector<float>& /*out*/) {}

auto XMStream::set_looping(bool const /*looping*/) -> bool { return false; }

#endif
} // namespace juke
