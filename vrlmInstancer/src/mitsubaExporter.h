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

	void exportScene(std::vector<Scene*> scenes, ViewPointNode* camera, std::string sceneFileName);

private:
	std::ofstream out;

	std::vector<Scene*> scenes;

	int pathTracingMaxDepth = 65;
	int nSamples = 64;
	float cameraFOV = 40;

	int imageWidth = 500;
	int imageHeight = 500;


private:
	void writeElement(std::string elementName, std::vector<std::string> attributes, int depth);
	void writeElementBegin(std::string elementName, std::vector<std::string> attributes, int depth);
	void writeElementEnd(std::string elementName, int depth);
	std::string& getLeadingSpaces(int depth);


	void writeIntegrator(int depth);
	void writeSensor(int depth);
	void writeSampler(int depth);
	void writeFilm(int depth);

	void writeMaterial(Material* mat, int depth);
	void writeShape(ShapeNode* shapeNode, std::string filename, int depth);
	void writeTransform(ShapeNode* shapeNode, int depth);
};