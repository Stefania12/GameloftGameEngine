#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "pugixml.hpp"
#include "../Utilities/utilities.h"
#include <unordered_map>


class ResourceManager
{
private:
	static ResourceManager* instance;
	ResourceManager();

public:
	std::unordered_map<int, ModelResource*> mrMap;
	std::unordered_map<int, ShaderResource*> srMap;
	std::unordered_map<int, TextureResource*> trMap;
	std::unordered_map<int, Model*> modelMap;
	std::unordered_map<int, Shader*> shaderMap;
	std::unordered_map<int, Texture*> textureMap;

	void Init(const char* xmlPath);
	Shader* LoadShader(int id);
	Model *LoadModel(int id);
	Texture* LoadTexture(int id);
	static ResourceManager* getInstance();
	template <class ResourceElement, class Resource>
	Resource* Load(std::unordered_map<int, ResourceElement*>& rElemMap, std::unordered_map<int, Resource*>& rMap, int id)
	{
		auto it = rMap.find(id);
		if (it == rMap.end())
		{
			Resource* r = new Resource(rElemMap[id]);
			rMap[id] = r;
			return r;
		}
		else
		{
			return it->second;
		}
	}
	void freeResources();
	~ResourceManager();
};

