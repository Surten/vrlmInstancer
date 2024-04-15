#pragma once
#include <vector>

#include "vrlmParser.h"

class Scene {

public:
	Scene() {}
	
	void loadSceneFromVrmlFile(std::string filePath) {
		vrmlParser.parseFile(filePath.c_str());
	}

private:
	VrmlParser vrmlParser;
};