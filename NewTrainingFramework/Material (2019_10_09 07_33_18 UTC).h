#pragma once

#include "Shader.h"
#include "Texture.h"
#include "../Utilities/Math.h"
#include <vector>
class Material
{
public:
	Material(Shader *s);
	~Material();
	void AddTexture(Texture* t);

	Shader* shader;
	std::vector<Texture*> textures;
	Vector3 color;
	
};

