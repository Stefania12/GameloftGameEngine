#pragma once
#include <iostream>
#include <string>
class TextureResource
{
public:
	int id=0;
	std::string type, path;
	std::string min_filter, mag_filter;
	std::string wrap_s, wrap_t;
	TextureResource(){}
	TextureResource(int xmlID, std::string xmlType, std::string xmlPath,
					std::string xmlMin_filter, std::string xmlMag_filter,
					std::string xmlWrap_s, std::string xmlWrap_t);
};

