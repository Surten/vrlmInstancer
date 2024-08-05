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
	std::ofstream out;

private:

	void writeSceneWideOptions();
	void writeCamera(const ViewPointNode& camera);
	void writeSampler();
	void writeIntegrator();
	void writeFilm();
	void writeLightSource(const LightNode& camera);
	void writeAtribute();

	void computeLookAt(vec3& loc, vec3& dir, vec3& up);




};