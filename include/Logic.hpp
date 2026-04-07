#ifndef LOGIC_HPP
#define LOGIC_HPP

#include "Global.hpp"

#include "glm/mat4x4.hpp"

// Classe responsável pela parte interativa do trabalho
class Logic {
	public:
		Logic(void);

		void processInput(GLFWwindow *window, float dt);

		glm::mat4 pacman_model = glm::mat4(1.0f);
		glm::mat4 controller_model = glm::mat4(1.0f);
		glm::mat4 book_model = glm::mat4(1.0f);
		glm::mat4 button_model = glm::mat4(1.0f);

		glm::mat4 base_pacman_model = glm::mat4(1.0f);
		glm::mat4 base_controller_model = glm::mat4(1.0f);
		glm::mat4 base_book_model = glm::mat4(1.0f);

	private:
		// definição das matrizes bases
		void setUpMatrices(void);
		// calculo das matrizes com bases nos seus movimentos
		void computeMatrices(void);

		// a rotação do controle
		float controller_rotation;
		// a escala do pacman (a escala segue uma função seno periódica)
		float pacman_scaling;
		// a penetração do botão no controle (quando ele é apertado)
		float button_penetration;

		// a posição do livro (que pode mudar, ele é o objeto a mudar de posição)
		glm::vec3 book_position;

		// variáveis apenas para alternar entre o modo wireframe
		int old_p_state;
		int gl_fill_mode;
};

#endif
