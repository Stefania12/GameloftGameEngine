#pragma once
#include "../Utilities/Math.h"
#include <stdlib.h>
#include <string>
#include "pugixml.hpp"

struct TrajectoryNode {
	Vector3 position;
	TrajectoryNode* next = nullptr, * prev = nullptr;
};

class Trajectory
{
public:
	Trajectory(pugi::xml_node trj);
	~Trajectory();
	void Insert(Vector3 pos);
	void NextInitialPosition();
	//void Update();
	//void rotateOx();
	//void rotateOy();
	void setR();
	TrajectoryNode* Next();

	TrajectoryNode* start = nullptr, * end = nullptr, *initial = nullptr;
	bool closed = false;
	std::string type, iteration, direction;
	int iteration_count = 0, radius = 0.0;
	float distance = 0.0, speed;
	Vector3 xAxis, yAxis, zAxis;
	Vector3 movementDirection, forward = { 0.0,0.0,-1.0 }, right = { 1.0, 0.0, 0.0 }, up = { 0.0,1.0,0.0 }, center = { 0.0,0.0,0.0 }, rotations = { 0.0, 0.0, 0.0 };
	Matrix planeRotationMatrix, centerTranslationMatrix, R;
};


