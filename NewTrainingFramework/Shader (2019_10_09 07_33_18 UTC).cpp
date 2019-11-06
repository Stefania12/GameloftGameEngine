#include "stdafx.h"
#include "Shader.h"
#include <cassert>


Shader::Shader(ShaderResource* xmlsr) :sr(xmlsr)
{
	Load();
}


void Shader::Load()
{
	VertexShader = esLoadShader(GL_VERTEX_SHADER, sr->vs.c_str());
	FragmentShader = esLoadShader(GL_FRAGMENT_SHADER, sr->fs.c_str());
	program = esLoadProgram(VertexShader, FragmentShader);

	if (!program)
		assert(false);

	glUseProgram(program);
	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	colorUniform = glGetUniformLocation(program, "u_color");
	uvAttribute = glGetAttribLocation(program, "a_uv");
	mini_uvAttribute = glGetAttribLocation(program, "a_mini_uv");
	normAttribute = glGetAttribLocation(program, "a_norm");

	translationUniform = glGetUniformLocation(program, "u_translation");
	mvpMatrixUniform = glGetUniformLocation(program, "u_mvpMatrix");
	modelMatrixUniform = glGetUniformLocation(program, "u_modelMatrix");
	heightUniform = glGetUniformLocation(program, "u_height");
	timeUniform = glGetUniformLocation(program, "u_time");
	dispMaxUniform = glGetUniformLocation(program, "u_dispMax");
	fogColorUniform = glGetUniformLocation(program, "u_fogColor");
	minRadiusUniform = glGetUniformLocation(program, "u_minRadius");
	maxRadiusUniform = glGetUniformLocation(program, "u_maxRadius");
	cameraPositionUniform = glGetUniformLocation(program, "u_camPos");
	ambientalColorUniform = glGetUniformLocation(program, "u_ambientalColor");
	ambientalRatioUniform = glGetUniformLocation(program, "u_ambRatio");
	specPowerUniform = glGetUniformLocation(program, "u_specPower");

	for (int i = 0; i < 7; i++)
		textureUniform[i] = glGetUniformLocation(program, ("u_texture" + std::to_string(i)).c_str());

	//directional
	for (int i = 0; i < maxLights; i++)
	{
		directionalLights[i].diffuseColor = glGetUniformLocation(program, ("u_directionalLights[" + std::to_string(i) + "].diffuseColor").c_str());
		directionalLights[i].specularColor = glGetUniformLocation(program, ("u_directionalLights[" + std::to_string(i) + "].specularColor").c_str());
		directionalLights[i].direction = glGetUniformLocation(program, ("u_directionalLights[" + std::to_string(i) + "].direction").c_str());
	}

	//point
	for (int i = 0; i < maxLights; i++)
	{
		pointLights[i].diffuseColor = glGetUniformLocation(program, ("u_pointLights[" + std::to_string(i) + "].diffuseColor").c_str());
		pointLights[i].specularColor = glGetUniformLocation(program, ("u_pointLights[" + std::to_string(i) + "].specularColor").c_str());
		pointLights[i].position = glGetUniformLocation(program, ("u_pointLights[" + std::to_string(i) + "].position").c_str());
		pointLights[i].Kc = glGetUniformLocation(program, ("u_pointLights[" + std::to_string(i) + "].Kc").c_str());
		pointLights[i].Kl = glGetUniformLocation(program, ("u_pointLights[" + std::to_string(i) + "].Kl").c_str());
		pointLights[i].Kq = glGetUniformLocation(program, ("u_pointLights[" + std::to_string(i) + "].Kq").c_str());
	}

	//spotlight
	for (int i = 0; i < maxLights; i++)
	{
		spotlightLights[i].diffuseColor = glGetUniformLocation(program, ("u_spotlightLights[" + std::to_string(i) + "].diffuseColor").c_str());
		spotlightLights[i].specularColor = glGetUniformLocation(program, ("u_spotlightLights[" + std::to_string(i) + "].specularColor").c_str());
		spotlightLights[i].position = glGetUniformLocation(program, ("u_spotlightLights[" + std::to_string(i) + "].position").c_str());
		spotlightLights[i].direction = glGetUniformLocation(program, ("u_spotlightLights[" + std::to_string(i) + "].direction").c_str());
		spotlightLights[i].smallLimit = glGetUniformLocation(program, ("u_spotlightLights[" + std::to_string(i) + "].smallLimit").c_str());
		spotlightLights[i].bigLimit = glGetUniformLocation(program, ("u_spotlightLights[" + std::to_string(i) + "].bigLimit").c_str());
		spotlightLights[i].Kc = glGetUniformLocation(program, ("u_spotlightLights[" + std::to_string(i) + "].Kc").c_str());
		spotlightLights[i].Kl = glGetUniformLocation(program, ("u_spotlightLights[" + std::to_string(i) + "].Kl").c_str());
		spotlightLights[i].Kq = glGetUniformLocation(program, ("u_spotlightLights[" + std::to_string(i) + "].Kq").c_str());
	}
}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}
