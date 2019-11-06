#include "stdafx.h"
#include "Model.h"
float* extractFloats(std::string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]) && str[i] != '.' && str[i] != '-')
		{
			str[i] = ' ';
		}
	}
	std::stringstream ss;
	ss << str;
	std::string temp;
	float found, * numbers = new float[param_num];

	ss >> temp;
	for (int i = 0; i < param_num; i++)
	{
		ss >> temp;
		std::stringstream(temp) >> numbers[i];
		temp = "";
	}
	return numbers;
}

std::vector<float> extractInfo(std::ifstream& input)
{
	int vertices_no;
	float* numbers;
	std::vector<float> Info;
	std::string line, temp;
	std::stringstream ss;
	getline(input, line);
	ss << line;
	ss >> temp >> temp;
	std::stringstream(temp) >> vertices_no;

	for (int i = 1; i <= vertices_no; i++)
	{
		getline(input, line);
		numbers = extractFloats(line);
		for (int j = 0; j < param_num; j++)
		{
			Info.push_back(numbers[j]);
		}
		delete[] numbers;
	}
	return Info;
}

unsigned short* extractShorts(std::string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
		{
			str[i] = ' ';
		}
	}
	std::stringstream ss;
	ss << str;
	std::string temp;
	unsigned short* numbers = new unsigned short[indices_num];
	ss >> temp;
	for (int i = 0; i < indices_num; i++)
	{
		ss >> temp;
		std::stringstream(temp) >> numbers[i];
		temp = "";
	}
	return numbers;
}

std::vector<unsigned short> extractIndices(std::ifstream& input)
{
	int indices_no;
	unsigned short* numbers;
	std::vector<unsigned short> Ind;
	std::string line, temp;
	std::stringstream ss;
	getline(input, line);
	ss << line;
	ss >> temp >> temp;
	std::stringstream(temp) >> indices_no;

	for (int i = 1; i <= indices_no / 3; i++)
	{
		getline(input, line);
		numbers = extractShorts(line);
		for (int j = 0; j < indices_num; j++)
		{
			Ind.push_back(numbers[j]);
		}
		delete[] numbers;
	}
	return Ind;
}

void loadModelInfo(const char* filename, std::vector<Vertex>& vertices, std::vector<unsigned short>& Indices)
{
	std::ifstream input;
	input.open(filename);
	std::vector<float> Info = extractInfo(input);
	Indices = extractIndices(input);
	input.close();
	Vertex v;
	int i = 0;

	while (i < Info.size())
	{
		v.pos.x = Info[i++];
		v.pos.y = Info[i++];
		v.pos.z = Info[i++];
		v.norm.x = Info[i++];
		v.norm.y = Info[i++];
		v.norm.z = Info[i++];
		v.binorm.x = Info[i++];
		v.binorm.y = Info[i++];
		v.binorm.z = Info[i++];
		v.tgt.x = Info[i++];
		v.tgt.y = Info[i++];
		v.tgt.z = Info[i++];
		v.uv.x = Info[i++];
		v.uv.y = Info[i++];
		vertices.push_back(v);
	}
}

Model::Model(ModelResource* r)
{
	mr = r;
	
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	loadModelInfo(mr->path.c_str(), vertices, indices);
	nrIndices = indices.size();

	glGenBuffers(1, &VBOid);
	glBindBuffer(GL_ARRAY_BUFFER, VBOid);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model::Model(int size, Vector3 color, Vector3 scale)
{
	Vertex v;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	
	for (int i = 0; i <= size; i++)
		for (int j = 0; j <= size; j++)
		{
			v.pos.x = (j - size / 2) * scale.x;
			v.pos.y = 0;
			v.pos.z = (i - size / 2) * scale.z;
			v.uv.x = 1. * j / size;
			v.uv.y = 1. * i / size;
			v.mini_uv = v.uv * size;
			v.color = color;
			v.norm = Vector3(0.0, 1.0, 0.0);
			vertices.push_back(v);

			if (i < size && j < size)
			{
				indices.push_back(i * (size + 1) + j);
				indices.push_back(i * (size + 1) + j + 1);
				indices.push_back((i + 1) * (size + 1) + j);

				indices.push_back((i + 1) * (size + 1) + j + 1);
				indices.push_back(i * (size + 1) + j + 1);
				indices.push_back((i + 1) * (size + 1) + j);
			}
		}
	nrIndices = indices.size();

	glGenBuffers(1, &VBOid);
	glBindBuffer(GL_ARRAY_BUFFER, VBOid);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model::~Model()
{
	glDeleteBuffers(1, &VBOid);
	glDeleteBuffers(1, &IBOid);
}
