#include "stdafx.h"
#include "Fire.h"


Fire::Fire(pugi::xml_node object) : SceneObject(object)
{
}

/*void Draw(bool wiredformat, float& time)
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
*/
Fire::~Fire()
{
}
