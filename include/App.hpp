#ifndef APP_HPP
#define APP_HPP

#include "Global.hpp"
#include "Logic.hpp"
#include <GLFW/glfw3.h>

#include <memory>

#include "Shader.hpp"
#include "Object3d.hpp"
#include "ObjectHandler.hpp"

class App {
	public:
		App(void);

		void run(void);

		~App(void);

	private:
		// game loop
		void loop(void);

		// funções de construção da cena
		void build(void);
		void buildTable(void);
		void buildSnes(void);
		void buildController(void);
		void buildMonitor(void);
		void buildPacman(void);
		void buildBook(void);
		void buildButton(void);
		void buildScenary(void);
		void buildScene(void);
		void updateScene(void);

		GLFWwindow *window = NULL;

		std::unique_ptr<Shader> main_shader = nullptr;

		glm::mat4 view;
		glm::mat4 projection;

		Logic logic;

		ObjectHandler table;
		ObjectHandler snes;
		ObjectHandler controller;
		ObjectHandler monitor;
		ObjectHandler pacman;
		ObjectHandler book;
		ObjectHandler button;

		ObjectHandler scenary;

		float last_frame = 0.0f;
		float dt = 0.0f;
};

#endif
