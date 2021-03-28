#pragma once

#include <vector>
#include <stack>

//#include "RenderItem.h"
#include "BoundingBox.h"
#include "FrustrumCull.h"

#define NUM_CHILDREN 8
#define MIN_BOUNDS 64.0

class RenderItem;
enum class Octant : unsigned char {
	O1 = 0x01,	// 0b00000001
	O2 = 0x02,	// 0b00000010
	O3 = 0x04,	// 0b00000100
	O4 = 0x08,	// ...
	O5 = 0x10,
	O6 = 0x20,
	O7 = 0x40,
	O8 = 0x80	// 0b10000000
};

// Utility method to calculate the bounds of a child node
void CalculateBounds(BoundingBox &out, Octant octant, BoundingBox parentRegion);

class Node {
public:
	Node* parent;
	Node* children[NUM_CHILDREN];
	unsigned char activeOctants = 0;
	bool hasChildren = false;
	bool treeReady = false;
	bool treeBuilt = false;

	std::vector<RenderItem> objects;
	BoundingBox region;

	//Node();
	//Node(BoundingBox bounds);
	Node(BoundingBox bounds, std::vector<RenderItem> objectList);

	void Build();
	void Render(Frustum frustum);
	void RenderRegion();
	void Destroy();
};

class Octree
{
	
};

