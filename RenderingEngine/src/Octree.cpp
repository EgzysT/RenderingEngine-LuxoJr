#include "Octree.h"

#include "GLM/glm.hpp"

void CalculateBounds(BoundingBox* out, Octant octant, BoundingBox parentRegion)
{
    glm::vec3 center = parentRegion.calculateCenter();
    if (octant == Octant::O1) {
        out = new BoundingBox(center, parentRegion.max);
    }
    else if (octant == Octant::O2) {
        out = new BoundingBox(glm::vec3(parentRegion.min.x, center.y, center.z), glm::vec3(center.x, parentRegion.max.y, parentRegion.max.z));
    }
    else if (octant == Octant::O3) {
        out = new BoundingBox(glm::vec3(parentRegion.min.x, parentRegion.min.y, center.z), glm::vec3(center.x, center.y, parentRegion.max.z));
    }
    else if (octant == Octant::O4) {
        out = new BoundingBox(glm::vec3(center.x, parentRegion.min.y, center.z), glm::vec3(parentRegion.max.x, center.y, parentRegion.max.z));
    }
    else if (octant == Octant::O5) {
        out = new BoundingBox(glm::vec3(center.x, center.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, parentRegion.max.y, center.z));
    }
    else if (octant == Octant::O6) {
        out = new BoundingBox(glm::vec3(parentRegion.min.x, center.y, parentRegion.min.z), glm::vec3(center.x, parentRegion.max.y, center.z));
    }
    else if (octant == Octant::O7) {
        out = new BoundingBox(parentRegion.min, center);
    }
    else if (octant == Octant::O8) {
        out = new BoundingBox(glm::vec3(center.x, parentRegion.min.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, center.y, center.z));
    }
}

Node::Node(BoundingBox bounds, std::vector<RenderItem> objectList)
    : region(bounds)
{
    objects.insert(objects.end(), objectList.begin(), objectList.end());
}

void Node::build()
{
    // Terminations Checks
    if (objects.size() <= 1) return;
    glm::vec3 dimension = region.calculateDimensions();
    for (int i = 0; i < 3; i++)
    {
        if (dimension[i] < MIN_BOUNDS) return;
    }

    //create regions
    BoundingBox octants[NUM_CHILDREN];
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        CalculateBounds(&octants[i], (Octant)(1 << i), region);
    }

    // determine which octants to place objects in
    std::vector<RenderItem> octLists[NUM_CHILDREN];
    std::stack<int> delList;

    for (int i = 0, length = objects.size(); i < length; i++) {
        RenderItem renderItem = objects[i];
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            if (octants[j].containsRegion(renderItem.aabb));
            octLists[j].push_back(renderItem);
            delList.push(i);
            break;
        }
    }

    //remove objects on delList
    while (objects.size() != 0)
    {
        objects.erase(objects.begin() + delList.top());
        delList.pop();
    }

    //populate octants
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if (octLists[i].size() != 0) {
            children[i] = new Node(octants[i], octLists[i]);
            children[i]->parent = this;
            children[i]->build();
            hasChildren = true;
            activeOctants |= (unsigned char)(1 << i);
        }
    }

    treeBuilt = true;
    treeReady = true;
}

void Node::destroy()
{
    if (children != nullptr) {
        for (int i = 0; i < NUM_CHILDREN; i++)
        {
            if (children[i] != nullptr) {
                children[i]->destroy();
                children[i] = nullptr;
            }
        }
    }

    objects.clear();
    /*while ()
    {

    }*/
}
