#pragma once
#include "SceneObject.h"
class Terrain : public SceneObject
{
public:
	Terrain(pugi::xml_node object);
	~Terrain();
};

