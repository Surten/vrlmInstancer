#include "sceneManager.h"

SceneManager::SceneManager() : pbrtExporter()  {

}


Scene* SceneManager::combineAllScenesIntoOne() {

	//UNUSED, FRAGMENT OF THE PAST

	//delete combinedScene;
	//Scene* ret = new Scene("combined scene");
	//for(auto scene: scenes)
	//{
	//	TransformNode* tn = new TransformNode(scene->name);
	//	tn->children = scene->RootNodes;
	//	ret->RootNodes.push_back(tn);
	//	ret->geometries.insert(ret->geometries.end(), scene->geometries.begin(), scene->geometries.end());
	//}
	//return ret;
	return nullptr;
}

bool SceneManager::loadScene(const std::string& filePath)
{
	Scene* scene = vrmlParser.parseFile(filePath.c_str());
	for (auto camera : scene->Cameras)
	{
		allCameras.push_back(camera);
	}
	if (scene == nullptr) return false;
	scenes.push_back(scene);
	return true;
}

bool SceneManager::loadTexturedScene(const std::string& filePath)
{
	Scene* scene = vrmlParser.parseFile(filePath.c_str());
	if (scene == nullptr) return false;
	scenesWithTextures.push_back(scene);
	return true;
}

bool SceneManager::saveScene(const std::string& outputFileName, const std::string& sceneName)
{
	Scene* scene = getSceneByName(sceneName);
	if (scene == nullptr) return false;
	vrmlSaver.saveSceneToVrml(outputFileName.c_str(), scene);
	return true;
}

bool SceneManager::saveScene(const std::string& outputFileName, int id)
{
	if (id < 0 || id >= scenes.size()) return false;
	vrmlSaver.saveSceneToVrml(outputFileName.c_str(), scenes.at(id));
	return true;
}

bool SceneManager::writeGeometriesOfScene(const std::string& sceneName)
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

bool SceneManager::instanceGeometry(const std::string& sceneName)
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

void SceneManager::instanceAllGeometry() {
	for (auto scene : scenes) {
		scene->findAndUseIdenticalGeometry();
	}
	for (auto scene : scenesWithTextures) {
		scene->findAndUseIdenticalGeometry();
	}
}

bool SceneManager::copyTextureCoordinatesBetweenScenes(const std::string& fromSceneName, const std::string& toSceneName)
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

void SceneManager::copyTextureCoordinatesFromAllTexturedScenesToAllOtherLoadedScenes()
{
	for (auto scene : scenes) {
		scene->findAndUseSameObjectsFromOtherScenesInThisScene(scenesWithTextures);
	}
}

void SceneManager::copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(Scene* specifiedScene)
{
	specifiedScene->findAndUseSameObjectsFromOtherScenesInThisScene(scenes);
}

void SceneManager::copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(int specifiedSceneId)
{
	scenes.at(specifiedSceneId)->findAndUseSameObjectsFromOtherScenesInThisScene(scenes);
}

void SceneManager::copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(const std::string& specifiedSceneName)
{
	Scene* specifiedScene = getSceneByName(specifiedSceneName);
	specifiedScene->findAndUseSameObjectsFromOtherScenesInThisScene(scenes);
}


void SceneManager::exportAllToPBRT(int cameraIndex) {
	std::string folder("pbrtv4/");
	pbrtExporter.exportScene(scenes, allCameras[cameraIndex], folder, "testHeader", "out.exr");
}

void SceneManager::unifyTextrureCoordScaleOfAllScenes() {
	float desiredTextureScale = getTextureCoordsToObjectCoordsScale();
	for (auto scene : scenes) {
		scene->scaleTextureCoordsForAllObjects(desiredTextureScale);
	}
}

void SceneManager::scaleAllScenesGeometry(float scale) {
	for (auto scene : scenes) {
		scene->scaleSceneGeometry(scale);
	}
}


bool SceneManager::convertSceneSpotLightsToGonioLights(std::string sceneName, std::string referenceSceneName)
{
	Scene* scene = getSceneByName(sceneName);
	if (scene == nullptr) return false;
	Scene* referenceScene = getSceneByName(referenceSceneName);
	if (referenceScene == nullptr) std::cout << "No reference File" << std::endl;
	scene->convertSpotLightsToGonioLights(referenceScene);
	return true;
}

float SceneManager::getTextureCoordsToObjectCoordsScale() {
	int i = 0;
	float scale;
	while ((scale = scenes[0]->geometries[i]->calculateTextureScale()) < 0)
	{
		i++;
	}
	return scale;
}


Scene* SceneManager::getSceneByName(const std::string& name)
{
	Scene* ret = nullptr;
	for (Scene* scene : scenes) {
		if (scene->name == name) {
			ret = scene;
		}
	}
	return ret;
}

