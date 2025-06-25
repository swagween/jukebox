#pragma once

#include <capo/buffer.hpp>
#include <capo/stream_pipe.hpp>
#include <filesystem>
#include <memory>

namespace juke {

class XMStream : public capo::IStreamPipe {
  public:
	static bool is_xm(std::filesystem::path const& path);

	explicit XMStream(std::filesystem::path const& path);

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
};

} // namespace juke
