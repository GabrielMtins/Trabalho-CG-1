#include "ObjectHandler.hpp"
#include "Builder.hpp"

MeshPart::MeshPart(int first, int count, glm::vec3 color, unsigned int texture) :
	first(first),
	count(count),
	color(color),
	texture(texture)
{
}

// Registra as partes de um cubo e altera ligeiramente a cor de cada grupo de faces
// para criar um efeito de volume/iluminação sem precisar de cálculos reais de luz
// adicionado principalmente para permitir a diferenciação entre as faces do modelo
void ObjectHandler::addCubeShading(int& counter, const glm::vec3& color) {
	parts.emplace_back( 0 + counter, 12, color * 0.8f);
	parts.emplace_back(12 + counter, 12, color * 1.0f);
	parts.emplace_back(24 + counter, 12, color * 0.9f);

	// 36 vértices ao todo por cubo (6 faces * 2 triângulos * 3 vértices)
	counter += 36;
}

// Mesmo princípio para o cilindro, escurecendo a tampa em relação às laterais
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

// Renderiza todas as partes registradas aplicando a cor certa via uniform antes do draw call
void ObjectHandler::render(Shader& shader) {
	glActiveTexture(GL_TEXTURE0);

	shader.setUniformInt("u_texture", 0);
	shader.setUniformMat4("u_model", model);

	shader.use();

	for(const auto& part : parts) {
		if(part.texture == 0) continue;

		glBindTexture(GL_TEXTURE_2D, part.texture);
		object->renderPart(part.first, part.count);
	}
}
