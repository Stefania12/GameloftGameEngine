#pragma once
#include <iostream>
#include "stdafx.h"
#include "../Utilities/math.h"
#include "../Utilities/esUtil.h"
#include "../Utilities/TGA.h"
#include <cassert>
#include "TextureResource.h"

class Texture
{
public:
	Texture(TextureResource* t);
	~Texture();
	void Bind(int slot = 0);
	void Load();

	GLuint texture_id=0;
	TextureResource* tr = 0;
	
};
