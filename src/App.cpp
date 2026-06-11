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
	"layout (location = 1) in vec3 a_normal;\n"
	"layout (location = 3) in vec2 a_uv;\n"
	"uniform mat4 u_model;"
	"uniform mat4 u_view;"
	"uniform mat4 u_projection;"
	"out vec2 v_uv;"
	"out vec3 v_normal;"
	"out vec3 v_fragpos;"
	"void main()\n"
	"{\n"
	"   mat4 mvp = u_projection * u_view * u_model;\n"
	"   gl_Position = mvp * vec4(a_position, 1.0f);\n"
	"   v_uv = a_uv;"
	"   v_normal = mat3(u_model) * a_normal;"
	"   v_fragpos = vec3(u_model * vec4(a_position, 1.0));"
	"}\0";

// Código fonte do Fragment Shader (calcula a cor dos pixels)
static const char *fragment_shader_src = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec3 u_color;\n"
	"uniform sampler2D u_texture;\n"
	"in vec2 v_uv;"
	"in vec3 v_normal;"
	"in vec3 v_fragpos;"
	"struct Light {"
	"   vec3 pos;"
	"   vec3 color;"
	"   vec3 intensity;"
	"};"
	"struct Sun {"
	"   vec3 dir;"
	"   vec3 color;"
	"   vec3 intensity;" 
	"};"
	"uniform Light u_lights[3];"
	"uniform Sun u_sun;"
	"uniform float u_inside;"
	"uniform vec3 u_ambient;"
	"uniform vec3 u_diffuse;"
	"uniform vec3 u_specular;"
	"uniform float u_shineness;"
	"uniform vec3 u_viewpos;"
	"uniform float u_ambient_scale;"
	"uniform float u_diffuse_scale;"
	"uniform float u_specular_scale;"
	"vec3 get_full_color(int i){"
	"   vec3 norm = normalize(v_normal);"
	"   vec3 light_dir = normalize(u_lights[i].pos - v_fragpos);"
	"   float diff = max(dot(norm, light_dir), 0.0);"
	"   vec3 view_dir = normalize(u_viewpos - v_fragpos);"
	"   vec3 reflect_dir = reflect(-light_dir, norm);"
	"   float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_shineness);"
	"   return u_lights[i].intensity * u_lights[i].color * (diff * u_diffuse * u_diffuse_scale + spec * u_specular * u_specular_scale);"
	"}"
	"vec3 get_sun_color(){"
	"   vec3 norm = normalize(v_normal);"
	"   float diff = max(dot(norm, u_sun.dir), 0.0);"
	"   vec3 view_dir = normalize(u_viewpos - v_fragpos);"
	"   vec3 reflect_dir = reflect(-u_sun.dir, norm);"
	"   float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_shineness);"
	"   return u_sun.intensity * u_sun.color * (diff * u_diffuse * u_diffuse_scale + spec * u_specular * u_specular_scale);"
	"}"
	"void main()\n"
	"{\n"
	"   vec3 ambient_color = vec3(1.0);"
	"   vec3 final_color = vec3(0.0);"
	"   final_color += u_ambient * ambient_color * u_ambient_scale;"
	"   final_color += get_sun_color() * (1.0 - u_inside);"
	"   final_color += get_full_color(0) * (1.0 - u_inside);"
	"   final_color += get_full_color(1) * (u_inside);"
	"   final_color += get_full_color(2) * (u_inside);"
	"   FragColor = vec4(final_color, 1.0) * texture(u_texture, v_uv);\n"
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
	main_shader->setUniformVec3("u_viewpos", logic.getPos());
	main_shader->setUniformFloat("u_ambient_scale", logic.ambient_scale);
	main_shader->setUniformFloat("u_diffuse_scale", logic.diffuse_scale);
	main_shader->setUniformFloat("u_specular_scale", logic.specular_scale);
	main_shader->use();

	if(logic.candle_working) {
		main_shader->setUniformVec3("u_lights[1].intensity", glm::vec3(1.0f));
	} else {
		main_shader->setUniformVec3("u_lights[1].intensity", glm::vec3(0.0f));
	}

	if(logic.lantern_working) {
		main_shader->setUniformVec3("u_lights[2].intensity", glm::vec3(1.0f));
	} else {
		main_shader->setUniformVec3("u_lights[2].intensity", glm::vec3(0.0f));
	}

	if(logic.torch_working) {
		main_shader->setUniformVec3("u_lights[0].intensity", glm::vec3(1.0f));
	} else {
		main_shader->setUniformVec3("u_lights[0].intensity", glm::vec3(0.0f));
	}

	if(logic.sun_working) {
		main_shader->setUniformVec3("u_sun.intensity", glm::vec3(1.0f));
	} else {
		main_shader->setUniformVec3("u_sun.intensity", glm::vec3(0.0f));
	}

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
	candle_handler->render(*main_shader);
	lantern_handler->render(*main_shader);
	torch_handler->render(*main_shader);


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
	candle_handler->model = logic.candle_matrix;

	{
		glm::vec3 pos = glm::vec3(
				candle_handler->model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
				);

		pos.y += 0.3f;

		main_shader->setUniformVec3("u_lights[0].pos", pos);
	}
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
	buildCandle();
	buildLantern();
	buildTorch();
	buildLights();
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
	house_handler->inside = 1.0f;
	house_handler->ambient = glm::vec3(0.6f);
	house_handler->diffuse = glm::vec3(1.0f);
	house_handler->specular = glm::vec3(0.0f);
	house_handler->shineness = 32.0f;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	house_handler->model = model;
}

