#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
	public:
		Shader(const char *vertex_shader_src, const char *fragment_shader_src);

		void use(void);

		~Shader(void);

	private:
		unsigned int id = 0;
};

#endif
