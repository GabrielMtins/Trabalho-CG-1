#include "Shader.hpp"
#include "Global.hpp"

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
		glGetProgramInfoLog(fragment_shader, 512, NULL, info_log);
		printf("Error: %s\n", info_log);
	}
}

void Shader::use(void) {
	glUseProgram(id);
}

Shader::~Shader(void) {
	glDeleteProgram(id);
}
