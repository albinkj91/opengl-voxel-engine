#include "Voxel.h"

using namespace std;

Voxel::Voxel(glm::vec3 pos)
	:position{pos}
{ }

glm::vec3 Voxel::get_position()
{
	return position;
}
