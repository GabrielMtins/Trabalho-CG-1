#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct Vertex {
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 normal = glm::vec3(0.0f);
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec2 uv = glm::vec2(0.0f);

	Vertex(void) :
		pos(glm::vec3(0.0f)),
		normal(glm::vec3(0.0f)),
		color(glm::vec3(1.0f)),
		uv(glm::vec2(0.0f))
	{
	}

	Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec3 color, glm::vec2 uv) :
		pos(pos),
		normal(normal),
		color(color),
		uv(uv)
	{
	}
};

#endif
