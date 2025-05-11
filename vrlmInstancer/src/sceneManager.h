#pragma once

#include <vector>
#include <string>

#include "scene.h"
#include "pbrtExporter.h"
#include "mitsubaExporter.h"
#include "vrmlParser.h"
#include "vrmlSaver.h"
#include "materials.h"

#include "project.h"
#include "animation.h"


class SceneManager {

public:

	MaterialsFile* materialsFile;

	AnimationInfo* animInfo;
	std::vector<Project*> projects;



public:
	SceneManager();
	~SceneManager();

	Scene* combineAllScenesIntoOne();
	
	bool loadScene(const std::string& filePath);
	bool loadTexturedScene(const std::string& filePath);
	
	bool saveScene(const std::string& outputFileName, const std::string& sceneName);
	bool saveScene(const std::string& outputFileName, int id);

	bool deleteScene(int id);

	bool writeGeometriesOfScene(const std::string& sceneName);
	bool writeGeometriesOfScene(int id);

	bool instanceGeometry(const std::string& sceneName);
	bool instanceGeometry(int id);

	void instanceAllGeometry();
	long getNumLoadedTriangles();

	bool copyTextureCoordinatesBetweenScenes(const std::string& fromSceneName, const std::string& toSceneName);
	bool copyTextureCoordinatesBetweenScenes(int fromSceneId, int toSceneId);

	void copyTextureCoordinatesFromAllTexturedScenesToAllOtherLoadedScenes();
	void copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(Scene* specifiedScene);
	void copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(const std::string& specifiedSceneName);
	void copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(int specifiedSceneId);

	void createDefaultCamera();
	void createDefaultEnviromentalLight(std::string envirometMapFileName);
	void exportAllToPBRT(int cameraIndex, std::string outputHeaderName, std::string outputFolder, std::string outputImageName, std::string integrator,int width, int height, int samples, bool createNewGeometry);
	void exportAllToMitsuba(int cameraIndex, std::string outputHeaderName, std::string outputFolder, std::string integrator, int width, int height, int samples, bool createNewGeometry);

	void unifyTextrureCoordScaleOfAllScenes();
	void scaleAllScenesGeometry(float scale);

	bool convertSceneSpotLightsToGonioLights(std::string sceneName, std::string referenceSceneName);

	void rotateSceneAroundY(int sceneID, float angleDegrees);
	void addProjectToScene(std::string lineCommands, int sceneID);

private:
	std::vector<Scene*> scenesWithTextures;

	std::vector<ViewPointNode*> allCameras;

	PbrtExporter pbrtExporter;
	MitsubaExporter mitsubaExporter;
	VrmlParser vrmlParser;
	VrmlSaver vrmlSaver;
	std::vector<Scene*> scenes;



private:

	void initExportMaterials();
	float getTextureCoordsToObjectCoordsScale();
	Scene* getSceneByName(const std::string& name);
};