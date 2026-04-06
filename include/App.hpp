#ifndef APP_HPP
#define APP_HPP

#include "Global.hpp"
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
		void loop(void);

		void build(void);
		void buildTable(void);
		void buildSnes(void);
		void buildController(void);

		GLFWwindow *window = NULL;

		std::unique_ptr<Shader> main_shader = nullptr;
		std::unique_ptr<Object3d> simple_triangle = nullptr;

		ObjectHandler table;
		ObjectHandler snes;
		ObjectHandler controller;
};

#endif
