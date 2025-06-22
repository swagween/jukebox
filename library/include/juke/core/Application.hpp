
#pragma once

#include <gvdi/context.hpp>
#include <juke/core/MediaPlayer.hpp>
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

	std::optional<gvdi::Context> m_context{};
	MediaPlayer m_player;
};

} // namespace juke
