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
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void App::build(void) {
	buildTable();
	buildSnes();
	buildController();
	buildMonitor();
	buildPacman();
	buildBook();
	buildButton();
	buildScenary();
	buildScene();
}

// Constrói a geometria da mesa
void App::buildTable(void) {
	std::vector<glm::vec3> vertices;

	glm::mat4 model(1.0f);
	glm::vec3 color(0.4f, 0.2f, 0.1f);
	int counter = 0;

	const float table_width = 12.0f;
	const float table_height = 8.0f;
	const float table_thickness = 0.2f;
	const float table_foot_thickness = 0.2f;
	const float table_foot_height = 6.0f;

	// Tampo da mesa
	model = glm::scale(model, glm::vec3(table_width, table_thickness, table_height));

	Builder::addCube(vertices, model);
	table.addCubeShading(counter, color);

	// Constrói os 4 pés da mesa
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

// Constrói a geometria do SNES
void App::buildSnes(void) {
	std::vector<glm::vec3> vertices;
	glm::mat4 model(1.0f);
	int counter = 0;
	const float final_scale = 1.0f / 48.0f;

	const glm::vec3 base_color(0.7f);
	const glm::vec3 power_color(0.45f, 0.25f, 0.86f);
	const glm::vec3 reset_color(0.5f);

	// Base do console
	{
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(32.0f, 8.0f, 48.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, base_color);
	}

	// Parte mais elevada do console
	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 8.0f, 24.0f));
		model = glm::scale(model, glm::vec3(32.0f, 2.0f, 12.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, base_color);
	}

	// Detalhes frontais e botões laterais (Power/Eject)
	for(int i = 0; i < 2; i++) {
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(4.0f + i * 16.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(8.0f, 9.0f, 25.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, glm::vec3(0.6f));

		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(3.0f + i * 16.0f, 8.0f, 12.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.8f, 2.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, power_color);
	}

	// Botão Reset
	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(12.0f, 8.0f, 4.0f));
		model = glm::scale(model, glm::vec3(8.0f, 0.5f, 8.0f));

		Builder::addCube(vertices, model);
		snes.addCubeShading(counter, reset_color);
	}

	// Aplica a escala final em todos os vértices gerados
	for(auto& vertex : vertices) {
		vertex *= final_scale;
	}

	snes.object = std::make_unique<Object3d>(vertices);
}

// Constrói a geometria do controle do SNES
void App::buildController(void) {
	std::vector<glm::vec3> vertices;

	glm::mat4 model(1.0f);
	int counter = 0;

	const float final_scale = 1.0f / 40.0f;
	const glm::vec3 base_color(0.7f);

	// Lado esquerdo
	{
		model = glm::mat4(1.0f);

		model = glm::scale(model, glm::vec3(16.0f, 16.0f, 4.0f));

		Builder::addCylinder(vertices, model);
		controller.addCylinderShading(counter, base_color);
	}

	// Parte central do controle
	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(8.0f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(24.0f, 10.0f, 4.0f));

		Builder::addCube(vertices, model);
		controller.addCubeShading(counter, base_color);
	}

	// Lado direito
	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(24.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(16.0f, 16.0f, 4.0f));

		Builder::addCylinder(vertices, model);
		controller.addCylinderShading(counter, base_color);
	}

	// Direcional do controle
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

	// Botões do controle (A, B, X, Y)
	{
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

	// Botões Start e Select no centro do controle
	for(int i = 0; i < 2; i++) {
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(16.0f + i * 5.0f, 10.0f, 4.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 2.0f));

		Builder::addCube(vertices, model);
		controller.addCubeShading(counter, glm::vec3(0.1f));
	}

	for(auto& vertex : vertices) {
		vertex *= final_scale;
	}

	controller.object = std::make_unique<Object3d>(vertices);
}

