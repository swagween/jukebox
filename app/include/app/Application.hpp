
#pragma once

#include <app/MediaPlayer.hpp>
#include <capo/engine.hpp>
#include <gvdi/context.hpp>
#include <optional>
#include <span>

namespace juke {

class Application {
  public:
	explicit Application();
	void run();

  private:
	static Application& self(GLFWwindow* window);
	static void set_glfw_callbacks(GLFWwindow* window);

	void on_file_drop(std::span<char const* const> paths);

	std::unique_ptr<capo::IEngine> m_engine{capo::create_engine()};
	std::optional<gvdi::Context> m_context{};

	std::optional<MediaPlayer> m_player{};
};

} // namespace juke
