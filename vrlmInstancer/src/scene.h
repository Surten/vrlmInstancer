#pragma once
#include <vector>

#include "vrlmParser.h"
#include "vrmlSaver.h"

class Scene {

public:
	Scene() : vrmlParser(&AllNodes, &RootNodes, &ShapeNodes, &geometries, &lights), vrmlSaver(&AllNodes, &RootNodes, &ShapeNodes, &geometries, &lights){}
	
	void loadSceneFromVrmlFile(std::string filePath);
	void saveSceneToVrmlFile(std::string outFilePath);
	void geometryFun();
	void writeOutGeometries();
	void findAndUseDuplicateGeometry();
	void findSimilarObjects(Scene* otherScene);


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