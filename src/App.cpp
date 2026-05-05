#include "App.hpp"
#include "Builder.hpp"
#include "ObjLoader.hpp"
#include "Texture.hpp"

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
	"layout (location = 3) in vec2 a_uv;\n"
	"uniform mat4 u_model;"
	"uniform mat4 u_view;"
	"uniform mat4 u_projection;"
	"out vec2 v_uv;"
	"void main()\n"
	"{\n"
	"   mat4 mvp = u_projection * u_view * u_model;\n"
	"   gl_Position = mvp * vec4(a_position, 1.0f);\n"
	"   v_uv = a_uv;"
	"}\0";

// Código fonte do Fragment Shader (calcula a cor dos pixels)
static const char *fragment_shader_src = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec3 u_color;\n"
	"uniform sampler2D u_texture;\n"
	"in vec2 v_uv;"
	"void main()\n"
	"{\n"
	//"   FragColor = vec4(texture(u_texture, v_uv), 1.0f);\n"
	"   FragColor = texture(u_texture, v_uv);\n"
	"   if(FragColor.a < 0.9) discard;"
	"}\n\0";

static const char *skybox_vs_src = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 a_position;\n"
	"out vec3 v_tex_coords;\n"
	"uniform mat4 u_projection;\n"
	"uniform mat4 u_view;\n"
	"void main()\n"
	"{\n"
	"	v_tex_coords = a_position;\n"
	"	gl_Position = u_projection * u_view * vec4(a_position, 1.0);"
	"}\n\0"
	;

static const char *skybox_fs_src = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 v_tex_coords;\n"
	"uniform samplerCube u_skybox;\n"
	"void main()\n"
	"{\n"
	"	FragColor = texture(u_skybox, v_tex_coords);\n"
	"}\n";


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
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Compila os shaders
	main_shader = std::make_unique<Shader>(vertex_shader_src, fragment_shader_src);
	skybox_shader = std::make_unique<Shader>(skybox_vs_src, skybox_fs_src);

	projection = glm::perspective(glm::radians(60.0f), 1.33f, 0.1f, 200.0f);
	glfwSetCursorPos(window, 600, 450);

	build();

	srand(0);

	// gerar um pequeno bosque ao redor da casa do jogador
	for(int i = 0; i < 100; i++) {
		float x = (float) (rand() %  4096 ) / 4096;
		float z = (float) (rand() %  4096 ) / 4096;

		x -= 0.5f;
		z -= 0.5f;

		x *= 25.0f;
		z *= 25.0f;

		if(x * x + z * z < 10.0f) {
			continue;
		}

		tree_positions.emplace_back(x, 0.0f, z);
	}
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

	renderSkybox();
	floor_handler->render(*main_shader);
	house_handler->render(*main_shader);

	bed_handler->render(*main_shader);
	chest_handler->render(*main_shader);
	chair_handler->render(*main_shader);
	table_handler->render(*main_shader);
	book_handler->render(*main_shader);
	bench_handler->render(*main_shader);
	small_tree_handler->render(*main_shader);

	renderTrees();

	// a lógica é responsável por processar qualquer input
	logic.processInput(window, dt);
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void App::renderSkybox(void) {
	// desativa mascara para renderizar skybox
	glDepthMask(GL_FALSE);

	glActiveTexture(GL_TEXTURE0);
	skybox_shader->setUniformInt("u_skybox", 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);

	skybox_shader->setUniformMat4("u_projection", projection);
	skybox_shader->setUniformMat4("u_view", glm::mat4(glm::mat3(view)));

	skybox_cube->render();

	glDepthMask(GL_TRUE);
}

void App::renderTrees(void) {
	for(const auto& i : tree_positions) {
		glm::mat4 preserved_model = big_tree_handler->model;

		big_tree_handler->model = glm::translate(big_tree_handler->model, i);
		big_tree_handler->render(*main_shader);

		big_tree_handler->model = preserved_model;
	}
}

// atualizar a cena
void App::updateScene(void) {
	view = logic.view;
	book_handler->model = logic.book_matrix;
	chair_handler->model = logic.chair_matrix;
	small_tree_handler->model = logic.tree_matrix;
}

