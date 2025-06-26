
#include <app/Application.hpp>
#include <stdexcept>

namespace juke {

Application::Application() {
	if (!m_engine) { throw std::runtime_error{"Failed to create Engine"}; }

	auto window = gvdi::Context::create_window({400.0f, 200.0f}, "Jukebox");
	if (!window) { throw std::runtime_error{"Failed to create window"}; }

	glfwSetWindowUserPointer(window.get(), this);
	set_glfw_callbacks(window.get());

	m_context.emplace(std::move(window));

	m_player.emplace(*m_engine);
}

void Application::run() {
	while (m_context->next_frame()) {
		m_player->handle_input();
		m_context->render();
	}
}

Application& Application::self(GLFWwindow* window) { return *static_cast<Application*>(glfwGetWindowUserPointer(window)); }

void Application::set_glfw_callbacks(GLFWwindow* window) {
	glfwSetDropCallback(window, [](GLFWwindow* window, int count, char const** paths) { self(window).on_file_drop({paths, static_cast<std::size_t>(count)}); });
}

void Application::on_file_drop([[maybe_unused]] std::span<char const* const> paths) {
	for (auto const& path : paths) {
		if (m_player->load_media(path)) { break; }
	}
}
} // namespace juke
