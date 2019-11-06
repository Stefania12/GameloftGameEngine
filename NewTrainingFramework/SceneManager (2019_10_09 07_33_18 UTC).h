#pragma once
#include "Camera.h"
#include <vector>
#include <iostream>
#include "pugixml.hpp"
#include "../Utilities/Math.h"
#include "Globals.h"
#include <unordered_map>
#include "Light.h"

class SceneObject;


class SceneManager
{
private:
	static SceneManager* instance;
	SceneManager();
public:
	~SceneManager();
	static SceneManager* getInstance();
	void Init(const char* xmlPath);
	void Draw(float &time);
	void FreeResources();
	void Update();

	std::unordered_map<int, Camera*> cameras;
	std::unordered_map<int, SceneObject*> objects;
	std::unordered_map<int, Light*> directionalLights, pointLights, spotlightLights;
	int activeCamera = 0;
	Vector3 backgroundColor, fogColor, ambientalLightColor;
	float minRadius = 0.0, maxRadius = 0.0, ambientalLightRatio = 0.0;
};

