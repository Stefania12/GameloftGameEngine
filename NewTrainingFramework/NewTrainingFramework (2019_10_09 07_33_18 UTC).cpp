// NewTrainingFramework.cpp : Defines the entry point for the console application.
//
//#include <algorithm>
//#include <cstdlib>

#include <time.h>
#include <cstddef>
#include <cassert>
#include <conio.h>
#include <iostream>
#include <Windows.h>
#include "stdafx.h"
#include "Vertex.h"
#include "Globals.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "pugixml.hpp"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

bool keys[300];
bool usingCursor = true;

float alpha = 0, colorVariation = 0, deltaColor = 0.1f;
float pas = 0.1f;
float green;
float timeElapsed = 0.0;
POINT cursor= { 1000, 500 };
Matrix mr, vp;

int Init ( ESContext *esContext)
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );

	SceneManager *s = SceneManager::getInstance();

	glEnable(GL_DEPTH_TEST);

	SetCursorPos(cursor.x, cursor.y);
	return 0;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	SceneManager::getInstance()->Draw(timeElapsed);
	
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
	GLenum err= glGetError();
	if (err)
		std::cout << err<< '\n';
}

void Update(ESContext* esContext, float deltaTime)
{
	timeElapsed += deltaTime;
	if (timeElapsed >= 1.0)
		timeElapsed -= 1.0;
	Camera* activeCamera = SceneManager::getInstance()->cameras[SceneManager::getInstance()->activeCamera];
	static int nr = 0;
	activeCamera->deltatime = deltaTime;
	
	

	POINT newcursor;
	float difx, dify;
	GetCursorPos(&newcursor);
	difx = newcursor.x - cursor.x;
	dify = newcursor.y - cursor.y;
	

	if (usingCursor)
	{	
		activeCamera->rotateOy(difx);
		activeCamera->rotateOx(dify);
		SetCursorPos(cursor.x, cursor.y);
	}

	if (GetAsyncKeyState(VK_LBUTTON)) {
		
		if (newcursor.x < Globals::screenWidth/2) {
			alpha += pas * 10 * deltaTime;
			if (alpha > 2 * pi)
				alpha -= 2 * pi;
			mr.SetRotationY(alpha);
		}
		else
		{
			if (newcursor.x > Globals::screenWidth / 2) {
				alpha -= pas * 10 * deltaTime;
				if (alpha < -2 * pi)
					alpha += 2 * pi;
				mr.SetRotationY(alpha);
			}
		}
	}

	if (GetAsyncKeyState(VK_LSHIFT))
		activeCamera->currentSpeed = 200;
	else
		activeCamera->currentSpeed = activeCamera->moveSpeed;

	if (keys['Q'] == 1)
		activeCamera->rotateOz(1);

	if (keys['E'] == 1)
		activeCamera->rotateOz(-1);

	if (keys['W'] == 1)
		activeCamera->moveOz(1);

	if (keys['S'] == 1)
		activeCamera->moveOz(-1);

	if (keys['D'] == 1)
		activeCamera->moveOx(1);

	if (keys['A'] == 1)
		activeCamera->moveOx(-1);

	if (keys[' '] == 1)
		activeCamera->moveOy(1);

	if (keys['V'] == 1)
		activeCamera->moveOy(-1);

	SceneManager::getInstance()->Update();
	
	std::cout << activeCamera->position.x << ' ' << activeCamera->position.y << ' ' << activeCamera->position.z << std::endl;
	static float time = 0;
	static int fps = 0;
	time += deltaTime;
	fps++;

	if (time >= 1) {
		time--;
		std::cout << fps << std::endl;
		fps = 0;
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	keys[key] = bIsPressed;
	if (key == 'C' && bIsPressed) {
		usingCursor = !usingCursor;
		if (usingCursor)
			SetCursorPos(cursor.x, cursor.y);
	}
	if (key == 'P' && bIsPressed)
		exit(1);
}

void CleanUp()
{
	delete SceneManager::getInstance();
	delete ResourceManager::getInstance();
}


int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
	Globals::loadScreen("../Resources/XMLs/Globals.xml");
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Project name", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	//getch();

	
	return 0;
}

