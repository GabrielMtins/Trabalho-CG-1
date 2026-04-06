#ifndef OBJECT_3D_HPP
#define OBJECT_3D_HPP

#include <vector>

#include "Global.hpp"
#include "glm/vec3.hpp"

class Object3d {
	public:
		Object3d(const std::vector<glm::vec3> vertices);

		void render(void);
		void renderPart(int first, int count);

		~Object3d(void);

	private:
		unsigned int vao, vbo;
		size_t num_vertices;
};

#endif
