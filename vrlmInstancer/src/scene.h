#pragma once
#include <vector>

#include "vrlmParser.h"
#include "vrmlSaver.h"

/// <summary>
/// Class containing the entirety of one loaded file
/// Stores the nodes in different arrays, often duplicating references in favor of better manipulation with the data
/// We use name usually to remember the file name of the file loaded
/// </summary>
class Scene {

public:
	Scene(std::string name) : name(name), vrmlParser(&AllNodes, &RootNodes, &ShapeNodes, &geometries, &lights), vrmlSaver(&AllNodes, &RootNodes, &ShapeNodes, &geometries, &lights){}
	~Scene();

	// loads a VRML file on the file path into the scene
	bool loadSceneFromVrmlFile(std::string filePath);

	// saves the VRML file into the outFilePath location
	void saveSceneToVrmlFile(std::string outFilePath);

	// just a helper method for testing different functionalities, we use it during debugging
	void geometryFun();

	// write the list of geometries with some information about them to the standard output
	void writeOutGeometries();

	/// <summary>
	/// Iterates over the geometries of this scene and finds identical ones
	/// based on three different properties, deleting one of the identical 
	/// geometries and referencing the other in the parent Shape node in the process
	/// </summary>
	void findAndUseIdenticalGeometry();

	/// <summary>
	/// Compares the geometries of two different scenes based on three properties
	/// If the geometries compare identical, it copies the texture coordinates from otherScene to this scene
	/// </summary>
	void findAndUseSameObjects(Scene* otherScene);

	/// <summary>
	/// Compares the geometries of this scene with all other loaded scenes based on three properties
	/// If the geometries compare identical, it copies the texture coordinates
	/// from the other scenes to this scene.
	/// </summary>
	void findAndUseSameObjectsFromOtherScenesInThisScene(std::vector<Scene*> scenes);

	std::string name;

private:
	VrmlParser vrmlParser;
	VrmlSaver vrmlSaver;

	std::vector<BaseNode*> AllNodes;
	std::vector<BaseNode*> RootNodes;
	std::vector<ShapeNode*> ShapeNodes;
	std::vector<Geometry*> geometries;
	std::vector<LightNode*> lights;

private:
	void findIdenticalGeometry(std::vector<std::pair<int, int>>& geoPairs);
};