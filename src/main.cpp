#include "App.hpp"

int main(void) {
	std::unique_ptr<App> app;

	app = std::make_unique<App>();

	app->run();

	glfwTerminate();

	return 0;
}
