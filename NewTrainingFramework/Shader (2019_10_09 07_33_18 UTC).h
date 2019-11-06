#pragma once
#include "../Utilities/math.h"
#include "../Utilities/esUtil.h"
#include "stdafx.h"
#include "ShaderResource.h"
#define maxLights 5

struct light
{
	GLint diffuseColor, specularColor, direction, position, smallLimit, bigLimit, Kc, Kl, Kq;
};

class Shader
{
public:
	Shader(ShaderResource* xmlsr);
	~Shader();
	void Load();

public:
	ShaderResource* sr=0;
	int program=0, VertexShader = 0, FragmentShader = 0;
	
	GLint positionAttribute = -1;
	GLint uvAttribute = -1;
	GLint mini_uvAttribute = -1;
	GLint normAttribute = -1;

	GLint colorUniform = -1;
	GLint mvpMatrixUniform = -1;
	GLint modelMatrixUniform = -1;
	GLint textureUniform[7] = { -1 };
	GLint translationUniform = -1;
	GLint heightUniform = -1;
	GLint timeUniform = -1;
	GLint dispMaxUniform = -1;
	GLint fogColorUniform = -1;
	GLint minRadiusUniform = -1;
	GLint maxRadiusUniform = -1;
	GLint cameraPositionUniform = -1;
	GLint ambientalColorUniform = -1;
	GLint ambientalRatioUniform = -1;
	GLint specPowerUniform = -1;
	light directionalLights[maxLights] = {{-1}};
	light pointLights[maxLights] = { {-1} };
	light spotlightLights[maxLights] = { {-1} };
};

