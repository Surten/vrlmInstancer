#pragma once
#include <iostream>
#include <fstream>
#include <vector>

#include "baseNode.h"
#include "materials.h"
#include "scene.h"


class PbrtExporter {

public:
	//Scene* scene;

public:

	PbrtExporter(AnimationInfo* animInfo);

	void exportScene(std::vector<Scene*> scenes, ViewPointNode* camera, std::string outputFolder,
		std::string headerFileName, std::string outputImageFormat, bool createNewGeometry,
		MaterialsFile* matFile, std::string integrator, AnimationInfo* animInfo);

private:
	std::ofstream out;
	std::ofstream outGeometry;
	std::ofstream outMaterials;

	std::string currentGeometryFileName;

	std::vector<Scene*> scenes;

	std::string outputFolder;
	std::string headerFileName;
	std::string integrator;

	std::string geometryAppendString;

	AnimationInfo* animInfo;

	std::string outputImageFormat;
	ViewPointNode* camera;
	bool createNewGeometry;


	int numberOfSamples = 64;
	int maxDepth = 12;
	int xResolution = 800;
	int yResolution = 600;

private:

	void exportStatic();
	void exportDynamic();
	void writeSceneWideOptions(std::string renderImageFileName);
	void writeCamera();
	void writeSampler();
	void writeIntegrator();
	void writeFilter();
	void writeFilm(std::string renderImageFileName);
	void writeFloor();
	void writeLightSource(LightNode* camera);
	void writeAllLightSourcesOfAScene(Scene* scene);
	void writeTexture();
	std::string getStringWithLeadingZeros(int number);
	void initCurrentGeometryFilename(const std::string& sceneName, bool animated = false, int animationTimeIndex = -1);
	void writeGeometry(Scene* scene);
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


	void writeAllMaterials(MaterialsFile* material);
	void writeMaterialReference(Mat* material);
	void writeMaterialNamed(Mat* material);


	void writeMaterialDiffuse(Mat* material);
	void writeMaterialCoatedDiffuse(Mat* material);
	void writeMaterialDiffuseTransmission(Mat* material);
	void writeMaterialDielectric(Mat* material);
	void writeMaterialConductor(Mat* material);
	void writeMaterialConductorReflectance(Mat* material);
	void writeMaterialCoatedConductor(Mat* material);
	void writeMaterialBTF(Mat* material);


	void writeBaseTransformNode(TransformNode* node);

	void pbrtTransformDoor(TransformNode* node);
	void pbrtTransformDoorHandle(TransformNode* node);
	void pbrtTransformWindow(TransformNode* node);
	void pbrtTransformWindowHandle(TransformNode* node);
	void pbrtTransformWindowShutter(TransformNode* node);
};