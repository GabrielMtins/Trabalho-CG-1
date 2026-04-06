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
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}

void Object3d::renderPart(int first, int count) {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, first, count);
}

Object3d::~Object3d(void) {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER_BINDING, 0);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
