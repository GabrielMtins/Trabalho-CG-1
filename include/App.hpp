#ifndef APP_HPP
#define APP_HPP

#include "glad/glad.h"
#include <GLFW/glfw3.h>

class App {
	public:
		App(void);

		void run(void);

		~App(void);

	private:
		void loop(void);

		GLFWwindow *window = NULL;
};

#endif
