
#include <app/Application.hpp>
#include <print>

int main() {
	try {
		juke::Application app{};
		app.run();
	} catch (std::exception const& e) {
		std::println("ERROR: {}", e.what());
		return EXIT_FAILURE;
	} catch (...) {
		std::println("ERROR: UNKNOWN");
		return EXIT_FAILURE;
	}
}
