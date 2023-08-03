#ifndef TREE_H
#define TREE_H

#include "Voxel.h"

class Tree : public Voxel
{
public:
	Tree(glm::vec3 const& position,
		Program const& program,
		unsigned const int height,
		unsigned const int tx_leaves,
		unsigned const int tx_bark,
		unsigned const int tx_rings);

	void render() const override;
private:
	unsigned int height;
	unsigned int texture_leaves;
	unsigned int texture_bark;
	unsigned int texture_rings;

	void draw_trunk() const;
	void draw_leaves(glm::vec3 const& offset) const;
};

#endif
