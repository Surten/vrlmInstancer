#include "sceneManager.h"

SceneManager::SceneManager() : pbrtExporter()  {

}


Scene* SceneManager::combineAllScenesIntoOne() {
	//delete combinedScene;
	Scene* ret = new Scene("combined scene");
	for(auto scene: scenes)
	{
		TransformNode* tn = new TransformNode(scene->name);
		tn->children = scene->RootNodes;
		ret->RootNodes.push_back(tn);
		ret->geometries.insert(ret->geometries.end(), scene->geometries.begin(), scene->geometries.end());
	}
	return ret;
}

bool SceneManager::loadScene(std::string filePath)
{
	Scene* scene = vrmlParser.parseFile(filePath.c_str());
	if (scene == nullptr) return false;
	scenes.push_back(scene);
	return true;
}

bool SceneManager::saveScene(std::string outputFileName, std::string sceneName)
{
	Scene* scene = getSceneByName(sceneName);
	if (scene == nullptr) return false;
	vrmlSaver.saveSceneToVrml(outputFileName.c_str(), scene);
	return true;
}

bool SceneManager::saveScene(std::string outputFileName, int id)
{
	if (id < 0 || id >= scenes.size()) return false;
	vrmlSaver.saveSceneToVrml(outputFileName.c_str(), scenes.at(id));
	return true;
}

bool SceneManager::writeGeometriesOfScene(std::string sceneName)
{
	Scene* scene = getSceneByName(sceneName);
	if (scene == nullptr) return false;
	scene->writeOutGeometries();
	return true;

}

bool SceneManager::writeGeometriesOfScene(int id)
{
	if (id < 0 || id >= scenes.size()) return false;
	scenes.at(id)->writeOutGeometries();
	return true;

}

bool SceneManager::instanceGeometry(std::string sceneName)
{
	Scene* scene = getSceneByName(sceneName);
	if (scene == nullptr) return false;
	scene->findAndUseIdenticalGeometry();
	return true;

}

bool SceneManager::instanceGeometry(int id)
{
	if (id < 0 || id >= scenes.size()) return false;
	scenes.at(id)->findAndUseIdenticalGeometry();
	return true;

}

bool SceneManager::copyTextureCoordinatesBetweenScenes(std::string fromSceneName, std::string toSceneName)
{
	Scene* sceneFrom = getSceneByName(fromSceneName);
	if (sceneFrom == nullptr) return false;
	Scene* sceneTo = getSceneByName(toSceneName);
	if (sceneTo == nullptr) return false;
	sceneFrom->findAndUseSameObjects(sceneTo);
	return true;
}

bool SceneManager::copyTextureCoordinatesBetweenScenes(int fromSceneId, int toSceneId)
{
	if (fromSceneId < 0 || fromSceneId >= scenes.size() || toSceneId < 0 || toSceneId >= scenes.size()) return false;
	scenes.at(fromSceneId)->findAndUseSameObjects(scenes.at(toSceneId));
	return true;
}

void SceneManager::copyTextureCoordinatesFromAllMyScenesToExternalScene(Scene* externalScene)
{
	externalScene->findAndUseSameObjectsFromOtherScenesInThisScene(scenes);
}

void SceneManager::exportAllToPBRT() {
	Scene* combinedScene = combineAllScenesIntoOne();
	pbrtExporter.exportScene(combinedScene);
}

Scene* SceneManager::getSceneByName(std::string name)
{
	Scene* ret = nullptr;
	for (Scene* scene : scenes) {
		if (scene->name == name) {
			ret = scene;
		}
	}
	return ret;
}
