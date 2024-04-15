#pragma once

#include <fstream>
#include <string>
#include "baseNode.h"

class VrmlParser {

private:
	std::ifstream vrlmFile;
	std::string str;
	float n;


private:
	void loadFile(const char* vrlmFileName);

	void skipComments();

	void readSymbol();

	void parseDEF(TransformNode* parent);

	void parseTransformNode(TransformNode* transformNode);

	void parseChildren(TransformNode* parent);

	void parseNextNode();

	void parseShape(TransformNode* parent);

	void parseApperance(ShapeNode* shapeNode);

	void parseGeometry(ShapeNode* shapeNode);

	void parseMaterial(ShapeNode* shapeNode);

	void parseTexture(ShapeNode* shapeNode);

	void parseCoords(ShapeNode* shapeNode);

	void parseTexCoords(ShapeNode* shapeNode);

	void parseCoordIndex(ShapeNode* shapeNode);

	void parseTexCoordIndex(ShapeNode* shapeNode);

public:
	bool lastWasNumber;

	std::vector<BaseNode*> AllNodes;
	std::vector<BaseNode*> RootNodes;

public:
	void parseFile(const char* vrlmFileName);
};



