
#pragma once

#include <capo/source.hpp>
#include <juke/core/Common.hpp>
#include <filesystem>

namespace juke {

class IMediaFile : public capo::Polymorphic {
  public:
	explicit IMediaFile(std::filesystem::path const& path) : m_filename(path.filename().string()), m_extension(path.filename().extension().string()) {}

	virtual bool bind_to(capo::ISource& source) = 0;
	virtual void set_cutoff(FilterType type, float to_cutoff, float sample_rate) = 0;
	virtual void write_to_wav() = 0;

	[[nodiscard]] std::string const& get_filename() const { return m_filename; }
	[[nodiscard]] std::string const& get_extension() const { return m_extension; }
	[[nodiscard]] virtual auto get_progress() const -> float = 0;

  private:
	std::string m_filename{};
	std::string m_extension{};
};

} // namespace juke