void App::buildGrassFloor(void) {
	floor_handler = std::make_unique<ObjectHandler>();
	floor_handler->inside = 0.0f;
	floor_handler->ambient = glm::vec3(0.2f);
	floor_handler->diffuse = glm::vec3(0.8f);
	floor_handler->specular = glm::vec3(0.0f);

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
		i.normal = glm::vec3(0.0f, 1.0f, 0.0f);
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
	bed_handler->inside = 1.0f;
	bed_handler->specular = glm::vec3(0.3f);
	bed_handler->ambient = glm::vec3(0.6f);
	bed_handler->shineness = 4.0f;
	
	model = glm::translate(model, glm::vec3(-2.7f, 0.0f, -3.0f));
	model = glm::scale(model, glm::vec3(1.2f));
	bed_handler->model = model;
}

void App::buildChest(void) {
	glm::mat4 model(1.0f);

	chest_handler = obj_loader.load("obj/in/chest.obj");
	chest_handler->inside = 1.0f;
	chest_handler->diffuse = glm::vec3(2.0f);
	chest_handler->shineness = 64.0f;
	chest_handler->ambient = glm::vec3(0.6f);

	model = glm::translate(model, glm::vec3(-2.8f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.4f));

	chest_handler->model = model;
}

void App::buildChair(void) {
	glm::mat4 model(1.0f);

	chair_handler = obj_loader.load("obj/in/Chair_1.obj");
	chair_handler->inside = 1.0f;
	chair_handler->specular = glm::vec3(0.0f);
	chair_handler->ambient = glm::vec3(0.6f);

	model = glm::translate(model, glm::vec3(+0.8f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.3f));

	chair_handler->model = model;
}

void App::buildBench(void) {
	glm::mat4 model(1.0f);

	bench_handler = obj_loader.load("obj/in/Bench.obj");
	bench_handler->inside = 0.0f;
	bench_handler->ambient = glm::vec3(0.1f);
	bench_handler->specular = glm::vec3(0.1f);
	bench_handler->diffuse = glm::vec3(0.7f);

	model = glm::translate(model, glm::vec3(+6.3f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.8f));

	bench_handler->model = model;
}

void App::buildTable(void) {
	glm::mat4 model(1.0f);

	table_handler = obj_loader.load("obj/in/Table_Large.obj");
	table_handler->inside = 1.0f;
	table_handler->ambient = glm::vec3(0.6f);
	table_handler->diffuse = glm::vec3(1.0f);
	table_handler->specular = glm::vec3(0.0f);

	model = glm::translate(model, glm::vec3(+0.8f, 0.0f, -3.3f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.2f));

	table_handler->model = model;
}

void App::buildBook(void) {
	glm::mat4 model(1.0f);

	book_handler = obj_loader.load("obj/in/Book_5.obj");
	book_handler->inside = 1.0f;

	model = glm::translate(model, glm::vec3(+0.8f, 1.05f, -3.3f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f));

	book_handler->model = model;
}

