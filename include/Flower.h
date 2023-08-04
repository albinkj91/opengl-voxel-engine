#ifndef FLOWER_H
#define FLOWER_H

#include "Voxel.h"

class Flower : public Voxel
{
public:
	Flower(glm::vec3 const& position,
		Program const& program,
		unsigned int const texture);

	void render() const override;
private:
	unsigned int texture;
};

#endif
