
#include <juke/core/Application.hpp>
#include <imgui-SFML.h>

namespace juke {

Application::Application() : m_window{sf::VideoMode({200, 100}), "Jukebox", sf::Style::Default, sf::State::Windowed} {}

void Application::run() {

	sf::Clock clock{};
	clock.start();

	if (!ImGui::SFML::Init(m_window)) { return; }

	while (m_window.isOpen()) {
		while (std::optional const event = m_window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) { m_window.close(); }
			ImGui::SFML::ProcessEvent(m_window, *event);
		}

		ImGui::SFML::Update(m_window, clock.restart());

		m_player.handle_input();

		m_player.update(clock.getElapsedTime());

		m_window.clear();
		ImGui::SFML::Render();
		m_window.display();
	}
}

Application::~Application() { ImGui::SFML::Shutdown(); }

} // namespace juke
