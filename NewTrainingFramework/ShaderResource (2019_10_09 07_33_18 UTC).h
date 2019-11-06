#pragma once
#include <string>
#include <iostream>
class ShaderResource
{
public:
	int id;
	std::string vs, fs;
	ShaderResource(int xmlID, std::string& xmlVS, std::string& xmlFS);
};
