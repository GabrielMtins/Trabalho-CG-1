#include "Logic.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

Logic::Logic(void) {
	old_p_state = GLFW_PRESS;
	gl_fill_mode = GL_FILL;
	controller_rotation = 0.0f;

	setUpMatrices();
}

void Logic::processInput(GLFWwindow *window) {
	int new_p_state;

	new_p_state = glfwGetKey(window, GLFW_KEY_P);

	if(new_p_state == GLFW_PRESS && new_p_state != old_p_state) {
		gl_fill_mode = (gl_fill_mode == GL_FILL) ? GL_LINE : GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, gl_fill_mode);
	}

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		controller_rotation -= 0.01f;
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		controller_rotation += 0.01f;
	}

	old_p_state = new_p_state;

	computeMatrices();
}

void Logic::setUpMatrices(void) {
	glm::mat4 model;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, 0.2f, 7.0f));
	model = glm::scale(model, glm::vec3(4.0f));
	base_controller_model = model;
}

void Logic::computeMatrices(void) {
	controller_model = base_controller_model;

	controller_model = glm::translate(controller_model, glm::vec3(0.5f, 0.0f, +0.1f));
	controller_model = glm::rotate(controller_model, controller_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	controller_model = glm::translate(controller_model, glm::vec3(-0.5f, 0.0f, -0.1f));
}
