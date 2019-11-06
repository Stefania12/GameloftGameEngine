#include "stdafx.h"
#include "SceneManager.h"
#include "SceneObject.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Fire.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
{
	Init("../Resources/XMLs/sceneManager.xml");
}

void SceneManager::Init(const char* xmlPath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlPath,
		pugi::parse_default | pugi::parse_declaration);
	if (!result)
	{
		std::cout << "Parse error: " << result.description()
			<< ", character pos= " << result.offset;
	}
	pugi::xml_node root = doc.document_element();

	backgroundColor.x = root.child("backgroundColor").child("r").text().as_float();
	backgroundColor.y = root.child("backgroundColor").child("g").text().as_float();
	backgroundColor.z = root.child("backgroundColor").child("b").text().as_float();

	if (root.child("fog"))
	{
		fogColor.x = root.child("fog").child("fogColor").child("r").text().as_float();
		fogColor.y = root.child("fog").child("fogColor").child("g").text().as_float();
		fogColor.z = root.child("fog").child("fogColor").child("b").text().as_float();
		minRadius = root.child("fog").child("minRadius").text().as_float();
		maxRadius = root.child("fog").child("maxRadius").text().as_float();
	}

	Camera* c;
	GLfloat z_near, z_far, fov, trSpeed, rotSpeed;
	GLint cam_id;
	Vector3 pos, target, up;
	pugi::xml_node camera = root.child("cameras").first_child();
	while (camera)
	{
		cam_id = camera.attribute("id").as_int();

		z_near = camera.child("near").text().as_float();
		z_far = camera.child("far").text().as_float();
		fov = camera.child("fov").text().as_float();

		pos.x = camera.child("position").child("x").text().as_float();
		pos.y = camera.child("position").child("y").text().as_float();
		pos.z = camera.child("position").child("z").text().as_float();

		target.x = camera.child("target").child("x").text().as_float();
		target.y = camera.child("target").child("y").text().as_float();
		target.z = camera.child("target").child("z").text().as_float();

		up.x = camera.child("up").child("x").text().as_float();
		up.y = camera.child("up").child("y").text().as_float();
		up.z = camera.child("up").child("z").text().as_float();

		trSpeed = camera.child("translationSpeed").text().as_float();
		rotSpeed = camera.child("rotationSpeed").text().as_float();

		c = new Camera(cam_id, z_near, z_far, fov, float(Globals::screenWidth) / Globals::screenHeight, pos, target, up, trSpeed, rotSpeed);
		cameras.insert(std::pair<int, Camera*>(c->id, c));
		camera = camera.next_sibling();
	}

	activeCamera = root.child("activeCamera").text().as_int();

	pugi::xml_node obj = root.child("objects").child("object");
	SceneObject* object;
	while (obj)
	{
		if (obj.child("type").text().as_string() == "skybox")
			object = new SkyBox(obj);
		else
			if (obj.child("type").text().as_string() == "terrain")
				object = new Terrain(obj);
			else
				if (obj.child("type").text().as_string() == "fire")
					object = new Fire(obj);
				else
					object = new SceneObject(obj);

		objects.insert(std::pair<int, SceneObject*>(object->id, object));
		obj = obj.next_sibling();
	}

	if (root.child("ambientalLight"))
	{
		ambientalLightColor.x = root.child("ambientalLight").child("color").child("r").text().as_float();
		ambientalLightColor.y = root.child("ambientalLight").child("color").child("g").text().as_float();
		ambientalLightColor.z = root.child("ambientalLight").child("color").child("b").text().as_float();

		ambientalLightRatio = root.child("ambientalLight").child("ratio").text().as_float();
	}

	pugi::xml_node light = root.child("lights").child("light");
	Light* l;
	while (light)
	{
		l = new Light(light);
		if (l->type == "directional")
			directionalLights.insert(std::pair<int, Light*>(l->id, l));
		else
			if (l->type == "point")
				pointLights.insert(std::pair<int, Light*>(l->id, l));
			else
				spotlightLights.insert(std::pair<int, Light*>(l->id, l));
		light = light.next_sibling();
	}

}

SceneManager* SceneManager::getInstance()
{
	if (!instance)
		instance = new SceneManager();
	return instance;
}

void SceneManager::Draw(float &time)
{
	for (auto i = objects.begin(); i != objects.end(); i++)
		 i->second->Draw(false, time);
}

void SceneManager::Update()
{
	for (auto i = objects.begin(); i != objects.end(); i++)
		i->second->Update();

	for (auto i = pointLights.begin(); i != pointLights.end(); i++)
		i->second->Update();

	for (auto i = spotlightLights.begin(); i != spotlightLights.end(); i++)
		i->second->Update();

	//std::cout << "croco " << objects[1]->position.x << ' ' << objects[1]->position.y << ' ' << objects[1]->position.z << std::endl;
	//std::cout << "light1 " << pointLights[1]->position.x << ' ' << pointLights[1]->position.y << ' ' << pointLights[1]->position.z << std::endl;

}

void SceneManager::FreeResources()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	for (auto i = cameras.begin(); i != cameras.end(); i++)
		delete i->second;
	for (auto i = 0; i < directionalLights.size(); i++)
		delete directionalLights[i];
	for (auto i = 0; i < pointLights.size(); i++)
		delete pointLights[i];
	for (auto i = 0; i < spotlightLights.size(); i++)
		delete spotlightLights[i];
}

SceneManager::~SceneManager()
{
	FreeResources();
}
