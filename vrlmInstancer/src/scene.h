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
	bool hasAnimatedCamera = false;



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

	/// <summary>
	/// Counts the number of triangles of all geometries in the scene
	/// </summary>
	/// <returns>The number of triangles</returns>
	int getNumSceneTriangles();


	/// <summary>
	/// Looks into the provided Scene for gonioLights and tries to substitute the Lights in the current
	/// Scene with lights from the provided scene, paired by location. If not present, creates a default GonioLight(may be wrong default)
	/// </summary>
	/// <param name="lightReferences"></param>
	void convertSpotLightsToGonioLights(Scene* lightReferences);

	/// <summary>
	/// Returns scene AABB, IF SCENE IS MODIFIED, THE AABB IS NOT RECALCULATED BY DEFAULT,
	/// NEEDS TO BE UPDATED MANUALLY BY CALLING calculateAABB() or initShapeNodeTransformMatricies() EXPLICITLY
	/// </summary>
	/// <returns>The scen AABB</returns>
	AABB& getSceneAABB();

	/// <summary>
	/// Calculates the Scene AABB and in the process, saves/updates the Transform Matricies from World space to Model space for each Shape Node
	/// </summary>
	void initShapeNodeTransformMatricies();

private:
	AABB sceneAABB;

private:

	/// <summary>
	/// Based on AABB, center of gravity and the number of verticies,
	/// paires up geometries that are supposedly the same, to be later used for instancing
	/// </summary>
	/// <param name="geoPairs">The output pairs of geometris, should have no duplicates</param>
	void findIdenticalGeometry(std::vector<std::pair<int, int>>& geoPairs);

	LightNode* findSameLightsByPosition(LightNode* light1, Scene* listOfOtherLights);
	void calculateAABBRecursive(TransformNode* transformNode, Matrix transformMatrix);
	void calculateAABB();
};