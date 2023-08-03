#ifndef TREE_H
#define TREE_H

#include "Voxel.h"

class Tree : public Voxel
{
public:
	Tree(glm::vec3 const& position,
		Program const& program,
		int const height,
		unsigned int const tx_leaves,
		unsigned int const tx_bark,
		unsigned int const tx_rings);

	void render() const override;
private:
	int height;
	unsigned int texture_leaves;
	unsigned int texture_bark;
	unsigned int texture_rings;

	void draw_trunk() const;
	void draw_leaves(glm::vec3 const& offset) const;
};

#endif