// funcoes de build, basicamente carregamento inicial e setup de matrizes
void App::build(void) {
	buildSkyboxCube();
	buildHouse();
	buildGrassFloor();
	buildBed();
	buildChest();
	buildChair();
	buildBench();
	buildTable();
	buildBook();
	buildBigTree();
	buildSmallTree();
}

void App::buildSkyboxCube(void) {
	static const std::vector<glm::vec3> raw = {
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),

		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),

		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),

		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f)

	};

	std::vector<Vertex> vertices;

	for(const auto& i : raw) {
		vertices.emplace_back(
				i,
				glm::vec3(0.0f),
				glm::vec3(0.0f),
				glm::vec2(0.0f)
				);
	}

	skybox_cube = std::make_unique<Object3d>(vertices);

	skybox_texture = Texture::loadCubemap(
			{
				"obj/skybox/right.png",
				"obj/skybox/left.png",
				"obj/skybox/top.png",
				"obj/skybox/bottom.png",
				"obj/skybox/back.png",
				"obj/skybox/front.png",
			}
			);
}

void App::buildHouse(void) {
	house_handler = obj_loader.load("obj/house3/casa_T2_CG.obj");
	house_handler->model = glm::mat4(1.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	house_handler->model = model;
}

void App::buildGrassFloor(void) {
	floor_handler = std::make_unique<ObjectHandler>();

	unsigned int tex_floor = Texture::load("obj/grass/grass.png");

	std::array<Vertex, 4> base_vertices = {
		Vertex(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(+1.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(+1.0f, 0.0f, +1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(-1.0f, 0.0f, +1.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)),
	};

	for(auto& i : base_vertices) {
		i.pos *= 1024.0f;
		i.uv *= 256.0f;
		i.pos.y -= 0.001f;
	}


	std::vector<Vertex> vertices = {
		base_vertices[0], base_vertices[1], base_vertices[2],
		base_vertices[0], base_vertices[2], base_vertices[3],
	};

	floor_handler->object = std::make_unique<Object3d>(vertices);
	floor_handler->parts.emplace_back(0, 6, glm::vec3(1.0f), tex_floor);
}

void App::buildBed(void) {
	glm::mat4 model(1.0f);

	bed_handler = obj_loader.load("obj/in/Bed_Twin1.obj");
	
	model = glm::translate(model, glm::vec3(-2.7f, 0.0f, -3.0f));
	model = glm::scale(model, glm::vec3(1.2f));
	bed_handler->model = model;
}

void App::buildChest(void) {
	glm::mat4 model(1.0f);

	chest_handler = obj_loader.load("obj/in/chest.obj");

	model = glm::translate(model, glm::vec3(-2.8f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.4f));

	chest_handler->model = model;
}

void App::buildChair(void) {
	glm::mat4 model(1.0f);

	chair_handler = obj_loader.load("obj/in/Chair_1.obj");

	model = glm::translate(model, glm::vec3(+0.8f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.3f));

	chair_handler->model = model;
}

void App::buildBench(void) {
	glm::mat4 model(1.0f);

	bench_handler = obj_loader.load("obj/in/Bench.obj");

	model = glm::translate(model, glm::vec3(+6.3f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.8f));

	bench_handler->model = model;
}

void App::buildTable(void) {
	glm::mat4 model(1.0f);

	table_handler = obj_loader.load("obj/in/Table_Large.obj");

	model = glm::translate(model, glm::vec3(+0.8f, 0.0f, -3.3f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.2f));

	table_handler->model = model;
}

void App::buildBook(void) {
	glm::mat4 model(1.0f);

	book_handler = obj_loader.load("obj/in/Book_5.obj");

	model = glm::translate(model, glm::vec3(+0.8f, 1.05f, -3.3f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f));

	book_handler->model = model;
}

void App::buildBigTree(void) {
	glm::mat4 model(1.0f);

	big_tree_handler = obj_loader.load("obj/trees/tree1.obj");

	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.5f));
	model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 0.0f));

	big_tree_handler->model = model;
}

void App::buildSmallTree(void) {
	glm::mat4 model(1.0f);

	small_tree_handler = obj_loader.load("obj/trees/tree2.obj");

	model = glm::translate(model, glm::vec3(+9.8f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f));
	model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 0.0f));

	small_tree_handler->model = model;
}
