#include "ObjectHandler.hpp"
#include "Builder.hpp"

MeshPart::MeshPart(int first, int count, glm::vec3 color) :
	first(first),
	count(count),
	color(color)
{
}

void ObjectHandler::addCubeShading(int& counter, const glm::vec3& color) {
	parts.emplace_back( 0 + counter, 12, color * 0.8f);
	parts.emplace_back(12 + counter, 12, color * 1.0f);
	parts.emplace_back(24 + counter, 12, color * 0.9f);

	counter += 36;
}

void ObjectHandler::addCylinderShading(int& counter, const glm::vec3& color) {
	parts.emplace_back(
			0 + counter,
			Builder::CYLINDER_TOP_NUM_VERTICES,
			color * 0.8f
			);

	parts.emplace_back(
			Builder::CYLINDER_TOP_NUM_VERTICES + counter,
			Builder::CYLINDER_SIDE_NUM_VERTICES,
			color * 1.0f
			);

	counter += Builder::CYLINDER_SIDE_NUM_VERTICES + Builder::CYLINDER_TOP_NUM_VERTICES;
}

void ObjectHandler::render(Shader& shader) {
	shader.setUniformMat4("u_model", model);

	shader.use();

	for(const auto& part : parts) {
		shader.setUniformVec3("u_color", part.color);
		object->renderPart(part.first, part.count);
	}
}
