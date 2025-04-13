#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "baseNode.h"
#include "scene.h"
#include "materials.h"


class MitsubaExporter {

public:

public:

	MitsubaExporter(AnimationInfo* animInfo);

	void exportScene(std::vector<Scene*> scenes, ViewPointNode* camera, std::string sceneFileName, std::string outputFolder, bool createNewGeometry, MaterialsFile* matFile);

private:
	std::ofstream out;
	std::ofstream outCurrentScene;

	std::vector<Scene*> scenes;

	MaterialsFile* matFile;
	ViewPointNode*  camera;

	AnimationInfo* animInfo;

	std::string outputFolder;
	std::string sceneFileName;
	std::string outputFolder;
	std::string currentGeometryFileName;
	std::string currentGeometryFolderPath;

	bool createNewGeometry;

	int pathTracingMaxDepth = -1;
	int nSamples = 2048;
	int imageWidth = 1920;
	int imageHeight = 1080;


private:
	void exportStatic();
	void exportDynamic();

	void writeElement(std::string elementName, std::vector<std::string> attributes, int depth);
	void writeElementBeg(std::string elementName, std::vector<std::string> attributes, int depth);
	void writeElementEnd(std::string elementName, int depth);
	void writeElementScene(std::string elementName, std::vector<std::string> attributes, int depth);
	void writeElementBegScene(std::string elementName, std::vector<std::string> attributes, int depth);
	void writeElementEndScene(std::string elementName, int depth);
	std::string getLeadingSpaces(int depth);

	void writeGeometryToObj(Geometry* geometry, std::string filePath);
	void writeAllGeometriesToObjFiles();

	void writeIntegrator(int depth);
	void writeSensor(int depth);
	void writeSampler(int depth);
	void writeFilm(int depth);

	void writeGeometryXML(Scene* scene, int depth);
	void writeAllShapeGroups(Scene* scene, int depth);
	void writeAllShapeReferences(Scene* scene, int depth);
	void writeShape(ShapeNode* shapeNode, std::string filepath, int depth);
	void writeShapeGroup(ShapeNode* shapeNode, std::string filepath, int depth);
	void writeShapeReference(ShapeNode* shapeNode, std::string filepath, int depth);
	void writeTransform(ShapeNode* shapeNode, int depth);
	void writeAllNamedMaterials(int depth);
	void writeBsdfNamed(Mat* material, int depth);
	void writeBsdfReference(Mat* material, int depth);
	void writeAllLights(Scene* scene, int depth);
	void writeLight(LightNode* lightNode, int depth);

	void writeMaterialDiffuse(Mat* material, int depth);
	void writeMaterialCoatedDiffuse(Mat* material, int depth);
	void writeMaterialDiffuseTransmission(Mat* material, int depth);
	void writeMaterialDielectric(Mat* material, int depth);
	void writeMaterialConductor(Mat* material, int depth);
	void writeMaterialConductorReflectance(Mat* material, int depth);
	void writeMaterialCoatedConductor(Mat* material, int depth);
	void writeMaterialBTF(Mat* material, int depth);


	std::string getStringWithLeadingZeros(int number);
	void initCurrentGeometryFilename(const std::string& sceneName, bool animated = false, int animationTimeIndex = -1);

	void mitsubaTransformNodeBase(TransformNode* transformNode, std::stack<Transform>& transforms);
	void mitsubaTransformDoor(TransformNode* node, std::stack<Transform>& transforms);
	void mitsubaTransformDoorHandle(TransformNode* node, std::stack<Transform>& transforms);
	void mitsubaTransformWindow(TransformNode* node, std::stack<Transform>& transforms);
	void mitsubaTransformWindowHandle(TransformNode* node, std::stack<Transform>& transforms);
	void mitsubaTransformWindowShutter(TransformNode* node, std::stack<Transform>& transforms);
};