#ifndef VOXEL_H
#define VOXEL_H

#include "Texture.h"
#include <vector>
#include <glm/vec3.hpp>

class Voxel
{
public:
	Voxel(glm::vec3 pos);
	virtual ~Voxel() = default;

	glm::vec3 get_position();
	virtual void render() const = 0;
private:
	glm::vec3 position;
};

#endif
