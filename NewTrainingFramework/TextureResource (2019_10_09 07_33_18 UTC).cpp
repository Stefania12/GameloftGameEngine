#include "stdafx.h"
#include "TextureResource.h"


TextureResource::TextureResource(int xmlID, std::string xmlType, std::string xmlPath,
	std::string xmlMin_filter, std::string xmlMag_filter, std::string xmlWrap_s, std::string xmlWrap_t)
{
	id = xmlID;
	type=xmlType;
	path=xmlPath;
	min_filter=xmlMin_filter;
	mag_filter=xmlMag_filter;
	wrap_s=xmlWrap_s;
	wrap_t=xmlWrap_t;
}
