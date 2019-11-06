#include "stdafx.h"
#include "SceneObject.h"


SceneObject::SceneObject(pugi::xml_node object)
{
	type = object.child("type").text().as_string("normal");
	if (type == "skybox")
	{
		position.y = object.child("position").child("y").text().as_float();
	}
	else
	{
		position.x = object.child("position").child("x").text().as_float();
		position.y = object.child("position").child("y").text().as_float();
		position.z = object.child("position").child("z").text().as_float();
	}

	if (object.child("specPower"))
		specPower = object.child("specPower").text().as_float();

	rotation.x = object.child("rotation").child("x").text().as_float();
	rotation.y = object.child("rotation").child("y").text().as_float();
	rotation.z = object.child("rotation").child("z").text().as_float();

	Matrix rotationXMatrix, rotationYMatrix, rotationZMatrix;
	rotationMatrix = rotationXMatrix.SetRotationX(rotation.x) * rotationYMatrix.SetRotationY(rotation.y) * rotationZMatrix.SetRotationZ(rotation.z);
	

	scale.x = object.child("scale").child("x").text().as_float();
	scale.y = object.child("scale").child("y").text().as_float();
	scale.z = object.child("scale").child("z").text().as_float();
	scaleMatrix = scaleMatrix.SetScale(scale);

	int shader_id = object.child("shader").text().as_int();
	material = new Material(ResourceManager::getInstance()->LoadShader(shader_id));

	id = object.attribute("id").as_int();

	if (object.child("color"))
	{
		col = true;
		color.x = object.child("color").child("r").text().as_float();
		color.y = object.child("color").child("g").text().as_float();
		color.z = object.child("color").child("b").text().as_float();
	}
	else
	{
		col = false;
	}
	
	if (object.child("size"))
		size = object.child("size").text().as_int();


	if (type == "terrain")
		model = new Model(size, color, scale);
	else
		model = ResourceManager::getInstance()->LoadModel(object.child("model").text().as_int());

	if (object.child("height"))
	{
			height.x = object.child("height").child("r").text().as_float();
			height.y = object.child("height").child("g").text().as_float();
			height.z = object.child("height").child("b").text().as_float();
	}
	
	if (object.child("wired"))
		wired = true;
	else
		wired = false;
	blend = object.child("blend").text().as_bool();
	name = object.child("name").text().as_string("untitled");

	if (object.child("dispMax"))
		dispMax = object.child("dispMax").text().as_float();

	if (object.child("textures"))
	{
		texture = true;
		pugi::xml_node tex = object.child("textures").child("texture");
		int t_id;

		while (tex)
		{
			t_id = tex.attribute("id").as_int();
			material->AddTexture(ResourceManager::getInstance()->LoadTexture(t_id));
			tex = tex.next_sibling();
		}
	}
	else
	{
		texture = false;
	}

	pugi::xml_node fol_cam = object.child("followingCamera");
	if (fol_cam)
	{
		int cam_id;
		pugi::xml_node cam = fol_cam.child("cameras").child("camera");
		while (cam)
		{
			cam_id = cam.text().as_int();
			camera_ids.push_back(cam_id);
			cam = cam.next_sibling();
		}
		if (fol_cam.child("ox"))
			followingCamera.x = 1.0;
		if (fol_cam.child("oy"))
			followingCamera.y = 1.0;
		if (fol_cam.child("oz"))
			followingCamera.z = 1.0;
	}

	pugi::xml_node trj = object.child("trajectory");
	if (trj)
	{
		trajectory = new Trajectory(trj);

		pugi::xml_node point = trj.child("point");
		Vector3 p;
		if (trajectory->type != "circle")
			while (point)
			{
				p.x = point.child("x").text().as_float();
				p.y = point.child("y").text().as_float();
				p.z = point.child("z").text().as_float();
				trajectory->Insert(p);
				point = point.next_sibling("point");
			}
		
		speed = object.child("trajectory").child("speed").text().as_float();
		
		if (trajectory->type != "circle")
		{
			position = trajectory->start->position;
			trajectory->NextInitialPosition();

			Matrix rotX, rotY, rotZ;

		}
	}

}

