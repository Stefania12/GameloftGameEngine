#pragma once
#include <string>
#include <iostream>
#include <stdlib.h>

class ModelResource
{
public:
	int id;
	std::string path;
	ModelResource(int xmlID, std::string& xmlPath);
	~ModelResource();
};

