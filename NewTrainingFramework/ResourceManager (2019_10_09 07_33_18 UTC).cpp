#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance=nullptr;

ModelResource* readModel(pugi::xml_node node)
{

	pugi::xml_node child = node.first_child();
	pugi::xml_attribute attr = node.attribute("id");
	ModelResource* mr = new ModelResource(attr.as_int(), "../" + std::string(child.text().as_string()));
	return mr;
}

ShaderResource* readShader(pugi::xml_node node)
{
	pugi::xml_attribute attr = node.attribute("id");
	int id = attr.as_int();
	std::string vs(node.child("vs").text().as_string());
	std::string fs(node.child("fs").text().as_string());
	ShaderResource* sr = new ShaderResource(id, "../" + vs, "../" + fs);
	return sr;
}

TextureResource* readTexture(pugi::xml_node node)
{
	int id = node.attribute("id").as_int();
	std::string type(node.attribute("type").as_string());
	std::string path(node.child("path").text().as_string());
	std::string min_filter(node.child("min_filter").text().as_string());
	std::string mag_filter(node.child("mag_filter").text().as_string());
	std::string wrap_s(node.child("wrap_s").text().as_string());
	std::string wrap_t(node.child("wrap_t").text().as_string());
	TextureResource* tr = new TextureResource(id, type, "../" + path, min_filter, mag_filter, wrap_s, wrap_t);
	return tr;
}

ResourceManager::ResourceManager()
{
	Init("../Resources/XMLs/resourceManager.xml");
}

void ResourceManager::Init(const char *xmlPath)
{
	ModelResource* mr;
	TextureResource* tr;
	ShaderResource* sr;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlPath,
		pugi::parse_default | pugi::parse_declaration);
	if (!result)
	{
		std::cout << "Parse error: " << result.description()
			<< ", character pos= " << result.offset;
	}
	pugi::xml_node root = doc.document_element();
	pugi::xml_node node = root.child("models"), resource = node.first_child();
	
	while (resource)
	{
		mr = readModel(resource);
		mrMap.insert(std::pair<int, ModelResource*> (mr->id, mr));
		resource = resource.next_sibling();
	}

	node = root.child("shaders");
	resource = node.first_child();
	while (resource)
	{
		sr = readShader(resource);
		srMap.insert(std::pair<int, ShaderResource*>(sr->id, sr));
		resource = resource.next_sibling();
	}

	node = root.child("textures");
	resource = node.first_child();
	while (resource)
	{
		tr = readTexture(resource);
		trMap.insert(std::pair<int, TextureResource*>(tr->id, tr));
		resource = resource.next_sibling();
	}

}

Shader* ResourceManager::LoadShader(int id)
{
	return Load(srMap, shaderMap, id);
}

Model* ResourceManager::LoadModel(int id)
{
	return Load(mrMap, modelMap, id);
}

Texture* ResourceManager::LoadTexture(int id)
{
	return Load(trMap, textureMap, id);
}

ResourceManager* ResourceManager::getInstance()
{
	if (!instance)
		instance = new ResourceManager();
	return instance;
}

void ResourceManager::freeResources()
{
	for (auto it_sr = srMap.begin(); it_sr != srMap.end(); it_sr++)
		delete it_sr->second;
	for (auto it_tr = trMap.begin(); it_tr != trMap.end(); it_tr++)
		delete it_tr->second;
	for (auto it_mr = mrMap.begin(); it_mr != mrMap.end(); it_mr++)
		delete it_mr->second;

	for (auto it_s = shaderMap.begin(); it_s != shaderMap.end(); it_s++)
		delete it_s->second;
	for (auto it_t = textureMap.begin(); it_t != textureMap.end(); it_t++)
		delete it_t->second;
	for (auto it_m = modelMap.begin(); it_m != modelMap.end(); it_m++)
		delete it_m->second;

}

ResourceManager::~ResourceManager()
{
	freeResources();
}
