#ifndef PROGRAM_H
#define PROGRAM_H

#include "Shader.h"
#include <vector>

class Program
{
public:
	Program();

	void init();
	void use() const;
	void clear_use() const;
	unsigned int get() const;
private:
	unsigned int program;
	Shader shader;

	void create_program(std::vector<unsigned int> const& shaders);
};

#endif