void SceneObject::AddLights(const Camera *c)
{
	Light* directionalLight, * pointLight, * spotlightLight;
	Vector3 ambientalColor = SceneManager::getInstance()->ambientalLightColor, specualrColor;
	Vector3 diffuseColor, lightDirection, lightPosition;

	if (material->shader->ambientalColorUniform != -1)
		glUniform3f(material->shader->ambientalColorUniform, ambientalColor.x, ambientalColor.y, ambientalColor.z);

	if (material->shader->cameraPositionUniform != -1)
		glUniform3f(material->shader->cameraPositionUniform, c->position.x, c->position.y, c->position.z);

	if (material->shader->ambientalRatioUniform != -1)
		glUniform1f(material->shader->ambientalRatioUniform, SceneManager::getInstance()->ambientalLightRatio);
	
	int idx = 0;
	//directional
	for (auto i = SceneManager::getInstance()->directionalLights.begin(); i != SceneManager::getInstance()->directionalLights.end(); i++)
	{
		directionalLight = i->second;
		specualrColor = directionalLight->specularColor;
		diffuseColor = directionalLight->diffuseColor;
		lightDirection = directionalLight->direction;
		if (material->shader->directionalLights[idx].specularColor != -1)
			glUniform3f(material->shader->directionalLights[idx].specularColor, specualrColor.x, specualrColor.y, specualrColor.z);
		if (material->shader->directionalLights[idx].diffuseColor != -1)
			glUniform3f(material->shader->directionalLights[idx].diffuseColor, diffuseColor.x, diffuseColor.y, diffuseColor.z);
		if (material->shader->directionalLights[idx].direction != -1)
			glUniform3f(material->shader->directionalLights[idx].direction, lightDirection.x, lightDirection.y, lightDirection.z);
		if (material->shader->specPowerUniform != -1)
			glUniform1f(material->shader->specPowerUniform, specPower);
		idx++;
	}

	idx = 0;
	//point
	for (auto i = SceneManager::getInstance()->pointLights.begin(); i != SceneManager::getInstance()->pointLights.end(); i++)
	{
		pointLight = i->second;
		specualrColor = pointLight->specularColor;
		diffuseColor = pointLight->diffuseColor;
		lightPosition = pointLight->position;
		if (material->shader->pointLights[idx].specularColor != -1)
			glUniform3f(material->shader->pointLights[idx].specularColor, specualrColor.x, specualrColor.y, specualrColor.z);
		if (material->shader->pointLights[idx].diffuseColor != -1)
			glUniform3f(material->shader->pointLights[idx].diffuseColor, diffuseColor.x, diffuseColor.y, diffuseColor.z);
		if (material->shader->pointLights[idx].position != -1)
			glUniform3f(material->shader->pointLights[idx].position, lightPosition.x, lightPosition.y, lightPosition.z);
		if (material->shader->specPowerUniform != -1)
			glUniform1f(material->shader->specPowerUniform, specPower);

		if (material->shader->pointLights[idx].Kc != -1)
			glUniform1f(material->shader->pointLights[idx].Kc, pointLight->Kc);
		if (material->shader->pointLights[idx].Kl != -1)
			glUniform1f(material->shader->pointLights[idx].Kl, pointLight->Kl);
		if (material->shader->pointLights[idx].Kq != -1)
			glUniform1f(material->shader->pointLights[idx].Kq, pointLight->Kq);
		idx++;
	}

	idx = 0;
	//spotlight
	for (auto i = SceneManager::getInstance()->spotlightLights.begin(); i != SceneManager::getInstance()->spotlightLights.end(); i++)
	{
		spotlightLight = i->second;
		specualrColor = spotlightLight->specularColor;
		diffuseColor = spotlightLight->diffuseColor;
		lightPosition = spotlightLight->position;
		lightDirection = spotlightLight->direction;
		if (material->shader->spotlightLights[idx].specularColor != -1)
			glUniform3f(material->shader->spotlightLights[idx].specularColor, specualrColor.x, specualrColor.y, specualrColor.z);
		if (material->shader->spotlightLights[idx].diffuseColor != -1)
			glUniform3f(material->shader->spotlightLights[idx].diffuseColor, diffuseColor.x, diffuseColor.y, diffuseColor.z);
		if (material->shader->spotlightLights[idx].position != -1)
			glUniform3f(material->shader->spotlightLights[idx].position, lightPosition.x, lightPosition.y, lightPosition.z);
		if (material->shader->spotlightLights[idx].direction != -1)
			glUniform3f(material->shader->spotlightLights[idx].direction, lightDirection.x, lightDirection.y, lightDirection.z);
		if (material->shader->specPowerUniform != -1)
			glUniform1f(material->shader->specPowerUniform, specPower);

		if (material->shader->spotlightLights[idx].smallLimit != -1)
			glUniform1f(material->shader->spotlightLights[idx].smallLimit, spotlightLight->smallLimit);
		if (material->shader->spotlightLights[idx].bigLimit != -1)
			glUniform1f(material->shader->spotlightLights[idx].bigLimit, spotlightLight->bigLimit);

		if (material->shader->spotlightLights[idx].Kc != -1)
			glUniform1f(material->shader->spotlightLights[idx].Kc, spotlightLight->Kc);
		if (material->shader->spotlightLights[idx].Kl != -1)
			glUniform1f(material->shader->spotlightLights[idx].Kl, spotlightLight->Kl);
		if (material->shader->spotlightLights[idx].Kq != -1)
			glUniform1f(material->shader->spotlightLights[idx].Kq, spotlightLight->Kq);
		idx++;
	}
}

