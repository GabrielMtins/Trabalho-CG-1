#include "Object3d.hpp"

// Construtor: aloca as estruturas do OpenGL e envia os vértices para a GPU
Object3d::Object3d(const std::vector<glm::vec3> vertices) {
	num_vertices = vertices.size();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Envia o array de posições
	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(glm::vec3) * vertices.size(),
			vertices.data(),
			GL_STATIC_DRAW
			);

	// Explica ao OpenGL como ler o buffer (3 floats x, y, z para cada vértice)
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

// Desenha a malha completa
void Object3d::render(void) {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}

// Permite desenhar apenas uma fatia específica do array
void Object3d::renderPart(int first, int count) {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, first, count);
}

Object3d::~Object3d(void) {
	// Liberar da memória os objetos
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER_BINDING, 0);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
