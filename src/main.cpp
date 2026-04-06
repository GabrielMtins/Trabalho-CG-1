#include "App.hpp"

int main(void) {
	App *app;

	// Inicializa a aplicação principal
	app = new App();

	// Inicia o loop principal do programa
	app->run();

	// Limpa os recursos alocados
	delete app;

	// Encerra a biblioteca GLFW antes de fechar o programa
	glfwTerminate();

	return 0;
}
