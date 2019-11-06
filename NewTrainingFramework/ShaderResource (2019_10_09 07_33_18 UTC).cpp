#include "stdafx.h"
#include "ShaderResource.h"


ShaderResource::ShaderResource(int xmlID, std::string& xmlVS, std::string& xmlFS)
{
	id = xmlID;
	vs = xmlVS;
	fs = xmlFS;
}

