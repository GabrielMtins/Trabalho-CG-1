#include "App.hpp"
#include "Builder.hpp"

#include <cstdio>
#include <cstdlib>

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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

static const char *fragment_shader_src = "#version 330 core\n"
    "out vec4 FragColor;\n"
	"uniform vec3 u_color;"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(u_color, 1.0f);\n"
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	std::vector<glm::vec3> vertices;
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.5f));

	//Builder::addCube(vertices, model);
	Builder::addCylinder(vertices, model);

	simple_triangle = std::make_unique<Object3d>(vertices);
	main_shader = std::make_unique<Shader>(vertex_shader_src, fragment_shader_src);

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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	
	view = glm::translate(view, glm::vec3(-4.0f, -2.9f, -12.0f));
	view = glm::rotate(view, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	main_shader->setUniformMat4("u_model", model);
	main_shader->setUniformMat4("u_view", view);
	main_shader->setUniformMat4("u_projection", glm::perspective(glm::radians(45.0f), 1.33f, 0.01f, 100.0f));

	main_shader->use();
	
	controller.render(*main_shader);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void App::build(void) {
	buildTable();
	buildSnes();
	buildController();
}

void App::buildTable(void) {
	std::vector<glm::vec3> vertices;

	glm::mat4 model(1.0f);
	glm::vec3 color(0.4f, 0.2f, 0.1f);
	int counter = 0;

	const float table_width = 10.0f;
	const float table_height = 4.0f;
	const float table_thickness = 0.2f;
	const float table_foot_thickness = 0.2f;
	const float table_foot_height = 6.0f;

	model = glm::scale(model, glm::vec3(table_width, table_thickness, table_height));

	Builder::addCube(vertices, model);
	table.addCubeShading(counter, color);

	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			model = glm::mat4(1.0f);

			model = glm::translate(
					model, glm::vec3((table_width - table_foot_thickness) * i, 0.0f, (table_height - table_foot_thickness) * j));

			model = glm::scale(model, glm::vec3(table_foot_thickness, -table_foot_height, table_foot_thickness));

			Builder::addCube(vertices, model);

			table.addCubeShading(counter, color);
		}
	}

	table.object = std::make_unique<Object3d>(vertices);
}


void App::buildSnes(void) {
	std::vector<glm::vec3> vertices;
	glm::mat4 model(1.0f);
	int counter = 0;
	float final_scale = 0.10f;

	const glm::vec3 base_color(0.7f);
	const glm::vec3 power_color(0.45f, 0.25f, 0.86f);
	const glm::vec3 reset_color(0.5f);

	{
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(32.0f, 8.0f, 48.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, base_color);
	}

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 8.0f, 24.0f));
		model = glm::scale(model, glm::vec3(32.0f, 2.0f, 12.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, base_color);
	}

	for(int i = 0; i < 2; i++) {
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(4.0f + i * 16.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(8.0f, 9.0f, 25.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, base_color);

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(3.0f + i * 16.0f, 8.0f, 12.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.8f, 2.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, power_color);
	}

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(12.0f, 8.0f, 4.0f));
		model = glm::scale(model, glm::vec3(8.0f, 0.5f, 8.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, reset_color);
	}

	for(auto& vertex : vertices) {
		vertex *= final_scale;
	}

	snes.object = std::make_unique<Object3d>(vertices);
}

void App::buildController(void) {
	std::vector<glm::vec3> vertices;

	glm::mat4 model(1.0f);
	glm::vec3 color(0.4f, 0.2f, 0.1f);
	int counter = 0;

	float final_scale = 0.20f;
	const glm::vec3 base_color(0.7f);

	{
		model = glm::mat4(1.0f);

		model = glm::scale(model, glm::vec3(16.0f, 16.0f, 4.0f));

		Builder::addCylinder(vertices, model);
		controller.addCylinderShading(counter, base_color);
	}

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(8.0f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(24.0f, 10.0f, 4.0f));

		Builder::addCube(vertices, model);
		controller.addCubeShading(counter, base_color);
	}

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(24.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(16.0f, 16.0f, 4.0f));

		Builder::addCylinder(vertices, model);
		controller.addCylinderShading(counter, base_color);
	}

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(4.0f, 7.0f, 4.0f));
		model = glm::scale(model, glm::vec3(8.0f, 2.0f, 2.0f));

		Builder::addCube(vertices, model);
		controller.addCubeShading(counter, glm::vec3(0.1f));

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(7.0f, 4.0f, 4.0f));
		model = glm::scale(model, glm::vec3(2.0f, 8.0f, 2.0f));

		Builder::addCube(vertices, model);
		controller.addCubeShading(counter, glm::vec3(0.1f));
	}

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(34.0f, 6.0f, 4.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 1.5f));

		Builder::addCylinder(vertices, model);
		controller.addCylinderShading(counter, glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(28.0f, 6.0f, 4.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 1.5f));

		Builder::addCylinder(vertices, model);
		controller.addCylinderShading(counter, glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(31.0f, 9.0f, 4.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 1.5f));

		Builder::addCylinder(vertices, model);
		controller.addCylinderShading(counter, glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(31.0f, 3.0f, 4.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 1.5f));

		Builder::addCylinder(vertices, model);
		controller.addCylinderShading(counter, glm::vec3(1.0f, 1.0f, 0.0f));
	}

	for(int i = 0; i < 2; i++) {
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(16.0f + i * 5.0f, 10.0f, 4.0f));
		/*
		model = glm::translate(model, glm::vec3(+0.5f));
		model = glm::rotate(model, 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f));
		*/
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 2.0f));

		Builder::addCube(vertices, model);
		controller.addCubeShading(counter, glm::vec3(0.1f));
	}

	for(auto& vertex : vertices) {
		vertex *= final_scale;
	}

	controller.object = std::make_unique<Object3d>(vertices);
}
