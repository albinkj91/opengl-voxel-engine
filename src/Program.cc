#define GL_GLEXT_PROTOTYPES

#include "Program.h"
#include <iostream>
#include <SFML/OpenGL.hpp>
#include <algorithm>

Program::Program()
	:program{}, shader{}
{ }

void Program::init_program()
{
	// load and compile shaders
	unsigned int vert_shader{shader.load(GL_VERTEX_SHADER, "data/vertshader.vert")};
	unsigned int frag_shader{shader.load(GL_FRAGMENT_SHADER, "data/fragshader.frag")};

	std::vector<unsigned int> shaders{};
	shaders.push_back(vert_shader);
	shaders.push_back(frag_shader);

	// link shaders to program
	create_program(shaders);
	for_each(shaders.begin(), shaders.end(), glDeleteShader);
}

void Program::create_program(std::vector<unsigned int> const& shaders)
{
	// create program object and attach shaders
	program = glCreateProgram();

	for_each(shaders.begin(), shaders.end(),
		[this](unsigned int const s)
		{
			glAttachShader(program, s);
		});

	// link program
	glLinkProgram(program);

	// check if successful
	int status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		int info_log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

		GLchar *str_info_log = new GLchar[info_log_length + 1];
		glGetProgramInfoLog(program, info_log_length, NULL, str_info_log);
		std::cout << "Linker failure: " << str_info_log << std::endl;
		delete[] str_info_log;
	}

	// detach shaders from program after linking (resource cleanup)
	for_each(shaders.begin(), shaders.end(),
		[this](unsigned int const s)
		{
			glDetachShader(program, s);
		});
}

void Program::use() const
{
	glUseProgram(program);
}

void Program::clear_use() const
{
	glUseProgram(0);
}

unsigned int Program::get() const
{
	return program;
}
