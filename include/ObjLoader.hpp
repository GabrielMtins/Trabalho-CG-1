#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include <vector>
#include "Vertex.hpp"
#include "ObjectHandler.hpp"

#include <unordered_map>

class ObjLoader {
	public:
		std::unique_ptr<ObjectHandler> load(const std::string& path);

	private:
		void pushToVertexBuffer(int v, int vt, int vn);
		void loadMtl(const std::string& path);

		std::vector<glm::vec3> vs;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		std::vector<Vertex> vertices;
		std::unordered_map<std::string, unsigned int> texture_loaded;
};

#endif