void App::buildMonitor(void) {
	std::vector<glm::vec3> vertices;

	glm::mat4 model(1.0f);
	int counter = 0;

	const float final_scale = 1.0f / 36.0f;
	const glm::vec3 base_color(0.95f, 0.90f, 0.76f);

	// vertices internos e externos do monitor, para fazer
	// o formato trapezóide a partir de quads
	const std::array<glm::vec3, 4> inner_vertices = {
		glm::vec3(4.0f,  4.0f, 0.0f),
		glm::vec3(32.0f, 4.0f, 0.0f),
		glm::vec3(32.0f, 28.0f, 0.0f),
		glm::vec3(4.0f,  28.0f, 0.0f),
	};

	const std::array<glm::vec3, 4> outer_vertices = {
		glm::vec3(6.0f,  6.0f, -16.0f),
		glm::vec3(30.0f, 6.0f, -16.0f),
		glm::vec3(30.0f, 26.0f, -16.0f),
		glm::vec3(6.0f,  26.0f, -16.0f),
	};

	// os dois for abaixo servem para fazer a moldura
	// do monitor
	for(int i = 0; i < 2; i++) {
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 28.0f * i, 0.0f));
		model = glm::scale(model, glm::vec3(36.0f, 4.0f, 8.0f));

		Builder::addCube(vertices, model);
		monitor.addCubeShading(counter, base_color);
	}

	for(int i = 0; i < 2; i++) {
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f + i * 32.0f, 4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 24.0f, 8.0f));

		Builder::addCube(vertices, model);
		monitor.addCubeShading(counter, base_color);
	}

	// os dois próximos blocos de código fazem a
	// base do monitor
	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(16.0f, -8.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 8.0f, 6.0f));

		Builder::addCube(vertices, model);
		monitor.addCubeShading(counter, base_color);
	}

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(8.0f, -12.0f, -6.0f));
		model = glm::scale(model, glm::vec3(20.0f, 4.0f, 16.0f));

		Builder::addCube(vertices, model);
		monitor.addCubeShading(counter, base_color);
	}

	// a tela do monitor
	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(4.0f, 4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(28.0f, 24.0f, 6.0f));

		Builder::addCube(vertices, model);
		monitor.addCubeShading(counter, glm::vec3(0.0f));
	}

	// esse bloco de código faz a face traseira e o trapezóide,
	// que não são visíveis na cena, só com wireframe
	{
		Builder::addQuad(vertices, outer_vertices);

		monitor.parts.emplace_back(counter, 6, base_color * 0.8f);
		counter += 6;
	}

	for(int i = 0; i < 4; i++) {
		int first = i;
		int last = (i + 1) % 4;

		const std::array<glm::vec3, 4> quad_vert = {
			inner_vertices[first],
			inner_vertices[last],
			outer_vertices[last],
			outer_vertices[first]
		};

		Builder::addQuad(vertices, quad_vert);
		monitor.parts.emplace_back(counter, 6, base_color);
		counter += 6;
	}

	for(auto& vertex : vertices) {
		vertex *= final_scale;
	}

	monitor.object = std::make_unique<Object3d>(vertices);
}

void App::buildPacman(void) {
	std::vector<glm::vec3> vertices;

	glm::mat4 model(1.0f);
	int counter = 0;

	const float final_scale = 1.0f / 8.0f;
	const glm::vec3 base_color(1.0f, 1.0f, 0.0f);

	// Para fazer o pacman sem textura, é necessário montá-lo através de primitivas.
	// Como utilizar um bloco por pixel é algo desnecessário, podemos utilizar faixas
	// horizontais no lugar disso, utilizando muito menos primitivas.
	// Os dados a seguir representam:
	// - deslocamento horizontal a partir do eixo x (esquerda)
	// - tamanho da faixa de pixels
	const std::vector<int> pixel_data = {
		2, 4,
		1, 6,
		0, 5,
		0, 4,
		0, 3,
		0, 5,
		1, 6,
		2, 4,
	};

	// Com os dados acima, é trivial montar uma
	// figura do estilo pacman
	for(size_t i = 0; i < pixel_data.size(); i++) {
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(pixel_data[i * 2], i, 0.0f));
		model = glm::scale(model, glm::vec3(pixel_data[i * 2 + 1], 1.0f, 0.1f));

		Builder::addCube(vertices, model);
	}

	/* 36 * o número de retângulos (paralelepípedos) */
	pacman.parts.emplace_back(counter, 36 * pixel_data.size() / 2, base_color);

	for(auto& vertex : vertices) {
		vertex *= final_scale;
	}

	pacman.object = std::make_unique<Object3d>(vertices);
}

