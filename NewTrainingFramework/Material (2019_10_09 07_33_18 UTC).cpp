#include "stdafx.h"
#include "Material.h"


Material::Material(Shader *s)
{
	shader = s;
}

void Material::AddTexture(Texture* t)
{
	textures.push_back(t);
}
Material::~Material()
{
}
