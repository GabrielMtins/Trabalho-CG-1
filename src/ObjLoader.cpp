#include "ObjLoader.hpp"
#include "Texture.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

static const std::string mtl_base_path = "obj/";

struct FaceIndex {
	int v_idx; // Índice do vértice
	int vt_idx; // Índice da coordenada de textura (opcional)
	int vn_idx; // Índice da normal (opcional)
};

std::unique_ptr<ObjectHandler> ObjLoader::load(const std::string& path) {
	auto handler = std::make_unique<ObjectHandler>();
	size_t first = 0, count = 0;

	vertices.clear();
	vs.clear();
	uvs.clear();
	normals.clear();

	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Erro ao abrir o arquivo: " << path << "\n";
		return nullptr;
	}

	std::string current_mtl;
	std::string line;

	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string prefix;
		ss >> prefix;

		if(prefix == "mtllib") {
			std::string mtl_path;
			ss >> mtl_path;
			loadMtl(mtl_base_path + mtl_path);
		}
		else if(prefix == "usemtl") {
			std::string mtl_name;
			ss >> mtl_name;

			if(count != 0 && !current_mtl.empty()) {
				handler->parts.emplace_back(first, count, glm::vec3(1.0f), texture_loaded.at(current_mtl));
				first = first + count;
				count = 0;
			}

			current_mtl = mtl_name;
		}
		else if (prefix == "v") {
			// Vértice geométrico
			glm::vec3 v;
			ss >> v.x >> v.y >> v.z;
			vs.push_back(v);
		} 
		else if (prefix == "vt") {
			// Coordenada de textura
			glm::vec2 vt;
			ss >> vt.x >> vt.y;
			uvs.push_back(vt);
		} 
		else if (prefix == "vn") {
			// Vetor normal
			glm::vec3 vn;
			ss >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		} 
		else if (prefix == "f") {
			// Face
			std::string faceData;
			std::vector<FaceIndex> face;

			while (ss >> faceData) {
				std::stringstream fss(faceData);
				std::string indexStr;
				
				// Formatos possíveis do OBJ:
				// 1. v/vt/vn
				// 2. v//vn
				// 3. v/vt
				// 4. v
				
				int v_idx = 0, vt_idx = 0, vn_idx = 0;
				
				// Lê o índice do vértice
				std::getline(fss, indexStr, '/');
				if (!indexStr.empty()) v_idx = std::stoi(indexStr);

				// Lê o índice da textura (se houver)
				std::getline(fss, indexStr, '/');
				if (!indexStr.empty()) vt_idx = std::stoi(indexStr);

				// Lê o índice da normal
				std::getline(fss, indexStr, '/');
				if (!indexStr.empty()) vn_idx = std::stoi(indexStr);

				// O .obj usa índices baseados em 1. Vamos converter para 0 (índices de C++).
				face.push_back({
					v_idx - 1,
					vt_idx - 1,
					vn_idx - 1
				});
			}

			if (face.size() > 3) {
                for (size_t i = 1; i < face.size() - 1; ++i) {
					pushToVertexBuffer(face[0].v_idx, face[0].vt_idx, face[0].vn_idx);
					pushToVertexBuffer(face[i].v_idx, face[i].vt_idx, face[i].vn_idx);
					pushToVertexBuffer(face[i + 1].v_idx, face[i + 1].vt_idx, face[i + 1].vn_idx);
					count += 3;
                }
            } else {
				for(const auto& i : face) {
					pushToVertexBuffer(i.v_idx, i.vt_idx, i.vn_idx);
				}
				count += 3;
            }
		}
	}

	file.close();

	if(count != 0) {
		handler->parts.emplace_back(first, count, glm::vec3(1.0f), 0);
	}

	return handler;
}

void ObjLoader::pushToVertexBuffer(int v, int vt, int vn) {
	glm::vec3 pos, normal, color;
	glm::vec2 uv;

	color = glm::vec3(1.0f);

	if(v >= 0) {
		pos = vs[v];
	}

	if(vt >= 0) {
		uv = uvs[vt];
	}

	if(vn >= 0) {
		normal = normals[vn];
	}

	vertices.emplace_back(
			pos,
			normal,
			color,
			uv
			);
}

void ObjLoader::loadMtl(const std::string& path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Erro ao abrir o arquivo: " << path << "\n";
		return;
	}

	std::string line;
	std::string current_mtl;

	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string prefix;
		ss >> prefix;

		if(prefix == "newmtl") {
			ss >> current_mtl;
		}
		else if(prefix == "map_Kd") {
			std::string text_path;
			ss >> text_path;

			texture_loaded[current_mtl] = Texture::load(text_path);
		}
		else if(prefix == "mtllib") {
			std::string mtl_path;
			ss >> mtl_path;
		}
	}
}
