#include "stdafx.h"
#include "ModelResource.h"


ModelResource::ModelResource(int xmlID, std::string& xmlPath)
{
	id = xmlID;
	path=xmlPath;
}


ModelResource::~ModelResource()
{
}
