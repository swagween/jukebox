#pragma once

#include <capo/buffer.hpp>
#include <capo/stream_pipe.hpp>
#include <juke/core/Common.hpp>
#include <juke/effect/HighPassFilter.hpp>
#include <juke/effect/LowPassFilter.hpp>
#include <filesystem>
#include <memory>

namespace juke {

class XMStream : public capo::IStreamPipe {
  public:
	static bool is_xm(std::filesystem::path const& path);

	explicit XMStream(std::filesystem::path const& path);

	void set_cutoff(FilterType type, float to_cutoff, float sample_rate);
	void write_to_wav();

	[[nodiscard]] auto get_progress() const -> float { return m_progress; }

  private:
	struct Impl;
	struct Deleter {
		void operator()(Impl* ptr) const noexcept;
	};

	void push_samples(std::vector<float>& out) final;
	[[nodiscard]] auto get_sample_rate() const -> std::uint32_t final { return capo::Buffer::sample_rate_v; }
	[[nodiscard]] auto get_channels() const -> std::uint8_t final { return 2; }
	auto set_looping(bool looping) -> bool final;

	std::unique_ptr<Impl, Deleter> m_impl{};

	LowPassFilter m_lowpass;
	HighPassFilter m_highpass;

	float m_progress{};

	std::filesystem::path m_path;
};

} // namespace juke
