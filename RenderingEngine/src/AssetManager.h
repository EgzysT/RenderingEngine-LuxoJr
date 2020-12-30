#pragma once

#include <memory>
#include <unordered_map>

#include <json.hpp>

#include "Mesh.h"

class AssetManager
{
using json = nlohmann::json;

public:
	static AssetManager* GetInstance();
private:
	static std::shared_ptr<AssetManager> instance;
	json modelsJSON;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> meshMap;
public:
	AssetManager();
	~AssetManager();
	std::shared_ptr<Mesh> GetMesh(std::string meshID);
	std::string GetModelPathOf(std::string id);
	std::string GetDiffusePathOf(std::string id);
	std::string GetNormalsPathOf(std::string id);
};

