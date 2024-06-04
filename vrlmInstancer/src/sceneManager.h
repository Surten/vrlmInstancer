#pragma once

#include <vector>
#include <string>

#include "scene.h"
#include "pbrtExporter.h"
#include "vrmlParser.h"
#include "vrmlSaver.h"

class SceneManager {
public:

public:
	SceneManager();

	Scene* combineAllScenesIntoOne();
	
	bool loadScene(std::string filePath);
	
	bool saveScene(std::string outputFileName, std::string sceneName);
	bool saveScene(std::string outputFileName, int id);

	bool writeGeometriesOfScene(std::string sceneName);
	bool writeGeometriesOfScene(int id);

	bool instanceGeometry(std::string sceneName);
	bool instanceGeometry(int id);

	bool copyTextureCoordinatesBetweenScenes(std::string fromSceneName, std::string toSceneName);
	bool copyTextureCoordinatesBetweenScenes(int fromSceneId, int toSceneId);

	void copyTextureCoordinatesFromAllMyScenesToExternalScene(Scene* externalScene);

	void exportAllToPBRT();

private:
	std::vector<Scene*> scenes;

	PbrtExporter pbrtExporter;
	VrmlParser vrmlParser;
	VrmlSaver vrmlSaver;


private:

	Scene* getSceneByName(std::string name);
};