void App::buildLights(void) {
	main_shader->setUniformVec3("u_sun.dir", glm::vec3(1.0f));
	main_shader->setUniformVec3("u_sun.color", glm::vec3(1.0f, 1.0f, 1.0f));
	main_shader->setUniformVec3("u_sun.intensity", glm::vec3(1.0f));
}

void App::buildBigTree(void) {
	glm::mat4 model(1.0f);

	big_tree_handler = obj_loader.load("obj/trees/tree1.obj");
	big_tree_handler->ambient = glm::vec3(0.3f);
	big_tree_handler->diffuse = glm::vec3(0.5f);
	big_tree_handler->specular = glm::vec3(0.1f);

	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(5.5f));
	model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 0.0f));

	big_tree_handler->model = model;
}

void App::buildSmallTree(void) {
	glm::mat4 model(1.0f);

	small_tree_handler = obj_loader.load("obj/trees/tree2.obj");
	small_tree_handler->ambient = glm::vec3(0.3f);
	small_tree_handler->diffuse = glm::vec3(0.5f);
	small_tree_handler->specular = glm::vec3(0.3f);

	model = glm::translate(model, glm::vec3(+9.8f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f));
	model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 0.0f));

	small_tree_handler->model = model;
}

void App::buildCandle(void) {
	glm::mat4 model(1.0f);

	candle_handler = obj_loader.load("obj/in/Candle_2.obj");
	candle_handler->inside = 0.0f;
	candle_handler->ambient = glm::vec3(0.8f);
	candle_handler->diffuse = glm::vec3(2.0f);
	candle_handler->specular = glm::vec3(0.0f);

	model = glm::translate(model, glm::vec3(6.5f, 1.0f, -2.3f));
	model = glm::scale(model, glm::vec3(1.5f));

	//main_shader->setUniformVec3("u_lights[1].pos", glm::vec3(0.0f, 1.6f, -3.0f));
	main_shader->setUniformVec3("u_lights[0].pos", glm::vec3(6.5f, 1.7f, -2.0f));
	main_shader->setUniformVec3("u_lights[0].intensity", glm::vec3(1.0f));
	main_shader->setUniformVec3("u_lights[0].color", glm::vec3(1.0f, 1.0f, 0.0f));

	candle_handler->model = model;
}

void App::buildLantern(void) {
	glm::mat4 model(1.0f);

	lantern_handler = obj_loader.load("obj/in/Lantern_Wall.obj");
	lantern_handler->inside = 1.0f;
	lantern_handler->ambient = glm::vec3(0.5f);
	lantern_handler->diffuse = glm::vec3(1.0f);
	lantern_handler->specular = glm::vec3(2.0f);

	model = glm::translate(model, glm::vec3(-3.8f, 1.5f, 1.3f));
	model = glm::scale(model, glm::vec3(1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	main_shader->setUniformVec3("u_lights[2].pos", glm::vec3(-2.0f, 2.0f, 1.3f));
	main_shader->setUniformVec3("u_lights[2].intensity", glm::vec3(1.0f));
	main_shader->setUniformVec3("u_lights[2].color", glm::vec3(0.2f, 0.5f, 1.0f));

	lantern_handler->model = model;
}

void App::buildTorch(void) {
	glm::mat4 model(1.0f);

	torch_handler = obj_loader.load("obj/in/Candle_1.obj");
	torch_handler->inside = 1.0f;
	torch_handler->ambient = glm::vec3(0.7f);
	torch_handler->diffuse = glm::vec3(1.0f);
	torch_handler->specular = glm::vec3(2.0f);

	model = glm::translate(model, glm::vec3(-0.0f, 1.0f, -3.3f));
	model = glm::scale(model, glm::vec3(1.5f));

	main_shader->setUniformVec3("u_lights[1].pos", glm::vec3(0.0f, 1.4f, -3.4f));
	main_shader->setUniformVec3("u_lights[1].intensity", glm::vec3(1.0f));
	main_shader->setUniformVec3("u_lights[1].color", glm::vec3(0.7f, 0.7f, 0.0f));

	torch_handler->model = model;
}
