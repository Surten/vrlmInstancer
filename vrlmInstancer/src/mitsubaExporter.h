#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "baseNode.h"
#include "scene.h"


class MitsubaExporter {

public:

public:

	MitsubaExporter();

	void exportScene(std::vector<Scene*> scenes, ViewPointNode* camera, std::string sceneFileName, std::string outputFolder);

private:
	std::ofstream out;

	std::vector<Scene*> scenes;

	std::string outputFolder;

	int pathTracingMaxDepth = 65;
	int nSamples = 64;
	int imageWidth = 500;
	int imageHeight = 500;


private:
	void writeElement(std::string elementName, std::vector<std::string> attributes, int depth);
	void writeElementBeg(std::string elementName, std::vector<std::string> attributes, int depth);
	void writeElementEnd(std::string elementName, int depth);
	std::string getLeadingSpaces(int depth);

	void writeGeometryToObj(Geometry* geometry, std::string filePath);
	void writeAllGeometriesToObjFiles();

	void writeIntegrator(int depth);
	void writeSensor(ViewPointNode* camera, int depth);
	void writeSampler(int depth);
	void writeFilm(int depth);

	void writeMaterial(Material* mat, int depth);
	void writeShape(ShapeNode* shapeNode, std::string filename, int depth);
	void writeTransform(ShapeNode* shapeNode, int depth);
	void writeBsdfNamed(Material* material, int depth);
	void writeBsdf(Material* material, int depth);
	void writeLight(LightNode* lightNode, int depth);
};