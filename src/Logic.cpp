#include "Logic.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>

Logic::Logic(void) {
	old_p_state = GLFW_PRESS;
	gl_fill_mode = GL_FILL;

	setUpMatrices();
}

void Logic::processInput(GLFWwindow *window, float dt) {
	static const float max_delta_pos = 0.025f * dt;
	glm::vec3 wish_dir = glm::vec3(0.0f);
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

	// alterar a direcao desejada
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		wish_dir.x += 1.0f;
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		wish_dir.x -= 1.0f;
	}

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		wish_dir.z += 1.0f;
	}

	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		wish_dir.z -= 1.0f;
	}

	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		wish_dir.y -= 1.0f;
	}

	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		wish_dir.y += 1.0f;
	}

	// calcular rotacao do livro
	if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		book_rotation -= 1.0f * dt;
	}

	if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		book_rotation += 1.0f * dt;
	}

	// calcular deslocamento da cadeira
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		chair_offset += 1.0f * dt;
	}

	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		chair_offset -= 1.0f * dt;
	}

	if(chair_offset < 0.0f) chair_offset = 0.0f;
	if(chair_offset > 1.0f) chair_offset = 1.0f;

	if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		tree_scale += 1.0f * dt;
	}

	if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		tree_scale -= 1.0f * dt;
	}

	if(tree_scale < 0.0f) tree_scale = 0.0f;
	if(tree_scale > 1.0f) tree_scale = 1.0f;

	camera.pos += glm::vec3(glm::rotate(glm::mat4(1.0f), -camera.yaw, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(wish_dir, 1.0f)) * max_delta_pos;

	// definir limites do player no mundo
	if(camera.pos.y > -1.5f) {
		camera.pos.y = -1.5f;
	}

	if(camera.pos.y < -15.0f) {
		camera.pos.y = -15.0f;
	}

	if(camera.pos.x < -40.0f) camera.pos.x = -40.0f;
	if(camera.pos.z < -40.0f) camera.pos.z = -40.0f;

	if(camera.pos.x > +40.0f) camera.pos.x = +40.0f;
	if(camera.pos.z > +40.0f) camera.pos.z = +40.0f;

	processMouseMovement(window, dt);
	computeMatrices();
}

void Logic::processMouseMovement(GLFWwindow *window, float dt) {
	static const float sensitivity = 0.001f;
	float centerX = 1200.0f / 2.0f;
	float centerY = 900.0f / 2.0f;

	// 2. Pega a posição atual do cursor
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// 3. Calcula o deslocamento
	float xoffset = float(xpos) - centerX;
	float yoffset = centerY - float(ypos); 

	glfwSetCursorPos(window, centerX, centerY);

	// 5. Aplica a sensibilidade ao movimento
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// 6. Atualiza os ângulos
	camera.yaw += xoffset;
	camera.pitch += yoffset;

	// 7. Trava o pitch para não virar a câmera de ponta cabeça
	if (camera.pitch > glm::radians(90.0f)) {
		camera.pitch = glm::radians(90.0f);
	}
	if (camera.pitch < -glm::radians(90.0f)) {
		camera.pitch = -glm::radians(90.0f);
	}
}

// definir as matrizes de base
void Logic::setUpMatrices(void) {
	camera.pos.y = -2.0f;

	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(+0.8f, 1.05f, -3.3f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		base_book_matrix = model;
	}

	{
		glm::mat4 model(1.0f);
	
		model = glm::translate(model, glm::vec3(+0.8f, 0.0f, -0.3f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f));
		base_chair_matrix = model;
	}

	{
		glm::mat4 model(1.0f);

		model = glm::scale(model, glm::vec3(1.5f));

		base_tree_matrix = model;
	}

}

// computar as matrizes de acordo com suas atualizacoes
void Logic::computeMatrices(void) {
	view = glm::mat4(1.0f);

	view = glm::rotate(view, -camera.pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, camera.yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, camera.pos);

	book_matrix = glm::rotate(base_book_matrix, book_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	chair_matrix = glm::translate(base_chair_matrix, glm::vec3(0.0f, 0.0f, chair_offset * 1.5f));

	tree_matrix = base_tree_matrix;
	tree_matrix = glm::translate(tree_matrix, glm::vec3(+6.8f, 0.0f, -0.3f));
	tree_matrix = glm::scale(tree_matrix, glm::vec3(tree_scale * 2.0f + 1.0f));
}
