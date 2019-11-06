#pragma once
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include"Vertex.h"
#include "ModelResource.h"
#include "Camera.h"
#include "Material.h"
#include "SceneManager.h"
#include <time.h>

#define param_num 14
#define indices_num 3

class Model
{

public:
	Model(ModelResource* r);
	Model(int size, Vector3 color, Vector3 scale);
	~Model();

	ModelResource* mr;
	GLuint IBOid=0, wiredIBOid=0, VBOid=0;
	GLuint nrIndices=0, nrIndicesWired=0;
	
};
