#ifndef LOGIC_HPP
#define LOGIC_HPP

#include "Global.hpp"

#include "glm/mat4x4.hpp"

// Classe responsável pela parte interativa do trabalho
class Logic {
	public:
		Logic(void);

		void processInput(GLFWwindow *window, float dt);

		glm::mat4 view;
		glm::mat4 book_matrix;
		glm::mat4 chair_matrix;
		glm::mat4 tree_matrix;

	private:
		void processMouseMovement(GLFWwindow *window, float dt);
		// definição das matrizes bases
		void setUpMatrices(void);
		// calculo das matrizes com bases nos seus movimentos
		void computeMatrices(void);

		struct {
			glm::vec3 pos = glm::vec3(0.0f);
			float pitch = -0.9f, yaw = -0.0f;
		} camera;

		float book_rotation;
		float chair_offset;
		float tree_scale;

		glm::mat4 base_book_matrix;
		glm::mat4 base_chair_matrix;
		glm::mat4 base_tree_matrix;

		int old_p_state;
		int gl_fill_mode;
};

#endif
