#include "App.hpp"
#include "Builder.hpp"

#include <cstdio>
#include <cstdlib>

#include <array>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Código fonte do Vertex Shader (calcula a posição dos vértices na tela)
static const char *vertex_shader_src = "#version 330 core\n"
    "layout (location = 0) in vec3 a_position;\n"
    "uniform mat4 u_model;"
    "uniform mat4 u_view;"
    "uniform mat4 u_projection;"
    "void main()\n"
    "{\n"
    "   mat4 mvp = u_projection * u_view * u_model;\n"
    "   gl_Position = mvp * vec4(a_position, 1.0f);\n"
    "}\0";

// Código fonte do Fragment Shader (calcula a cor dos pixels)
static const char *fragment_shader_src = "#version 330 core\n"
    "out vec4 FragColor;\n"
	"uniform vec3 u_color;"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(u_color, 1.0f);\n"
    "}\n\0";


App::App(void) {
	glfwInit();

	// Configura o GLFW para usar o OpenGL 3.3 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1200, 900, "Trabalho", NULL, NULL);

	if(window == NULL) {
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);

	// Carrega os ponteiros de função do OpenGL usando o GLAD
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize OpenGL.\n");
		exit(-1);
	}

	glViewport(0, 0, 1200, 900);
	// Habilita o teste de profundidade para que objetos mais próximos cubram os mais distantes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Compila os shaders
	main_shader = std::make_unique<Shader>(vertex_shader_src, fragment_shader_src);

	// Constrói os objetos da cena
	build();
}

void App::run(void) {
	while(!glfwWindowShouldClose(window)) {
		loop();
	}
}

App::~App(void) {
}

void App::loop(void) {
	float current_frame = float(glfwGetTime());
	dt = current_frame - last_frame;
	last_frame = current_frame;

	// Limpa a tela e o buffer de profundidade a cada frame
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	updateScene();

	// Envia as matrizes de transformação para o shader
	main_shader->setUniformMat4("u_view", view);
	main_shader->setUniformMat4("u_projection", projection);
	main_shader->use();
	
	// Renderiza os objetos
	table.render(*main_shader);
	snes.render(*main_shader);
	controller.render(*main_shader);
	monitor.render(*main_shader);
	pacman.render(*main_shader);
	book.render(*main_shader);
	button.render(*main_shader);
	scenary.render(*main_shader);

	// a lógica é responsável por processar qualquer input
	logic.processInput(window, dt);
	*/
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void App::build(void) {
	/*
	buildTable();
	buildSnes();
	buildController();
	buildMonitor();
	buildPacman();
	buildBook();
	buildButton();
	buildScenary();
	buildScene();
	*/
}

