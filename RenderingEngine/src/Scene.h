#pragma once

#include <string>
#include <vector>

#include <json.hpp>

#include "RenderItem.h"
#include "Graphics.h"

class Scene
{
	using json = nlohmann::json;

private:
	json sceneJSON;
public:
	Scene(std::string filepath);
	std::vector<RenderItem> CreateRenderItems(Graphics* graphics);
};

