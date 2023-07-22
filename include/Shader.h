#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <SFML/OpenGL.hpp>

class Shader
{
public:
	Shader();

	unsigned int load(GLenum const shader_type, std::string const& filename) const;
private:
	std::string load_shader(std::string const& filename) const;
	unsigned int create_shader(GLenum shader_type, std::string const& shader_as_string) const;
};

#endif
