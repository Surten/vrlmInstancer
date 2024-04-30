#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "baseNode.h"

class VrmlSaver {
public:
	std::vector<BaseNode*> * AllNodes;
	std::vector<BaseNode*> * RootNodes;
	std::vector<ShapeNode*> * ShapeNodes;
	std::vector<Geometry*> * geometries;

public:

	VrmlSaver(std::vector<BaseNode*>* AllNodes, std::vector<BaseNode*>* RootNodes, std::vector<ShapeNode*>* ShapeNodes, std::vector<Geometry*>* geometries);

	void saveLoadedToVrml(const char* outputFileName);

private:
	std::ofstream out;
	int numOfPointsPerLine = 10;
	int numOfIndicesPerLine = 8;

private:
	void writeHeader();
	std::string getLeadingSpaces(int numberOfSpaces);
	void writeNodesfromRoot();
	void writeTransformNode(TransformNode* node);
	void writeShapeNode(ShapeNode* node);
	void writeChildren(TransformNode* node);
	void writeApperance(ShapeNode* node);
	void writeMaterial(ShapeNode* node);
	void writeTexture(ShapeNode* node);
	void writeGeometryDEF(ShapeNode* node);
	void writeGeometryCoords(ShapeNode* node);
	void writeGeometryTexCoords(ShapeNode* node);
	void writeGeometryIndices(ShapeNode* node);
	void writeGeometryTextureIndices(ShapeNode* node);
	void writeGeometryUSE(ShapeNode* node);
};