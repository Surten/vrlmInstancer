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

	void parseColor(ShapeNode* shapeNode);

	void parseCoords(ShapeNode* shapeNode);

	void parseTexCoords(ShapeNode* shapeNode);

	void parseColorIndex(ShapeNode* shapeNode);
	
	void parseCoordIndex(ShapeNode* shapeNode);

	void parseTexCoordIndex(ShapeNode* shapeNode);

	void parseSpotLight(LightNode* light);

	void parseViewPoint(ViewPointNode* viewPointNode);

public:
	bool lastWasNumber;

	std::vector<BaseNode*> * AllNodes;
	std::vector<BaseNode*> * RootNodes;
	std::vector<ShapeNode*> * ShapeNodes;
	std::vector<Geometry*> * geometries;
	std::vector<LightNode*> * lights;

public:
	VrmlParser(std::vector<BaseNode*> *AllNodes, std::vector<BaseNode*> *RootNodes,
		std::vector<ShapeNode*> *ShapeNodes, std::vector<Geometry*>* geometries, std::vector<LightNode*>* lights);
	bool parseFile(const char* vrlmFileName);
};



