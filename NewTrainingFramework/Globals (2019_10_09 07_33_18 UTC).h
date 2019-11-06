#pragma once
#include<iostream>
#include<pugixml.hpp>

class Globals
{
public:
	static int screenWidth;
	static int screenHeight;

	static void loadScreen(char* xmlpath);
};