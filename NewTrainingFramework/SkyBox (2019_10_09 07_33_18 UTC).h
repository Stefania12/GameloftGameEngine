#pragma once
#include "SceneObject.h"

class SkyBox : public SceneObject
{
public:
	SkyBox(pugi::xml_node object);
	//void Draw(bool wiredformat, float& time) override;
	~SkyBox();
};

