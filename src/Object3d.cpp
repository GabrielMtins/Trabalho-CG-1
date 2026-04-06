#include "Object3d.hpp"

Object3d::Object3d(const std::vector<glm::vec3> vertices) {
	num_vertices = vertices.size();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(glm::vec3) * vertices.size(),
			vertices.data(),
			GL_STATIC_DRAW
			);

	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			3 * sizeof(float),
			(void *) 0
			);

	glEnableVertexAttribArray(0);
}

void Object3d::render(void) {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, num_vertices);
}

Object3d::~Object3d(void) {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
