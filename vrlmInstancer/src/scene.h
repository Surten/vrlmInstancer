#pragma once
#include <vector>
#include "baseNode.h"


/// <summary>
/// Class containing the entirety of one loaded file
/// Stores the nodes in different arrays, often duplicating references in favor of better manipulation with the data
/// We use name usually to remember the file name of the file loaded
/// </summary>
class Scene {
public:

	std::string name;
	std::vector<BaseNode*> RootNodes;
	std::vector<Geometry*> geometries;
	std::vector<BaseNode*> AllNodes;
	std::vector<ShapeNode*> ShapeNodes;
	std::vector<LightNode*> lights;
	std::vector<ViewPointNode*> Cameras;

	bool isAnimated = false;



public:
	Scene(std::string name);
	~Scene();


	// just a helper method for testing different functionalities, we use it during debugging
	void geometryFun();

	// write the list of geometries with some information about them to the standard output
	void writeOutGeometries();

	void scaleTextureCoordsForAllObjects(float desiredTextureScale);
	void scaleSceneGeometry(float scale);

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
	void findAndUseSameObjectsFromOtherScenesInThisScene(std::vector<Scene*>& scenes);

	void findShapeNodesByTheirMaterialDiffuseComponentAndReplaceTheirTexturePath(vec3 diffuseComponent, std::string texturePath);

	void convertSpotLightsToGonioLights(Scene* lightReferences);

	AABB& getSceneAABB();

	void initShapeNodeTransformMatricies();

private:
	AABB sceneAABB;

private:
	void findIdenticalGeometry(std::vector<std::pair<int, int>>& geoPairs);

	LightNode* findSameLightsByPosition(LightNode* light1, Scene* listOfOtherLights);
	void calculateAABBRecursive(TransformNode* transformNode, Matrix transformMatrix);
	void calculateAABB();
};