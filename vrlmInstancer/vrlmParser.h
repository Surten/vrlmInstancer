#pragma once

#include <fstream>
#include <string>

class VrmlParser {

private:
	std::ifstream vrlmFile;
	std::string str;
	float n;

private:
	void loadFile(const char* vrlmFileName);

	void skipComments();

	void readSymbol();

	void parseDEF();

	void parseTransformNode(std::string name);

	void parseChildren(std::string name);

	void parseNextNode();

	void parseShape(std::string name);

	void parseApperance(std::string name);

	void parseGeometry(std::string name);

	void parseMaterial(std::string name);

	void parseTexture(std::string name);

	void parseCoords();

	void parseTexCoords();

	void parseCoordIndex();

	void parseTexCoordIndex();

public:
	void parseFile(const char* vrlmFileName);

public:
	bool lastWasNumber;
};



