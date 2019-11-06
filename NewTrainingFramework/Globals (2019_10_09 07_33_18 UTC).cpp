#include "stdafx.h"
#include "Globals.h"
int Globals::screenWidth;
int Globals::screenHeight;

void Globals::loadScreen(char* xmlpath)
{
	pugi::xml_document doc;
	// Load XML file into memory
	// Remark: to fully read declaration entries you have to specify
	// "pugi::parse_declaration"
	pugi::xml_parse_result result = doc.load_file(xmlpath,
		pugi::parse_default | pugi::parse_declaration);
	if (!result)
	{
		std::cout << "Parse error: " << result.description()
			<< ", character pos= " << result.offset;
	}
	// A valid XML document must have a single root node
	pugi::xml_node root = doc.document_element();
	pugi::xml_node node = root.child("screen");

	// Read attribute value
	pugi::xml_attribute attr;
	if (attr = node.attribute("width"))
	{
		screenWidth = attr.as_int();
	}
	if (attr = node.attribute("height"))
	{
		screenHeight = attr.as_int();
	}
}