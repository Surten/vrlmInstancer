#pragma once
#include <iostream>
#include <fstream>
#include <vector>

#include "baseNode.h"
#include "scene.h"


class PbrtExporter {

public:
	//Scene* scene;

public:

	PbrtExporter();

	void exportScene(std::vector<Scene*> scenes, ViewPointNode* camera, std::string folder, std::string headerFileName, std::string renderImageFileName, bool createNewGeometry);

private:
	std::ofstream out;
	std::ofstream outGeometry;

	std::string currentGeometryFileName;

	std::vector<Scene*> scenes;

	int numberOfSamples = 64;
	int maxDepth = 8;
	int xResolution = 1280;
	int yResolution = 900;

private:

	void writeSceneWideOptions(const ViewPointNode* camera, std::string renderImageFileName);
	void writeCamera(const ViewPointNode* camera);
	void writeSampler();
	void writeIntegrator();
	void writeFilter();
	void writeFilm(std::string renderImageFileName);
	void writeFloor();
	void writeLightSource(LightNode* camera);
	void writeAllLightSourcesOfAScene(Scene* scene);
	void writeTexture();
	void writeGeometry(Scene* scene, std::string folder, bool createNewGeometry);
	void writeObjectInstances(Scene* scene);
	void writeSceneHierarchy(Scene* scene);
	void writeTransformNode(TransformNode* node);
	void writeNodeChildren(TransformNode* node);
	void writeShapeNode(ShapeNode* node);
	void writeMaterial(Material* material);
	void writeMaterialWithTexture(Material* material);
	void writeTriangleMesh(Geometry* material);
	void writeTriangleMeshWithTexture(Geometry* material);
	void convertTextureCoordsIntoPBRTFormat(Geometry* geometry, std::vector<vec3>& newCoords,
		std::vector<vec2>& newTexCoords, std::vector<vec3i>& newCoordIndex);
};