#ifndef APP_HPP
#define APP_HPP

#include "Global.hpp"
#include "Logic.hpp"
#include <GLFW/glfw3.h>

#include <memory>

#include "Shader.hpp"
#include "Object3d.hpp"
#include "ObjectHandler.hpp"
#include "ObjLoader.hpp"

class App {
	public:
		App(void);

		void run(void);

		~App(void);

	private:
		// game loop
		void loop(void);
		void renderSkybox(void);
		void renderTrees(void);
		void updateScene(void);

		// funções de construção da cena
		void build(void);
		void buildSkyboxCube(void);
		void buildHouse(void);
		void buildGrassFloor(void);

		// objetos dentro da casa
		void buildBed(void);
		void buildChest(void);
		void buildChair(void);
		void buildBench(void);
		void buildTable(void);
		void buildBook(void);
		void buildLights(void);

		void buildBigTree(void);
		void buildSmallTree(void);

		void buildCandle(void);
		void buildLantern(void);
		void buildTorch(void);

		GLFWwindow *window = NULL;

		std::unique_ptr<Shader> main_shader = nullptr;
		std::unique_ptr<Shader> skybox_shader = nullptr;

		glm::mat4 view;
		glm::mat4 projection;

		Logic logic;

		std::unique_ptr<ObjectHandler> house_handler;
		std::unique_ptr<ObjectHandler> floor_handler;

		// objetos *de facto*
		std::unique_ptr<ObjectHandler> bed_handler;
		std::unique_ptr<ObjectHandler> chest_handler;
		std::unique_ptr<ObjectHandler> chair_handler;
		std::unique_ptr<ObjectHandler> bench_handler;
		std::unique_ptr<ObjectHandler> table_handler;
		std::unique_ptr<ObjectHandler> book_handler;

		std::unique_ptr<ObjectHandler> big_tree_handler;
		std::unique_ptr<ObjectHandler> small_tree_handler;

		std::unique_ptr<ObjectHandler> candle_handler;
		std::unique_ptr<ObjectHandler> lantern_handler;
		std::unique_ptr<ObjectHandler> torch_handler;

		ObjLoader obj_loader;

		float last_frame = 0.0f;
		float dt = 0.0f;

		std::vector<glm::vec3> tree_positions;

		std::unique_ptr<Object3d> skybox_cube;
		unsigned int skybox_texture;
};

#endif
