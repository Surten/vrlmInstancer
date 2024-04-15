#pragma once
#include <vector>

#include "vrlmParser.h"
#include "vrmlSaver.h"

class Scene {

public:
	Scene() {}
	
	void loadSceneFromVrmlFile(std::string filePath);
	void saveSceneToVrmlFile(std::string outFilePath);

private:
	VrmlParser vrmlParser;
	VrmlSaver vrmlSaver;
};