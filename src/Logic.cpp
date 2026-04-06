#include "Logic.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>

Logic::Logic(void) {
	old_p_state = GLFW_PRESS;
	gl_fill_mode = GL_FILL;
	controller_rotation = 0.0f;

	pacman_scaling = 1.0f;

	setUpMatrices();
}

void Logic::processInput(GLFWwindow *window, float dt) {
	// lógica para exibir wireframes sem usar callback
	{
		int new_p_state;
	
		new_p_state = glfwGetKey(window, GLFW_KEY_P);
	
		if(new_p_state == GLFW_PRESS && new_p_state != old_p_state) {
			gl_fill_mode = (gl_fill_mode == GL_FILL) ? GL_LINE : GL_FILL;
			glPolygonMode(GL_FRONT_AND_BACK, gl_fill_mode);
		}
	
		old_p_state = new_p_state;
	}

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		controller_rotation -= 1.5f * dt;
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		controller_rotation += 1.5f * dt;
	}

	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		pacman_scaling += 3.0f * dt;
		button_penetration += 8.0f * dt;
	} else {
		button_penetration -= 8.0f * dt;
	}

	if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		book_position.z -= 0.01f;
	}

	if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		book_position.z += 0.01f;
	}

	if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		book_position.x -= 0.01f;
	}

	if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		book_position.x += 0.01f;
	}

	if(book_position.x < 0.0f) book_position.x = 0.0f;
	if(book_position.x > 0.3f) book_position.x = 0.3f;
	if(book_position.z < -0.5f) book_position.z = -0.5f;
	if(book_position.z > 0.0f) book_position.z = 0.0f;

	if(button_penetration < 0.0f) button_penetration = 0.0f;
	if(button_penetration > 1.0f) button_penetration = 1.0f;

	computeMatrices();
}

void Logic::setUpMatrices(void) {
	glm::mat4 model;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(6.0f, 0.5f, 7.0f));
	model = glm::scale(model, glm::vec3(4.0f));

	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
	model = glm::rotate(model, -0.2f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));

	base_controller_model = model;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f));

	base_book_model = model;
}

void Logic::computeMatrices(void) {
	controller_model = base_controller_model;

	controller_model = glm::translate(controller_model, glm::vec3(0.5f, 0.0f, +0.1f));
	controller_model = glm::rotate(controller_model, controller_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	controller_model = glm::translate(controller_model, glm::vec3(-0.5f, 0.0f, -0.1f));

	pacman_model = base_pacman_model;

	pacman_model = glm::translate(pacman_model, glm::vec3(0.5f, 0.5f, 0.0f));
	pacman_model = glm::scale(pacman_model, glm::vec3((sinf(pacman_scaling) + 1.0f) / 2.0f + 0.5f));
	pacman_model = glm::translate(pacman_model, glm::vec3(-0.5f, -0.5f, 0.0f));

	book_model = base_book_model;
	book_model = glm::translate(book_model, book_position);

	button_model = controller_model;
	button_model = glm::translate(button_model, glm::vec3(0.0f, 0.0f, -button_penetration * 0.03f));
}
