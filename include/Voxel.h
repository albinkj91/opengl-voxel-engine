#ifndef VOXEL_H
#define VOXEL_H

#include "Texture.h"
#include "Program.h"
#include <vector>
#include <glm/vec3.hpp>

class Voxel
{
public:
	Voxel(glm::vec3 const& position, Program const& program);
	virtual ~Voxel() = default;

	glm::vec3 get_position();
	virtual void render() const = 0;
protected:
	glm::vec3 position;
	Program program;
};

#endif
