#include "App.hpp"

#include <cstdio>
#include <cstdlib>

#include <vector>

#include "glm/glm.hpp"


static const char *vertex_shader_src = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

static const char *fragment_shader_src = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

static const std::vector<glm::vec3> triangle_vertices = {
	{-0.5f, -0.5f, 0.0f},
	{ 0.5f, -0.5f, 0.0f},
	{ 0.0f,  0.5f, 0.0f},
};


App::App(void) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Trabalho", NULL, NULL);

	if(window == NULL) {
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize OpenGL.\n");
		exit(-1);
	}

	glViewport(0, 0, 800, 600);

	simple_triangle = std::make_unique<Object3d>(triangle_vertices);
	main_shader = std::make_unique<Shader>(vertex_shader_src, fragment_shader_src);
}

void App::run(void) {
	while(!glfwWindowShouldClose(window)) {
		loop();
	}
}

App::~App(void) {
}

void App::loop(void) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	main_shader->use();
	simple_triangle->render();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

