#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "baseNode.h"
#include "scene.h"

/// <summary>
/// Responsible for exporting the data into a VRML 2.0 file
/// Is created by and gets references to the data from Scene class
/// </summary>
class VrmlSaver {
public:


public:

	VrmlSaver();

	/// <summary>
	/// Exports the scene data to a file  on a given path
	/// </summary>
	void saveSceneToVrml(const char* outputFileName, Scene* scene);

private:
	std::ofstream out;
	int numOfPointsPerLine = 3;
	int numOfIndicesPerLine = 3;
	Scene* scene;

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
	void writeLightNode(LightNode* lightNode);
	void writeViewPointNode(ViewPointNode* lightNode);

};