#include "Octree.h"
#include "RenderItem.h"

#include "GLM/glm.hpp"

#include <iostream>
#include "GLM/gtx/string_cast.hpp"

void CalculateBounds(BoundingBox &out, Octant octant, BoundingBox parentRegion)
{
    glm::vec3 center = parentRegion.calculateCenter();
    if (octant == Octant::O1) {
        out = BoundingBox(center, parentRegion.max, glm::vec3(0.3, 0.7, 0.0));
    }
    else if (octant == Octant::O2) {
        out = BoundingBox(glm::vec3(parentRegion.min.x, center.y, center.z), glm::vec3(center.x, parentRegion.max.y, parentRegion.max.z), glm::vec3(0.3, 0.7, 0.0));
    }
    else if (octant == Octant::O3) {
        out = BoundingBox(glm::vec3(parentRegion.min.x, parentRegion.min.y, center.z), glm::vec3(center.x, center.y, parentRegion.max.z), glm::vec3(0.3, 0.7, 0.0));
    }
    else if (octant == Octant::O4) {
        out = BoundingBox(glm::vec3(center.x, parentRegion.min.y, center.z), glm::vec3(parentRegion.max.x, center.y, parentRegion.max.z), glm::vec3(0.3, 0.7, 0.0));
    }
    else if (octant == Octant::O5) {
        out = BoundingBox(glm::vec3(center.x, center.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, parentRegion.max.y, center.z), glm::vec3(0.3, 0.7, 0.0));
    }
    else if (octant == Octant::O6) {
        out = BoundingBox(glm::vec3(parentRegion.min.x, center.y, parentRegion.min.z), glm::vec3(center.x, parentRegion.max.y, center.z), glm::vec3(0.3, 0.7, 0.0));
    }
    else if (octant == Octant::O7) {
        out = BoundingBox(parentRegion.min, center, glm::vec3(0.3, 0.7, 0.0));
    }
    else if (octant == Octant::O8) {
        out = BoundingBox(glm::vec3(center.x, parentRegion.min.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, center.y, center.z), glm::vec3(0.3, 0.7, 0.0));
    }
}

Node::Node(BoundingBox bounds, std::vector<RenderItem> objectList)
    : region(bounds)
{
    objects.insert(objects.end(), objectList.begin(), objectList.end());
}

void Node::Build()
{
    //DEBUG
    BoundingBox big(glm::vec3(-100, -100, -100), glm::vec3(100, 100, 100), glm::vec3(1, 1, 1));
    BoundingBox small(glm::vec3(-200.0, -200.0, -200.0), glm::vec3(-93.0, -93.0, -93.0), glm::vec3(1, 1, 1));
    assert(!big.containsRegion(small));

    BoundingBox big2(glm::vec3(-100, -100, -100), glm::vec3(100, 100, 100), glm::vec3(1, 1, 1));
    BoundingBox small2(glm::vec3(-100.0, -100.0, -100.0), glm::vec3(-93.0, -93.0, -93.0), glm::vec3(1, 1, 1));
    assert(big2.containsRegion(small2));

    // Termination Checks
    if (objects.size() <= LEAF_CHILDREN) return;
    glm::vec3 dimension = region.calculateDimensions();
    for (int i = 0; i < 3; i++)
    {
        if (dimension[i] < MIN_BOUNDS) return;
    }

    //create regions
    BoundingBox octants[NUM_CHILDREN];
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        CalculateBounds(octants[i], (Octant)(1 << i), region);
    }

    // determine which octants to place objects in
    std::vector<RenderItem> octLists[NUM_CHILDREN];
    std::stack<int> delList;

    for (int i = 0, length = objects.size(); i < length; i++) {
        RenderItem renderItem = objects[i];
        for (int j = 0; j < NUM_CHILDREN; j++)
        {
            if (octants[j].containsRegion(renderItem.aabb)) {
                octLists[j].push_back(renderItem);
                delList.push(i);
                break;
            }
        }
        // TODO ELSE should stay here?
    }

    //remove objects on delList
    while (delList.size() != 0)
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
            children[i]->Build();
            //std::cout << "i:" << i 
            //    << "  min:" << glm::to_string(children[i]->region.min) 
            //    << "  max:" << glm::to_string(children[i]->region.max) << std::endl;
            hasChildren = true;
            activeOctants |= (unsigned char)(1 << i);
        }
    }

    treeBuilt = true;
    treeReady = true;
}

void Node::Render(Frustum frustum)
{
    if (frustum.IsBoxVisible(region.min, region.max)) {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i].Render(false);
        }
        for (int i = 0; i < NUM_CHILDREN; i++)
        {
            if (children[i] != nullptr) {
                children[i]->Render(frustum);
            }
        }
    }
}

void Node::RenderRegion()
{
    region.Render();
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if (children[i] != nullptr) {
            children[i]->RenderRegion();
        }
    }
}

void Node::Destroy()
{
    if (children != nullptr) {
        for (int i = 0; i < NUM_CHILDREN; i++)
        {
            if (children[i] != nullptr) {
                children[i]->Destroy();
                children[i] = nullptr;
            }
        }
    }

    objects.clear();
}
