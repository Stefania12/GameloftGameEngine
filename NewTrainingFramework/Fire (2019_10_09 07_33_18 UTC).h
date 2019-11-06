#pragma once
#include "SceneObject.h"
class Fire : public SceneObject
{
public:
	Fire(pugi::xml_node object);
	~Fire();
	//void Draw(bool wiredformat, float& time) override;
};

