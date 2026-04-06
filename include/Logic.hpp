#ifndef LOGIC_HPP
#define LOGIC_HPP

#include "Global.hpp"

#include "glm/mat4x4.hpp"

class Logic {
	public:
		Logic(void);

		void processInput(GLFWwindow *window);

		glm::mat4 pacman_model = glm::mat4(1.0f);
		glm::mat4 controller_model = glm::mat4(1.0f);

		glm::mat4 base_pacman_model = glm::mat4(1.0f);
		glm::mat4 base_controller_model = glm::mat4(1.0f);

	private:
		void setUpMatrices(void);
		void computeMatrices(void);

		float controller_rotation;

		int old_p_state;
		int gl_fill_mode;
};

#endif