void App::buildBook(void) {
	std::vector<glm::vec3> vertices;

	glm::mat4 model(1.0f);
	int counter = 0;

	const float final_scale = 1.0f / 32.0f;
	const glm::vec3 base_color(0.76f, 0.44f, 0.25f);

	// Esse e o próximo bloco fazem a capa do livro
	for(int i = 0; i < 2; i++){
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f + i * 5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(16.0f, 1.0f, 32.0f));

		Builder::addCube(vertices, model);
		book.addCubeShading(counter, base_color);
	}

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 32.0f));

		Builder::addCube(vertices, model);
		book.addCubeShading(counter, base_color);
	}

	// Esse faz as "páginas"
	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(14.0f, 4.0f, 30.0f));

		Builder::addCube(vertices, model);
		book.addCubeShading(counter, glm::vec3(1.0f));
	}

	// E os blocos restantes fazem a letra "C" que aparece
	// no livro.
	for(int i = 0; i < 2; i++) {
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(4.0f, 6.1f, 8.0f));
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 8.0f * i));
		model = glm::scale(model, glm::vec3(4.0f, 0.01f, 1.0f));

		Builder::addCube(vertices, model);
	}

	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			model = glm::mat4(1.0f);

			model = glm::translate(model, glm::vec3(4.0f, 6.1f, 8.0f));
			model = glm::translate(model, glm::vec3(1.0f + 5.0f * i, 0.0f, 1.0f + 6.0f * j));
			model = glm::scale(model, glm::vec3(1.0f, 0.01f, 1.0f));

			Builder::addCube(vertices, model);

		}
	}

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(4.0f, 6.1f, 8.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.01f, 4.0f));

		Builder::addCube(vertices, model);
	}

	/* 7 cubos cada um com 36 vértices */
	book.parts.emplace_back(counter, 7 * 36, glm::vec3(1.0f));

	for(auto& vertex : vertices) {
		vertex *= final_scale;
	}

	book.object = std::make_unique<Object3d>(vertices);
}

// Esse daqui é o botão individual do snes, no código ele é
// tratado como um objeto separado para ter sua própria matriz
// model.
void App::buildButton(void) {
	std::vector<glm::vec3> vertices;

	glm::mat4 model(1.0f);
	int counter = 0;

	const float final_scale = 1.0f / 40.0f;

	{
		model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(34.0f, 6.0f, 4.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 1.5f));

		Builder::addCylinder(vertices, model);
		button.addCylinderShading(counter, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	for(auto& vertex : vertices) {
		vertex *= final_scale;
	}

	button.object = std::make_unique<Object3d>(vertices);
}

void App::buildScenary(void) {
	std::vector<glm::vec3> vertices;

	int counter = 0;
	glm::mat4 model(1.0f);

	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, -8.0f, -15.0f));
		model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));

		Builder::addCube(vertices, model);
		scenary.addCubeShading(counter, glm::vec3(1.0f));
	}

	scenary.object = std::make_unique<Object3d>(vertices);
}

// Define as matrizes das partes estáticas da cena
void App::buildScene(void) {
	glm::mat4 model;
	view = glm::mat4(1.0f);
	
	// Afasta a câmera da origem
	view = glm::translate(view, glm::vec3(-6.0f, -3.9f, -16.0f));
	view = glm::rotate(view, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(glm::radians(45.0f), 1.33f, 0.01f, 100.0f);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(8.0f, 1.5f, 0.9f));
	model = glm::scale(model, glm::vec3(4.0f));
	model = glm::rotate(model, -0.3f, glm::vec3(0.0f, 1.0f, 0.0f));
	monitor.model = model;

	model = glm::translate(model, glm::vec3(0.4f, 0.3f, 0.2f));
	model = glm::scale(model, glm::vec3(0.25f));
	logic.base_pacman_model = model;
	pacman.model = model;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
	table.model = model;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(7.0f, 0.2f, 2.0f));
	model = glm::scale(model, glm::vec3(4.0f));
	model = glm::rotate(model, -3.2f, glm::vec3(0.0f, 1.0f, 0.0f));
	snes.model = model;
}

// Atualiza as matrizes da cena de acordo com o módulo de Lógica
void App::updateScene(void) {
	controller.model = logic.controller_model;
	pacman.model = logic.pacman_model;
	book.model = logic.book_model;
	button.model = logic.button_model;
}
