#include "App.hpp"

int main(void) {
	App *app;

	app = new App();

	app->run();

	delete app;

	glfwTerminate();

	return 0;
}
