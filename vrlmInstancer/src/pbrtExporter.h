#pragma once
#include <iostream>
#include <fstream>
#include <vector>

#include "baseNode.h"
#include "scene.h"


class PbrtExporter {

public:
	Scene* scene;

public:

	PbrtExporter();

	void exportScene(Scene* scene);

private:
	std::ofstream outGeometry;
	std::ofstream outMain;

private:




};