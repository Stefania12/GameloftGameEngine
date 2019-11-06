#pragma once
#include <iostream>
#include "../Utilities/Math.h"
#include "pugixml.hpp"
class SceneObject;

class Light
{
public:
	Light(pugi::xml_node light);
	Light();
	~Light();
	void Update();

	GLint id = -1, associatedObject = -1;
	GLfloat specPower, smallLimit, bigLimit, Kc, Kl, Kq;
	Vector3 direction, position, diffuseColor, specularColor;
	std::string type;
};

