#include "scene.h"

void Scene::loadSceneFromVrmlFile(std::string filePath) {
	vrmlParser.parseFile(filePath.c_str());
}

void Scene::saveSceneToVrmlFile(std::string outFilePath) {
	vrmlSaver.AllNodes = &AllNodes;
	vrmlSaver.RootNodes = &RootNodes;

	vrmlSaver.saveLoadedToVrml(outFilePath.c_str());
}

void Scene::geometryFun() {
	for (int i = 0; i < ShapeNodes.size(); i++)
	{
		if (ShapeNodes[i]->geometry == nullptr) continue;
		//vec3 a = ShapeNodes[i]->geometry->getCenterOfGravity();
		//AABB b = ShapeNodes[i]->geometry->getAABB();
		//std::cout << ShapeNodes[i]->geometry->name << std::endl;
		//std::cout << a << std::endl << b.min << std::endl << b.max << std::endl << std::endl;
	}
}
