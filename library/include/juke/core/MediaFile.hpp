
#pragma once

#include <capo/source.hpp>
#include <filesystem>

namespace juke {

class IMediaFile : public capo::Polymorphic {
  public:
	explicit IMediaFile(std::filesystem::path const& path) : m_filename(path.filename().string()) {}

	virtual bool bind_to(capo::ISource& source) = 0;

	[[nodiscard]] std::string const& get_filename() const { return m_filename; }

  private:
	std::string m_filename{};
};

} // namespace juke
