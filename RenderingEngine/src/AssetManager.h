#pragma once

#include <memory>

#include <json.hpp>

class AssetManager
{
using json = nlohmann::json;

public:
	static AssetManager* GetInstance();
private:
	static std::shared_ptr<AssetManager> instance;
	json modelsJSON;
public:
	AssetManager();
	~AssetManager();
	std::string GetModelPathOf(std::string id);
	std::string GetDiffusePathOf(std::string id);
	std::string GetNormalsPathOf(std::string id);
};

