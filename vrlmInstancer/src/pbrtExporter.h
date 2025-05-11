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

	/// <summary>
	///  Exports all loaded scenes to the PBRTv4 file Format
	/// </summary>
	/// <param name="scenes">List of loaded scenes</param>
	/// <param name="camera">The camera to be used for PBRT render</param>
	/// <param name="outputFolder">The path to the folder where the entire export should be</param>
	/// <param name="headerFileName">The header file name</param>
	/// <param name="outputImageFormat">The output will be named the same as the header file, but with this extension</param>
	/// <param name="createNewGeometry">if False, only creates the header file</param>
	/// <param name="matFile">pointer the all the Materials loaded from MaterialsFile</param>
	/// <param name="integrator">Name of the integrator used for rendering</param>
	/// <param name="width">Resulting image Width</param>
	/// <param name="height"><Resulting image Height/param>
	/// <param name="samples">Number of samples per pixel</param>
	/// <param name="animInfo">Pointer to the AnimInfo, which contains info about all desired animations</param>
	void exportScene(std::vector<Scene*> scenes, ViewPointNode* camera, std::string outputFolder,
		std::string headerFileName, std::string outputImageFormat, bool createNewGeometry,
		MaterialsFile* matFile, std::string integrator, int width, int height, int samples, AnimationInfo* animInfo);

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
	int xResolution = 300;
	int yResolution = 200;

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
	void writeMaterialReference(Mat* material, bool hasTextureCoordinates);
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