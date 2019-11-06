#pragma once
#include "stdafx.h"
#include "Trajectory.h"

Trajectory::Trajectory(pugi::xml_node trj)
{
	type = trj.attribute("type").as_string();
	iteration = trj.attribute("iteration").as_string();

	if (type == "line_loop" || type == "circle")
	{
		closed = true;
		direction = "normal";
	}
	else
		direction = trj.attribute("direction").as_string();

	if (trj.child("center"))
	{
		center.x = trj.child("center").child("x").text().as_float();
		center.y = trj.child("center").child("y").text().as_float();
		center.z = trj.child("center").child("z").text().as_float();

		centerTranslationMatrix.SetTranslation(center);
	}

	if (trj.child("radius"))
		radius = trj.child("radius").text().as_float();

	if (trj.child("planeRotation"))
	{
		Matrix rotationXMatrix, rotationZMatrix;
		rotationXMatrix.SetRotationX(trj.child("planeRotation").child("x").text().as_float());
		rotationZMatrix.SetRotationZ(trj.child("planeRotation").child("z").text().as_float());

		planeRotationMatrix = rotationXMatrix * rotationZMatrix;
	}
	speed = trj.child("speed").text().as_float();

	up.x = trj.child("up").child("x").text().as_float();
	up.y = trj.child("up").child("y").text().as_float();
	up.z = trj.child("up").child("z").text().as_float();
}

Trajectory::~Trajectory()
{
	TrajectoryNode* p = start, *q;
	while (p != end)
	{
		q = p->next;
		delete p;
		p = q;
	}
	delete end;
	start = nullptr;
	end = nullptr;
}

void Trajectory::Insert(Vector3 pos)
{
	TrajectoryNode* p = new TrajectoryNode;
	p->position = pos;

	if (!start)
	{
		start = p;
		end = p;
		initial = p;
		if (!closed)
		{
			p->next = nullptr;
			p->prev = nullptr;
		}
		else
		{
			p->next = p;
			p->prev = p;
		}
	}
	else
	{
		end->next = p;
		p->prev = end;
		end = p;
		if (closed)
		{
			p->next = start;
			start->prev = p;
		}
		else
			p->next = nullptr;
	}	
}

TrajectoryNode* Trajectory::Next()
{
	if (initial == nullptr)
		return nullptr;
	if (direction == "normal")
		return initial->next;
	if (iteration_count % 2 == 0)
		return initial->next;
	return initial->prev;
}

void Trajectory::NextInitialPosition()
{
	TrajectoryNode* oldInitial = initial;
	if (initial)
	{
		if (Next())
		{
			if (iteration == "infinite" || iteration_count < stoi(iteration))
			{
				initial = Next();
				if (initial == start)
					iteration_count++;
			}
			else
				initial = nullptr;
		}
		else
		{
			iteration_count++;
			if (iteration == "infinite" || iteration_count < stoi(iteration))
			{
				if (direction == "normal")
					initial = start;
				else
					initial = Next();
			}
			else
				initial = nullptr;
		}
		if (initial)
		{
			movementDirection = (initial->position - oldInitial->position).Normalize();
			forward = movementDirection;
			setR();
		}
	}
}

void Trajectory::setR()
{
	zAxis = -forward.Normalize();
	yAxis = up.Normalize();
	xAxis = -zAxis.Cross(yAxis).Normalize();

	R[0][0] = xAxis.x; R[0][1] = xAxis.y; R[0][2] = xAxis.z; R[0][3] = 0;
	R[1][0] = yAxis.x; R[1][1] = yAxis.y; R[1][2] = yAxis.z; R[1][3] = 0;
	R[2][0] = zAxis.x; R[2][1] = zAxis.y; R[2][2] = zAxis.z; R[2][3] = 0;
	R[3][0] = 0;       R[3][1] = 0;       R[3][2] = 0;       R[3][3] = 1;
}
