#ifndef SHADER_HPP
#define SHADER_HPP

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include <string>

class Shader {
	public:
		Shader(const char *vertex_shader_src, const char *fragment_shader_src);

		void use(void);

		~Shader(void);

		void setUniformInt(const std::string& name, int i);
		void setUniformFloat(const std::string& name, float i);
		void setUniformVec3(const std::string& name, const glm::vec3& pos);
		void setUniformMat4(const std::string& name, const glm::mat4& mat);

	private:
		int getUniformLocation(const std::string& name);

		unsigned int id = 0;
};

#endif
