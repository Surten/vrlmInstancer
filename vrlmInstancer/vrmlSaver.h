#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "baseNode.h"

class VrmlSaver {
public:
	std::vector<BaseNode*> * AllNodes;
	std::vector<BaseNode*> * RootNodes;

public:

	VrmlSaver();

	void saveLoadedToVrml(const char* outputFileName);

private:
	std::ofstream out;

private:
	void writeHeader();
	void writeNodesfromRoot();
	void writeTransformNode(TransformNode* node);
	void writeShapeNode(ShapeNode* node);
	void writeChildren(TransformNode* node);
	void writeApperance(ShapeNode* node);
	void writeMaterial(ShapeNode* node);
	void writeTexture(ShapeNode* node);
	void writeGeometryDEF(ShapeNode* node);
};