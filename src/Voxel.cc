#include "Voxel.h"

using namespace std;

Voxel::Voxel(float const x, float const y, float const z, int const type)
	:x{x}, y{y}, z{z}, type{type}
{ }

glm::vec3 Voxel::get_position()
{
	return glm::vec3{x, y, z};
}
