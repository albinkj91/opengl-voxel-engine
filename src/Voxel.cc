#include "Voxel.h"

using namespace std;

Voxel::Voxel(glm::vec3 const& position, Program const& program)
	:position{position}, program{program}
{ }

glm::vec3 Voxel::get_position()
{
	return position;
}
