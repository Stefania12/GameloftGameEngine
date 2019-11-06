#include "stdafx.h"
#include "Light.h"
#include "SceneManager.h"
#include "SceneObject.h"

Light::Light(pugi::xml_node light)
{
	id = light.attribute("id").as_int();

	specPower = light.child("specPower").text().as_float();

	if (light.child("direction"))
	{
		direction.x = light.child("direction").child("x").text().as_float();
		direction.y = light.child("direction").child("y").text().as_float();
		direction.z = light.child("direction").child("z").text().as_float();
	}
	if (light.child("position"))
	{
		position.x = light.child("position").child("x").text().as_float();
		position.y = light.child("position").child("y").text().as_float();
		position.z = light.child("position").child("z").text().as_float();
	}

	diffuseColor.x = light.child("diffuseColor").child("r").text().as_float();
	diffuseColor.y = light.child("diffuseColor").child("g").text().as_float();
	diffuseColor.z = light.child("diffuseColor").child("b").text().as_float();

	specularColor.x = light.child("specularColor").child("r").text().as_float();
	specularColor.y = light.child("specularColor").child("g").text().as_float();
	specularColor.z = light.child("specularColor").child("b").text().as_float();
		if (light.child("limits"))
	{
		smallLimit = light.child("limits").child("smallLimit").text().as_float();
		bigLimit = light.child("limits").child("bigLimit").text().as_float();
	}

	if (light.child("constants"))
	{
		Kc = light.child("constants").child("Kc").text().as_float();
		Kl = light.child("constants").child("Kl").text().as_float();
		Kq = light.child("constants").child("Kq").text().as_float();
	}

	
	if (light.child("associatedObject"))
		associatedObject = light.child("associatedObject").text().as_int();
	else
		associatedObject = -1;

	type = light.child("type").text().as_string();
}

Light::Light()
{
}

void Light::Update()
{
	if (associatedObject != -1)
		position = SceneManager::getInstance()->objects[associatedObject]->position;
}

Light::~Light()
{
}
