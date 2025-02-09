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

bool SceneManager::deleteScene(int id)
{
	if (id < 0 || id >= scenes.size()) return false;
	delete scenes.at(id);
	scenes.erase(scenes.begin() + id);
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

void SceneManager::createDefaultCamera()
{
	ViewPointNode* viewPointNode = new ViewPointNode();
	viewPointNode->name = "DefaultCamera";
	scenes[0]->AllNodes.push_back(viewPointNode);
	scenes[0]->Cameras.push_back(viewPointNode);
	allCameras.push_back(viewPointNode);
	scenes[0]->RootNodes.push_back(viewPointNode);
	viewPointNode->type = NodeTypes::ViewPoint;
	viewPointNode->nodeDepth = 0;

	AABB retAABB = scenes[0]->geometries[0]->getAABB();
	for (auto geometry : scenes[0]->geometries)
	{
		AABB toAdd = geometry->getAABB();
		std::cout << "from " << toAdd.min << " to " << toAdd.max << std::endl;
		retAABB.uniteWithAABB(toAdd);
	}
	vec3 cameraPosition(-188, 193.5, 212.2);
	cameraPosition.normalize();
	std::cout << "Lenght " << retAABB.getDiagonal().len() << std::endl;
	cameraPosition = cameraPosition * retAABB.getDiagonal().len() * 0.75;
	
	for (auto node : scenes[0]->RootNodes)
	{
		if (node->type == NodeTypes::Transform)
		{
			static_cast<TransformNode*>(node)->translation.setVector(0, 0, 0);
		}
	}

	viewPointNode->position = cameraPosition;
	viewPointNode->position = viewPointNode->position * 2;
	viewPointNode->orientation[0] = 0.5299;
	viewPointNode->orientation[1] = 0.8192;
	viewPointNode->orientation[2] = 0.2195;
	viewPointNode->orientation[3] = -0.9363;
	viewPointNode->fieldOfView = 0.9;
	viewPointNode->description = "Generated Default Camera";

	std::cout << "Created default camera for scene with index 0" << std::endl;
}

void SceneManager::createDefaultEnviromentalLight(std::string envirometMapFileName)
{
	LightNode* lightNode = new LightNode();
	lightNode->name = "DefaultEnviromentalLight";
	scenes[0]->AllNodes.push_back(lightNode);
	scenes[0]->lights.push_back(lightNode);
	scenes[0]->RootNodes.push_back(lightNode);
	lightNode->type = NodeTypes::Light;
	lightNode->lightType = LightNode::LightType::ENVIROMENTAL_LIGHT;
	lightNode->url = envirometMapFileName;


	std::cout << "Created default enviromental light for scene with index 0, because no lights were detected" << std::endl;
}

void SceneManager::exportAllToPBRT(int cameraIndex, std::string name, std::string outputImageFormat) {
	if (cameraIndex == -1 || allCameras.size() == 0)
	{
		createDefaultCamera();
		cameraIndex = 0;
		
	}
	bool hasLights = false;
	for (auto scene : scenes)
	{
		if (scene->lights.size() > 0)
		{
			hasLights = true;
			break;
		}
	}
	if (!hasLights) createDefaultEnviromentalLight("sky.exr");

	std::string outputFolder("pbrtv4/textured/");
	std::string outputHeaderName = "headers/header_" + name;
	std::string outputImageName = "output/" + name + "_render." + outputImageFormat;
	pbrtExporter.exportScene(scenes, allCameras[cameraIndex], outputFolder, outputHeaderName, outputImageName);
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

