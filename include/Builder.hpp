#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <array>
#include <vector>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Builder {
	public:
		static void addCube(std::vector<glm::vec3>& vertices, const glm::mat4& transform);
		static void addCylinder(std::vector<glm::vec3>& vertices, const glm::mat4& transform);
		static void addQuad(std::vector<glm::vec3>& vertices, const std::array<glm::vec3, 4> points);
		static glm::vec3 applyTransform(const glm::vec3& vertex, const glm::mat4& transform);

		static const int CYLINDER_SIDE_NUM_VERTICES;
		static const int CYLINDER_TOP_NUM_VERTICES;

	private:
		static void fillCylinder(void);

		static const std::vector<glm::vec3> cube_vertices;
		static const std::vector<glm::vec3> cylinder_vertices_2d;
		static std::vector<glm::vec3> cylinder_vertices;
};

#endif
