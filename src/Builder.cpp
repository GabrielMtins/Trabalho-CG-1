#include "Builder.hpp"

#include "glm/vec4.hpp"

#include <cmath>

const std::vector<glm::vec3> Builder::cube_vertices = {
	{-0.5, -0.5, +0.5},
	{+0.5, -0.5, +0.5},
	{-0.5, +0.5, +0.5},
	{+0.5, +0.5, +0.5},

	// Face 2 do Cubo
	{+0.5, -0.5, +0.5},
	{+0.5, -0.5, -0.5},		 
	{+0.5, +0.5, +0.5},
	{+0.5, +0.5, -0.5},
	
	// Face 3 do Cubo
	{+0.5, -0.5, -0.5},
	{-0.5, -0.5, -0.5},			
	{+0.5, +0.5, -0.5},
	{-0.5, +0.5, -0.5},

	// Face 4 do Cubo
	{-0.5, -0.5, -0.5},
	{-0.5, -0.5, +0.5},		 
	{-0.5, +0.5, -0.5},
	{-0.5, +0.5, +0.5},

	// Face 5 do Cubo
	{-0.5, -0.5, -0.5},
	{+0.5, -0.5, -0.5},		 
	{-0.5, -0.5, +0.5},
	{+0.5, -0.5, +0.5},
	
	// Face 6 do Cubo
	{-0.5, +0.5, +0.5},
	{+0.5, +0.5, +0.5},		   
	{-0.5, +0.5, -0.5},
	{+0.5, +0.5, -0.5}
};

const std::vector<glm::vec3> Builder::cylinder_vertices_2d = {
	{+1.0f, +0.0f, +0.0f},
	{+0.7f, +0.7f, +0.0f},
	{+0.0f, +1.0f, +0.0f},
	{-0.7f, +0.7f, +0.0f},
	{-1.0f, +0.0f, +0.0f},
	{-0.7f, -0.7f, +0.0f},
	{-0.0f, -1.0f, +0.0f},
	{+0.7f, -0.7f, +0.0f},
};

std::vector<glm::vec3> Builder::cylinder_vertices = {};

void Builder::addCube(std::vector<glm::vec3>& vertices, const glm::mat4& transform) {
	for(const glm::vec3& vertex : cube_vertices) {
		vertices.push_back(applyTransform(vertex, transform));
	}
}

void Builder::addCylinder(std::vector<glm::vec3>& vertices, const glm::mat4& transform) {
	if(cylinder_vertices.empty()) {
		fillCylinder();
	}

	for(const glm::vec3& vertex : cylinder_vertices) {
		vertices.push_back(applyTransform(vertex, transform));
	}
}

glm::vec3 Builder::applyTransform(const glm::vec3& vertex, const glm::mat4& transform) {
	return glm::vec3(transform * glm::vec4(vertex, 1.0f));
}

void Builder::fillCylinder(void) {
	glm::vec3 offset(0.0f, 0.0f, 0.5f);

	for(size_t i = 0; i < 4; i++) {
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 1) % 8] + offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[i + 0] + offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 4) % 8] + offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 5) % 8] + offset);
	}

	for(size_t i = 0; i < 4; i++) {
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 1) % 8] - offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[i + 0] - offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 4) % 8] - offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 5) % 8] - offset);
	}

	for(size_t i = 0; i < 8; i++) {
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 0)] + offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 1) % 8] + offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 0)] - offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 1) % 8] - offset);
	}
}
