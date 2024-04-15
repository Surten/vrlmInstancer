#include "scene.h"

void Scene::loadSceneFromVrmlFile(std::string filePath) {
	vrmlParser.parseFile(filePath.c_str());
}

void Scene::saveSceneToVrmlFile(std::string outFilePath) {
	vrmlSaver.AllNodes = &vrmlParser.AllNodes;
	vrmlSaver.RootNodes = &vrmlParser.RootNodes;

	vrmlSaver.saveLoadedToVrml(outFilePath.c_str());
}