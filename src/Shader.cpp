#include "Shader.hpp"
#include "Global.hpp"

#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const char *vertex_shader_src, const char *fragment_shader_src) {
	unsigned int vertex_shader, fragment_shader;
	int  success;
	char info_log[512];

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if(!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("Error: %s\n", info_log);
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

	if(!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		printf("Error: %s\n", info_log);
	}

	id = glCreateProgram();

	glAttachShader(id, vertex_shader);
	glAttachShader(id, fragment_shader);
	glLinkProgram(id);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if(!success) {
		glGetProgramInfoLog(id, 512, NULL, info_log);
		printf("Error: %s\n", info_log);
	}
}

void Shader::use(void) {
	glUseProgram(id);
}

Shader::~Shader(void) {
	glDeleteProgram(id);
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& pos) {
	int location;

	location = getUniformLocation(name);

	if(location < 0) {
		printf("Error: Não há o uniform de nome: %s", name.c_str());
	}

	use();
	glUniform3f(
			location,
			pos.x,
			pos.y,
			pos.z
			);
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4& mat) {
	int location;

	location = getUniformLocation(name);

	if(location < 0) {
		printf("Error: Não há o uniform de nome: %s\n", name.c_str());
	}

	use();
	glUniformMatrix4fv(
			location,
			1,
			GL_FALSE,
			glm::value_ptr(mat)
			);

}

int Shader::getUniformLocation(const std::string& name) {
	return glGetUniformLocation(id, name.c_str());
}
