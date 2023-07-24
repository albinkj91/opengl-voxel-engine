#ifndef VOXEL_H
#define VOXEL_H

#include "Texture.h"
#include <vector>
#include <glm/mat4x4.hpp>

class Voxel
{
public:
	Voxel(float const x, float const y, float const z, int const type);

	static const int GRASS = 0;
	static const int DIRT = 1;
private:
	float x;
	float y;
	float z;
	int type;
};

#endif
