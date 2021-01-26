#include "Scene.h"

#include <iostream>
#include <fstream>

#include <GLM/gtc/random.hpp>

Scene::Scene(std::string filepath)
{
    std::ifstream sceneFile;
    sceneFile.open(filepath);
    if (!sceneFile.is_open()) {
        std::cout << "ERROR: failed to open scene json file at " << filepath << std::endl;
    }
    sceneFile >> sceneJSON;
    sceneFile.close();
}

std::vector<RenderItem> Scene::CreateRenderItems(Graphics* graphics)
{
    //std::vector<RenderItem> renderItems;
    //auto scene = sceneJSON["scene"];
    //for (json::iterator it = scene.begin(); it != scene.end(); ++it) {
    //    std::string modelID = it->at("model");
    //    glm::vec3 pos (it->at("pos")["x"], it->at("pos")["y"], it->at("pos")["z"]);
    //    glm::vec3 rot(it->at("rot")["x"], it->at("rot")["y"], it->at("rot")["z"]);
    //    glm::vec3 sca(it->at("scale")["x"], it->at("scale")["y"], it->at("scale")["z"]);
    //    //Mesh mesh = Mesh{};
    //    //mesh.LoadMesh(modelID);
    //    RenderItem renderItem{ graphics, modelID };
    //    renderItem.SetPosition(pos.x, pos.y, pos.z);
    //    renderItem.SetRotation(rot.x, rot.y, rot.z);
    //    renderItem.SetScale(sca.x, sca.y, sca.z);
    //    renderItems.push_back(renderItem);
    //}
    //return renderItems;

    std::vector<RenderItem> renderItems;
    int numAsteroids = 50000;
    double radius = 500;
    for (int i = 0; i < numAsteroids; i++) {
        glm::vec3 pos = glm::ballRand(radius);
        glm::vec3 rot(glm::ballRand(179.9));
        glm::vec3 sca(0.2, 0.2, 0.2);
        RenderItem renderItem{ graphics, "rockBrown" };
        renderItem.SetPosition(pos.x, pos.y, pos.z);
        //renderItem.SetRotation(rot.x, rot.y, rot.z);
        renderItem.SetScale(sca.x, sca.y, sca.z);
        renderItem.CalculateBoundingBox();
        renderItems.push_back(renderItem);
    }
    return renderItems;
}
