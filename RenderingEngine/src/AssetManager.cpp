#include "AssetManager.h"

#include <iostream>
#include <fstream>

#define MODELS_JSON_PATH "..\\assets\\models\\models.json"
#define MODELS_FOLDER "..\\assets\\models\\"

std::shared_ptr<AssetManager> AssetManager::instance;

AssetManager* AssetManager::GetInstance()
{
    if (!instance)
    {
        instance = std::make_shared<AssetManager>();
    }
    return instance.get();
}

AssetManager::AssetManager()
{
    std::ifstream modelsJsonFile;
    modelsJsonFile.open(MODELS_JSON_PATH);
    if (!modelsJsonFile.is_open()) {
        std::cout << "ERROR" << std::endl;
    }
    modelsJsonFile >> modelsJSON;
    modelsJsonFile.close();
}

AssetManager::~AssetManager()
{
}

std::shared_ptr<Mesh> AssetManager::GetMesh(std::string meshID)
{
    auto meshCache = meshMap.find(meshID);
    if (meshCache != meshMap.end()) //found in cache
        return meshCache->second;

    std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>();     // not found, create new
    newMesh->LoadMesh(meshID);
    
    meshMap[meshID] = newMesh;     // store new
    return newMesh;
}

std::string AssetManager::GetModelPathOf(std::string id)
{
    std::string modelPath = modelsJSON["models"][id.c_str()]["meshPath"];
    return MODELS_FOLDER + modelPath;
}

std::string AssetManager::GetDiffusePathOf(std::string id)
{
    std::string diffusePath = modelsJSON["models"][id.c_str()]["diffusePath"];
    return MODELS_FOLDER + diffusePath;
}

std::string AssetManager::GetNormalsPathOf(std::string id)
{
    std::string normalsPath = modelsJSON["models"][id.c_str()]["normalsPath"];
    return MODELS_FOLDER + normalsPath;
}
