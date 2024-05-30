#pragma once
#include <vector>

#include "vrlmParser.h"
#include "vrmlSaver.h"

class Scene {

public:
	Scene(std::string name) : name(name), vrmlParser(&AllNodes, &RootNodes, &ShapeNodes, &geometries, &lights), vrmlSaver(&AllNodes, &RootNodes, &ShapeNodes, &geometries, &lights){}
	~Scene();

	bool loadSceneFromVrmlFile(std::string filePath);
	void saveSceneToVrmlFile(std::string outFilePath);
	void geometryFun();
	void writeOutGeometries();
	void findAndUseDuplicateGeometry();
	void findAndUseSameObjects(Scene* otherScene);
	void findAndUseSameObjectsFromOtherScenesInThisScene(std::vector<Scene*> scenes);

	std::string name;

private:
	VrmlParser vrmlParser;
	VrmlSaver vrmlSaver;

	std::vector<BaseNode*> AllNodes;
	std::vector<BaseNode*> RootNodes;
	std::vector<ShapeNode*> ShapeNodes;
	std::vector<Geometry*> geometries;
	std::vector<LightNode*> lights;

private:
	void findDuplicateGeometry(std::vector<std::pair<int, int>>& geoPairs);
};