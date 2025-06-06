#include "sceneManager.h"

#include "geometryModify.h"

SceneManager::SceneManager() : animInfo(new AnimationInfo()), pbrtExporter(animInfo), mitsubaExporter(animInfo)
{
	materialsFile = new MaterialsFile();
}
SceneManager::~SceneManager() {
	delete materialsFile;
	delete animInfo;
	for(Project* p : projects)
		delete p;
	for (Scene* s : scenes)
		delete s;
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
	if (scene == nullptr) return false;
	for (auto camera : scene->Cameras)
	{
		allCameras.push_back(camera);
	}
	// Check for door objects
	CheckListDoors(scene->RootNodes);

	// Check for the shutters objects
	CheckListShutters(scene->RootNodes);

	// Check for the windows objects
	CheckListWindows(scene->RootNodes);

	for (LightNode* lightNode : scene->lights)
	{
		lightNode->correctName();
	}

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
	scene->initShapeNodeTransformMatricies();
	scene->findAndUseIdenticalGeometry();
	return true;

}

bool SceneManager::instanceGeometry(int id)
{
	if (id < 0 || id >= scenes.size()) return false;
	scenes.at(id)->initShapeNodeTransformMatricies();
	scenes.at(id)->findAndUseIdenticalGeometry();
	return true;

}

void SceneManager::instanceAllGeometry() {
	for (auto scene : scenes) {
		scene->initShapeNodeTransformMatricies();
		scene->findAndUseIdenticalGeometry();
	}
	for (auto scene : scenesWithTextures) {
		scene->initShapeNodeTransformMatricies();
		scene->findAndUseIdenticalGeometry();
	}
}

long SceneManager::getNumLoadedTriangles()
{
	long count = 0;
	for (Scene* scene : scenes)
	{
		count += scene->getNumSceneTriangles();
	}
	return count;
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
		if (geometry->name.find("Bearer") != std::string::npos)
			continue;
		retAABB.uniteWithAABB(toAdd);
	}
	vec3 cameraPosition(-200, 200, 200);
	cameraPosition.normalize();
	float length = retAABB.getDiagonal().len();
	if(length > 0.001)
		cameraPosition = cameraPosition * length;
	
	cameraPosition = cameraPosition + (retAABB.getArithmeticCenter() - vec3());

	for (auto node : scenes[0]->RootNodes)
	{
		if (node->type == NodeTypes::Transform)
		{
			static_cast<TransformNode*>(node)->translation.setVector(0, 0, 0);
		}
	}

	viewPointNode->position = cameraPosition;

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

void SceneManager::exportAllToPBRT(int cameraIndex, std::string outputHeaderName, std::string outputFolder, std::string outputImageFormat, std::string integrator, int width, int height, int samples, bool createNewGeometry) {
	bool createdCustomFloor = false;
	ViewPointNode* camera;
	if (cameraIndex == -1 || allCameras.size() == 0)
	{
		camera = nullptr;
	}
	else
	{
		camera = allCameras[cameraIndex];
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


	for (Project* p : projects)
		p->executeActions();

	AnimationInfo* animInfoCopy = new AnimationInfo(*animInfo);

	initExportMaterials();
	pbrtExporter.exportScene(scenes, camera, outputFolder, outputHeaderName, outputImageFormat, createNewGeometry, materialsFile, integrator, width, height, samples, animInfoCopy);
}

void SceneManager::exportAllToMitsuba(int cameraIndex, std::string mainSceneName, std::string outputFolder, std::string integrator, int width, int height, int samples, bool createNewGeometry) {
	ViewPointNode* camera;
	if (cameraIndex == -1 || allCameras.size() == 0)
	{
		camera = nullptr;
	}
	else
	{
		camera = allCameras[cameraIndex];
	}
	bool hasLights = false;
	for (Scene* scene : scenes)
	{
		if (scene->lights.size() > 0)
		{
			hasLights = true;
			break;
		}
	}
	if (!hasLights) createDefaultEnviromentalLight("envmap.hdr");

	for (Project* p : projects)
		p->executeActions();

	AnimationInfo* animInfoCopy = new AnimationInfo(*animInfo);

	initExportMaterials();
	mitsubaExporter.exportScene(scenes, camera, mainSceneName, outputFolder, integrator, width, height, samples, createNewGeometry, materialsFile, animInfoCopy);
}

void SceneManager::unifyTextrureCoordScaleOfAllScenes() {
	float desiredTextureScale = getTextureCoordsToObjectCoordsScale();
	for (Scene* scene : scenes) {
		scene->scaleTextureCoordsForAllObjects(desiredTextureScale);
	}
	for (Scene* scene : scenesWithTextures) {
		scene->scaleTextureCoordsForAllObjects(desiredTextureScale);
	}
}

void SceneManager::scaleAllScenesGeometry(float scale) {
	for (Scene* scene : scenes) {
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

#define DEG_TO_RAD(angle)   ((angle) / 57.29577951308f)

void SceneManager::rotateSceneAroundY(int sceneID, float angleDegrees)
{
	for (auto root_node : scenes.at(sceneID)->RootNodes)
	{
		if (root_node->type == NodeTypes::Transform)
		{
			float rotation[4] = { 0, 1, 0, DEG_TO_RAD(angleDegrees) };
			static_cast<TransformNode*>(root_node)->setRotation(rotation);
		}
	}
}

float SceneManager::getTextureCoordsToObjectCoordsScale() {
	double scale = 0.0;
	bool use_relative_scale = false;
	if (use_relative_scale) {
		//int i = 0;
		//int geoCount = 0;
		//for (auto scene : scenes)
		//{
		//	for (auto geometry : scene->geometries)
		//	{
		//		float geoScale = geometry->calculateTextureScale();
		//		if (geoScale > 0)
		//		{
		//			scale += geoScale;
		//			geoCount++;
		//		}
		//	}
		//}

		//scale = scale / geoCount;
	}
	else
	{
		scale = 100.0;
	}
	return (float)scale;
}

void SceneManager::addProjectToScene(std::string lineCommands, int sceneID)
{
	Project* project = nullptr;
	for (Project* p : projects)
	{
		if (p->scene == scenes[sceneID])
		{
			project = p;
		}
	}
	if (project == nullptr)
	{
		project = new Project(animInfo, scenes[sceneID]);
		project->initProject();
		projects.push_back(project);
	}

	project->readProject(lineCommands);
}

void SceneManager::initExportMaterials()
{
	for (auto scene : scenes)
	{
		materialsFile->AddSceneMaterials(scene);
	}
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

