#pragma once
#include <string>
#include "../Utilities/Math.h"
#include <vector>
#include "Material.h"
#include "pugixml.hpp"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Trajectory.h"
class SceneObject
{
public:
	SceneObject(pugi::xml_node object);
	virtual ~SceneObject();
	virtual void Draw(bool wiredformat, float &time);
	void DrawNormal(const Camera* c,  Matrix& modelMatrix);
	void DrawFire(const Camera* c,  Matrix& modelMatrix, float& time);
	void DrawTerrain();
	void AddShaderBasics(const Camera* c, Matrix& modelMatrix);
	void AddLights(const Camera* c);
	void AddFog();
	void Update();

	int id = 0, depthTest = 0, size = 0;
	float speed = 0.0, dispMax = 0.0, specPower = 0.0;
	bool blend, wired, col, texture;
	std::string type, name;
	std::vector<int> camera_ids, light_ids;
	Trajectory* trajectory = nullptr;
	Vector3 color, position, rotation, scale, height, followingCamera = Vector3(0.0, 0.0, 0.0), forward = Vector3(0.0,0.0,1.0);
	Material* material  = nullptr;
	Model* model = nullptr;
	Matrix scaleMatrix, rotationMatrix, modelMatrix;
};

