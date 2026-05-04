#ifndef OBJECT_HANDLER_HPP
#define OBJECT_HANDLER_HPP

#include "Object3d.hpp"
#include "glm/mat4x4.hpp"
#include "Shader.hpp"

#include <memory>

struct MeshPart {
	int first = 0;
	int count = 0;
	glm::vec3 color;
	unsigned int texture = 0;

	MeshPart(int first, int count, glm::vec3 color = glm::vec3(1.0f), unsigned int texture = 0);
};

struct ObjectHandler {
	std::unique_ptr<Object3d> object = nullptr;
	std::vector<MeshPart> parts;
	glm::mat4 model = glm::mat4(1.0f);

	void addCubeShading(int& counter, const glm::vec3& color);
	void addCylinderShading(int& counter, const glm::vec3& color);
	void render(Shader& shader);
};

#endif
