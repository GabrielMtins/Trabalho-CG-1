#include "Builder.hpp"

#include "glm/vec4.hpp"

#include <cmath>
#include <cstdio>

const int Builder::CYLINDER_SIDE_NUM_VERTICES = 48;
const int Builder::CYLINDER_TOP_NUM_VERTICES = 48;

const std::vector<glm::vec3> Builder::cube_vertices = {
	{0.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 1.0f},
	
	{0.0f, 0.0f, 0.0f},			
	{1.0f, 0.0f, 0.0f},
	{1.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},

	// face 3
	{0.0f, 0.0f, 1.0f},		 
	{0.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 1.0f},

	// face 6
	{1.0f, 0.0f, 0.0f},		 
	{1.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 0.0f},

	// face 4
	{1.0f, 0.0f, 0.0f},		 
	{0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 1.0f},
	
	// face 5
	{1.0f, 1.0f, 1.0f},		   
	{0.0f, 1.0f, 1.0f},
	{0.0f, 1.0f, 0.0f},
	{1.0f, 1.0f, 0.0f},
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
	std::array<glm::vec3, 4> face_vertices;

	for(size_t i = 0; i < cube_vertices.size(); i += 4) {
		for(size_t j = 0; j < 4; j++) {
			face_vertices[j] = applyTransform(cube_vertices[i + j], transform);
		}

		addQuad(vertices, face_vertices);
	}

	/*
	for(const glm::vec3& vertex : cube_vertices) {
		std::array
		vertices.push_back(applyTransform(vertex, transform));
	}
	*/
}

void Builder::addCylinder(std::vector<glm::vec3>& vertices, const glm::mat4& transform) {
	if(cylinder_vertices.empty()) {
		fillCylinder();
	}

	for(const glm::vec3& vertex : cylinder_vertices) {
		vertices.push_back(applyTransform(vertex, transform));
	}
}

void Builder::addQuad(std::vector<glm::vec3>& vertices, const std::array<glm::vec3, 4> points) {
	for(int i = 0; i < 3; i++) {
		vertices.push_back(
				points[i]
				);
	}

	for(int i = 0; i < 3; i++) {
		vertices.push_back(
				points[(i + 2) % 4]
				);
	}
}

glm::vec3 Builder::applyTransform(const glm::vec3& vertex, const glm::mat4& transform) {
	return glm::vec3(transform * glm::vec4(vertex, 1.0f));
}

void Builder::fillCylinder(void) {
	glm::vec3 offset(0.0f, 0.0f, 1.0f);

	for(size_t i = 0; i < 8; i++) {
		cylinder_vertices.emplace_back(glm::vec3(0.0f) + offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 1) % 8] + offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[i + 0] + offset);

		cylinder_vertices.emplace_back(glm::vec3(0.0f) - offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 1) % 8] - offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[i + 0] - offset);
	}

	for(size_t i = 0; i < 8; i++) {
		std::array<glm::vec3, 4> quad_arr = {
			cylinder_vertices_2d[(i + 0)] + offset,
			cylinder_vertices_2d[(i + 1) % 8] + offset,
			cylinder_vertices_2d[(i + 1) % 8] - offset,
			cylinder_vertices_2d[(i + 0)] - offset,
		};

		addQuad(cylinder_vertices, quad_arr);

		/*
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 0)] + offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 1) % 8] + offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 0)] - offset);
		cylinder_vertices.emplace_back(cylinder_vertices_2d[(i + 1) % 8] - offset);
		*/
	}

	for(auto& vertex : cylinder_vertices) {
		vertex += glm::vec3(1.0f, 1.0f, 1.0f);
		vertex /= 2;
	}
}
