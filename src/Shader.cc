#define GL_GLEXT_PROTOTYPES

#include "Shader.h"
#include <fstream>
#include <iostream>

using namespace std;

Shader::Shader()
{ }

unsigned int Shader::load(GLenum const shader_type, string const& filename) const
{
	string shader_as_string{load_shader(filename)};
	return create_shader(shader_type, shader_as_string);
}

string Shader::load_shader(string const& filename) const
{
	string shader{};
	string tmp{};
	ifstream ifs{filename};

	while(getline(ifs, tmp, '\n'))
	{
		shader.append(tmp)
			.append("\n");
	}
	return shader;
}

unsigned int Shader::create_shader(GLenum const shader_type, string const& shader_as_string) const
{
	// create OpenGL shader object
	unsigned int shader{glCreateShader(shader_type)};
	// shader code as c-style string
	auto source = shader_as_string.data();
	// fed into shader object. Length = NULL implies the strings are null-terminated
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// check if compilation was successful and if not, log error
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int info_log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		GLchar *str_info_log = new GLchar[info_log_length + 1];
		glGetShaderInfoLog(shader, info_log_length, NULL, str_info_log);

		string str_shader_type{};
		switch(shader_type)
		{
			case GL_VERTEX_SHADER: str_shader_type = "vertex"; break;
			case GL_GEOMETRY_SHADER: str_shader_type = "geometry"; break;
			case GL_FRAGMENT_SHADER: str_shader_type = "fragment"; break;
		}

		cout << "Compile failure in "
			 <<  str_shader_type
			 << " shader:\n"
			 << str_info_log
			 << endl;
		delete[] str_info_log;
	}
	return shader;
}