void SceneObject::AddFog()
{
	if (material->shader->minRadiusUniform != -1)
		glUniform1f(material->shader->minRadiusUniform, SceneManager::getInstance()->minRadius);

	if (material->shader->maxRadiusUniform != -1)
		glUniform1f(material->shader->maxRadiusUniform, SceneManager::getInstance()->maxRadius);

	if (material->shader->fogColorUniform != -1)
		glUniform3f(material->shader->fogColorUniform, SceneManager::getInstance()->fogColor.x, SceneManager::getInstance()->fogColor.y, SceneManager::getInstance()->fogColor.z);
}

void SceneObject::AddShaderBasics(const Camera *c,  Matrix& modelMatrix)
{
	if (material->shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(material->shader->positionAttribute);
		glVertexAttribPointer(material->shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (material->shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(material->shader->uvAttribute);
		glVertexAttribPointer(material->shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	if (material->shader->normAttribute != -1)
	{
		glEnableVertexAttribArray(material->shader->normAttribute);
		glVertexAttribPointer(material->shader->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
	}

	if (material->shader->mini_uvAttribute != -1)
	{
		glEnableVertexAttribArray(material->shader->mini_uvAttribute);
		glVertexAttribPointer(material->shader->mini_uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mini_uv));
	}

	Matrix mvp = modelMatrix * c->viewProjectionMatrix;
	if (material->shader->mvpMatrixUniform != -1)
		glUniformMatrix4fv(material->shader->mvpMatrixUniform, 1, GL_FALSE, (GLfloat*)mvp.m);

	if (material->shader->modelMatrixUniform != -1)
		glUniformMatrix4fv(material->shader->modelMatrixUniform, 1, GL_FALSE, (GLfloat*)modelMatrix.m);


	for (int i = 0; i < material->textures.size(); i++)
	{
		if (material->shader->textureUniform[i] != -1)
			glUniform1i(material->shader->textureUniform[i], i);
		material->textures[i]->Bind(i);
	}
}

void SceneObject::Update()
{
	Camera* c = SceneManager::getInstance()->cameras[SceneManager::getInstance()->activeCamera];
	if (followingCamera.x)
		position.x = c->position.x;
	if (followingCamera.y)
		position.y = c->position.y;
	if (followingCamera.z)
		position.z = c->position.z;


	if (trajectory && trajectory->type != "circle")
	{
		if (trajectory->initial && c->deltatime != 0.0)
		{
			Vector3 d = trajectory->movementDirection * speed * c->deltatime;
			if (d.Length() <= (trajectory->initial->position - position).Length())
				position = position + d;
			else
				position = trajectory->initial->position;

			if (position == trajectory->initial->position)
			{
				trajectory->NextInitialPosition();
			}
		}
	}

	

	if (type != "terrain")
	{
		Matrix translationMatrix;
		Matrix angleRotationMatrix;

		if (trajectory)
		{
			if (trajectory->type == "circle")
			{
				if (c->deltatime)
				{
					trajectory->distance += speed * c->deltatime;
					if (trajectory->distance >= 2 * pi * trajectory->radius)
						trajectory->distance -= 2 * pi * trajectory->radius;
					float rotationAngle = trajectory->distance / trajectory->radius;
					Matrix M = translationMatrix.SetTranslation(trajectory->radius, 0.0, 0.0) * angleRotationMatrix.SetRotationY(rotationAngle) * trajectory->planeRotationMatrix * trajectory->centerTranslationMatrix;
					Vector4 pos = Vector4(0.0, 0.0, 0.0, 1.0) * M;
					position = Vector3(pos.x / pos.w, pos.y / pos.w, pos.z / pos.w);
					modelMatrix = scaleMatrix * rotationMatrix * M;
				}
			}
			else
				modelMatrix = scaleMatrix * rotationMatrix * trajectory->R * translationMatrix.SetTranslation(position);
		}
		else
			modelMatrix = scaleMatrix * rotationMatrix * translationMatrix.SetTranslation(position);
	}

}

void SceneObject::Draw(bool wiredformat, float &time)
{ 
	if (blend == true)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	if (type == "terrain")
	{
		DrawTerrain();
	}
	else
	{
		Camera* c = SceneManager::getInstance()->cameras[SceneManager::getInstance()->activeCamera];

		if (type == "normal" || type == "skybox")
			DrawNormal(c, modelMatrix);
		else
			DrawFire(c, modelMatrix, time);
	}
}

void SceneObject::DrawNormal(const Camera* c, Matrix& modelMatrix)
{

	glUseProgram(material->shader->program);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBOid);

	AddShaderBasics(c, modelMatrix);
	AddFog();
	AddLights(c);

	glDrawElements(GL_TRIANGLES, model->nrIndices, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::DrawFire(const Camera* c, Matrix& modelMatrix, float& time)
{
	if (time > 1)
		time = fmod(time, 1);

	glUseProgram(material->shader->program);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBOid);

	AddShaderBasics(c, modelMatrix);
	
	if (material->shader->timeUniform != -1)
		glUniform1f(material->shader->timeUniform, time);

	if (material->shader->dispMaxUniform != -1)
		glUniform1f(material->shader->dispMaxUniform, dispMax);

	AddFog();

	glDrawElements(GL_TRIANGLES, model->nrIndices, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::DrawTerrain()
{
	glUseProgram(material->shader->program);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBOid);

	Matrix translationMatrix;
	Camera* c = SceneManager::getInstance()->cameras[SceneManager::getInstance()->activeCamera];
	Vector3 pos = position;
	pos.x = pos.x - fmod(pos.x, scale.x);
	pos.z = pos.z - fmod(pos.z, scale.z);

	modelMatrix = rotationMatrix * translationMatrix.SetTranslation(pos);

	AddShaderBasics(c, modelMatrix);


	if (material->shader->translationUniform != -1)
	{
		Vector2 tr = Vector2((pos.x) / (size * scale.x), (pos.z) / (size * scale.z));
		glUniform2f(material->shader->translationUniform, tr.x, tr.y);
	}


	if (material->shader->heightUniform != -1)
		glUniform3f(material->shader->heightUniform, height.x, height.y, height.z);

	AddFog();
	AddLights(c);

	glDrawElements(GL_TRIANGLES, model->nrIndices, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

SceneObject::~SceneObject()
{
	delete material;
	if (type == "terrain")
		delete model;
	if (trajectory)
		delete trajectory;
}
