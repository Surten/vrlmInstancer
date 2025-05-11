#include "scene.h"

#include <iostream>
#include <iomanip>
#include <algorithm>


Scene::Scene(std::string name) : name(name) {
	
}

Scene::~Scene() {
	for (auto node : AllNodes) {
		delete node;
	}
	for (auto geometry : geometries) {
		delete geometry;
	}
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

void Scene::writeOutGeometries() {
	for (int i = 0; i < geometries.size(); i++)
	{
		vec3 a = geometries[i]->getCenterOfGravity();
		AABB b = geometries[i]->getAABB();

		std::cout << std::left << std::setw(30) << geometries[i]->name;
		std::cout << "diagAABB: " << b.getDiagonal();
		std::cout << "arit to grav: " << geometries[i]->getCenterOfGravity() - b.getArithmeticCenter();
		std::cout << "aabb: min: " << b.min << "  max: " << b.max;
		std::cout << "coords Size: " << geometries[i]->coords.size() << " Indices size: " << geometries[i]->facesPointsIndex.size() << std::endl;
	}
}

void Scene::scaleTextureCoordsForAllObjects(float desiredTextureScale) {
	for (auto geometry : geometries) {

		BaseNode* node = geometry->parent->parent;
		vec3 sceneScale(1.f, 1.f, 1.f);
		while (node != nullptr)
		{
			if (node->type == NodeTypes::Transform)
			{
				sceneScale = static_cast<TransformNode*>(node)->scale * sceneScale;
			}
			node = node->parent;
		}
		geometry->scaleTextureCoords(desiredTextureScale, sceneScale);
	}
}

void Scene::scaleSceneGeometry(float scale)
{
	for (auto& geometry : geometries) {
		geometry->scaleCoords(0.01f);
	}

	for (auto node : AllNodes)
	{
		switch (node->type)
		{
		case NodeTypes::Transform:
		{
			TransformNode* transformNode = static_cast<TransformNode*>(node);
			transformNode->translation.x = transformNode->translation.x * scale;
			transformNode->translation.y = transformNode->translation.y * scale;
			transformNode->translation.z = transformNode->translation.z * scale;
		}
			break;
		case NodeTypes::Light:
		{
			LightNode* lightNode = static_cast<LightNode*>(node);
			lightNode->location.x = lightNode->location.x * scale;
			lightNode->location.y = lightNode->location.y * scale;
			lightNode->location.z = lightNode->location.z * scale;
		}
			break;
		case NodeTypes::ViewPoint:
		{
			ViewPointNode* viewPointNode = static_cast<ViewPointNode*>(node);
			viewPointNode->position.x = viewPointNode->position.x * scale;
			viewPointNode->position.y = viewPointNode->position.y * scale;
			viewPointNode->position.z = viewPointNode->position.z * scale;
		}
			break;
		default:
			break;
		}
	}
	calculateAABB();
}

void Scene::findIdenticalGeometry(std::vector<std::pair<int, int>> & geoPairs) {

	// Traverse all
	for (size_t i = 0; i < geometries.size(); i++)
	{
		// Save all important info of the first Geometry for the comparison
		int nPoints = static_cast<int>(geometries.at(i)->coords.size());
		AABB aabb = geometries[i]->getAABB();
		vec3 centerOfGravity = geometries[i]->getCenterOfGravity();

		vec3 diagonal = aabb.getDiagonal();
		vec3 vectorToGravCenter = centerOfGravity - aabb.getArithmeticCenter();

		// Traverse all remaining geometries (from current to the end)
		for (size_t j = i+1; j < geometries.size(); j++)
		{
			// Save all important info of the second Geometry for the comparison
			int otherNpoints = static_cast<int>(geometries.at(j)->coords.size());
			AABB otherAabb = geometries[j]->getAABB();
			vec3 otherCenterOfGravity = geometries[j]->getCenterOfGravity();

			//Compare geometries
			if (nPoints == otherNpoints && aabb.min.areEqual(otherAabb.min) && aabb.max.areEqual(otherAabb.max) && vectorToGravCenter.areEqual(otherCenterOfGravity - otherAabb.getArithmeticCenter())) {
				//Before adding, we need to check, that geometries[i] will be the source geometry
				// if i is already in some pair as second, it means that both i and j were already 
				// found by some previous geometry, therefore we skip this
				// yes, this can be done more intuitively... We could just find if j is in a pair as second.
				// But I am too far and do not want to break this by revriting it :D
				bool addAsNewConnection = true;
				for (size_t k = 0; k < geoPairs.size(); k++)
				{
					if (geoPairs[k].second == i) {
						addAsNewConnection = false;
					}
				}
				// Add indexes of geometries as a connection
				if(addAsNewConnection)
					geoPairs.push_back(std::pair<int,int>(i,j));
			}
		}
	}
}

void Scene::findAndUseIdenticalGeometry() {
	std::vector<std::pair<int, int>> geoPairs;
	findIdenticalGeometry(geoPairs);
	
	for (size_t i = 0; i < geoPairs.size(); i++)
	{
		static_cast<TransformNode*>(geometries[geoPairs[i].second]->parent->parent)->translation += (geometries[geoPairs[i].second]->getAABB().min - geometries[geoPairs[i].first]->getAABB().min);
		geometries[geoPairs[i].second]->parent->usesOtherGeometry = true;
		geometries[geoPairs[i].second]->parent->geometry = geometries[geoPairs[i].first];
		geometries[geoPairs[i].first]->otherShapeNodesUsingMe.push_back(geometries[geoPairs[i].second]->parent);
		for (size_t j = 0; j < geometries[geoPairs[i].second]->otherShapeNodesUsingMe.size(); j++)
		{
			geometries[geoPairs[i].second]->otherShapeNodesUsingMe[j]->geometry = geometries[geoPairs[i].first];
			geometries[geoPairs[i].first]->otherShapeNodesUsingMe.push_back(geometries[geoPairs[i].second]->otherShapeNodesUsingMe[j]);
		}
	}
	int lastUsedIndex = 0;

	int geometriesBefore = geometries.size(), trianglesBefore = getNumSceneTriangles();
	for (size_t i = 0; i < geometries.size(); i++)
	{
		if (geometries[i]->parent->usesOtherGeometry) {
			delete(geometries[i]);
		}
		else {
			geometries[lastUsedIndex++] = geometries[i];
		}
	}
	geometries.resize(lastUsedIndex);

	int geometriesAfter = geometries.size(), trianglesAfter = getNumSceneTriangles();
	std::cout << "Reduced the number of geometries from " <<geometriesBefore << " to " << geometriesAfter << std::endl;
	std::cout << "Reduced the number of triangles from " <<trianglesBefore << " to " << trianglesAfter << std::endl;

}


void Scene::findAndUseSameObjects(Scene* otherScene) {
	if (this == otherScene) return;

	initShapeNodeTransformMatricies();
	otherScene->initShapeNodeTransformMatricies();

	std::vector<std::pair<Geometry*, Geometry*>> geoPairs;
	float epsilon = 0.001f;
	for (size_t i = 0; i < geometries.size(); i++)
	{
		int nPoints = static_cast<int>(geometries.at(i)->coords.size());
		AABB aabb = geometries[i]->getAABB();
		vec3 centerOfGravity = geometries[i]->getCenterOfGravity();

		//aabb.max = *(geometries[i]->parent->transformToRootMatrix) * aabb.max;
		//aabb.min = *(geometries[i]->parent->transformToRootMatrix) * aabb.min;
		//centerOfGravity = *(geometries[i]->parent->transformToRootMatrix) * centerOfGravity;

		vec3 diagonal = aabb.getDiagonal();
		vec3 vectorToGravCenter = centerOfGravity - aabb.getArithmeticCenter();

		for (size_t j = 0; j < otherScene->geometries.size(); j++)
		{
			int otherNpoints = static_cast<int>(otherScene->geometries.at(j)->coords.size());
			AABB otherAabb = otherScene->geometries[j]->getAABB();
			vec3 otherCenterOfGravity = otherScene->geometries[j]->getCenterOfGravity();

			//otherAabb.max = *(otherScene->geometries[j]->parent->transformToRootMatrix) * otherAabb.max;
			//otherAabb.min = *(otherScene->geometries[j]->parent->transformToRootMatrix) * otherAabb.min;
			//otherCenterOfGravity = *(otherScene->geometries[j]->parent->transformToRootMatrix) * otherCenterOfGravity;

			if (nPoints == otherNpoints && diagonal.areEqual(otherAabb.getDiagonal()) && vectorToGravCenter.areEqual(otherCenterOfGravity - otherAabb.getArithmeticCenter())) {
				geoPairs.push_back(std::pair<Geometry*, Geometry*>(geometries.at(i), otherScene->geometries.at(j)));
			}
		}
	}
	for (size_t i = 0; i < geoPairs.size(); i++)
	{
		//std::cout << geoPairs[i].second->name << " " << geoPairs[i].first->name << std::endl;
		geoPairs[i].first->coords = geoPairs[i].second->coords;
		geoPairs[i].first->facesPointsIndex = geoPairs[i].second->facesPointsIndex;
		geoPairs[i].first->textureCoords = geoPairs[i].second->textureCoords;
		geoPairs[i].first->facesTextureIndex = geoPairs[i].second->facesTextureIndex;
		geoPairs[i].first->parent->textureFilePath = geoPairs[i].second->parent->textureFilePath;
		geoPairs[i].first->parent->textureType = geoPairs[i].second->parent->textureType;
		for (size_t j = 0; j < geoPairs[i].first->otherShapeNodesUsingMe.size(); j++) {
			geoPairs[i].first->otherShapeNodesUsingMe[j]->textureFilePath = geoPairs[i].second->parent->textureFilePath;
			geoPairs[i].first->otherShapeNodesUsingMe[j]->textureType = geoPairs[i].second->parent->textureType;
		}
	}


}

void Scene::findAndUseSameObjectsFromOtherScenesInThisScene(std::vector<Scene*>& scenes) {
	for (auto scene : scenes) {
		if (this == scene) continue;
		findAndUseSameObjects(scene);
	}
}

int Scene::getNumSceneTriangles() 
{
	int count = 0;
	for (Geometry* geometry : geometries)
	{
		count += geometry->facesPointsIndex.size();
	}
	return count;
}

LightNode* Scene::findSameLightsByPosition(LightNode* light, Scene* listOfOtherLights)
{
	for (int i = 0; i < listOfOtherLights->lights.size(); i++)
	{
		if (light->location.areEqual(listOfOtherLights->lights[i]->location))
			return listOfOtherLights->lights[i];
	}
	return nullptr;
}

/// <summary>
/// Converts spotlights to gonio lights, for our purpose, we only implement them using a determined url "IESrjh_ts.ies"
/// </summary>
/// <param name="lightReferences"> if some of the lights have a definition elsewhere </param>
void Scene::convertSpotLightsToGonioLights(Scene* lightReferences) {

	LightNode *referenceLight = nullptr;

	for (int i = 0; i < lights.size(); i++)
	{
		if (referenceLight = findSameLightsByPosition(lights[i], lightReferences))
		{
			lights[i]->url = referenceLight->url;
			lights[i]->intensity = referenceLight->intensity;
		}
		else
		{
			lights[i]->url = "IESrjh_ts.ies";
			lights[i]->intensity = 106.230003f;
		}
		lights[i]->lightType = LightNode::LightType::GONIOLIGHT;
	}
}



void Scene::calculateAABBRecursive(TransformNode* transformNode, Matrix transformMatrix)
{
	Matrix translateMat;
	Matrix rotateMat;
	Matrix scaleMat;
	Matrix scaleOrientationMat;
	vec3 rotationAxis(transformNode->rotation.x, transformNode->rotation.y, transformNode->rotation.z);
	if (transformNode->hasTranslation()) translateMat.mTranslate(transformNode->translation);
	if (transformNode->hasRotation())
		rotateMat.mRotate(rotationAxis, transformNode->rotation.par);
	if (transformNode->hasScaleOrientation())
	{
		Matrix r1;
		Matrix s;
		Matrix r2;
		vec3 axis(transformNode->scaleOrientation.x, transformNode->scaleOrientation.y, transformNode->scaleOrientation.z);
		r1.mRotate(axis, transformNode->scaleOrientation.par);
		s.mScale(transformNode->scale);
		r2.mRotate(axis, -transformNode->scaleOrientation.par);
		scaleMat = r1 * s * r2;
	}
	else if (transformNode->hasScale()) 
		scaleMat.mScale(transformNode->scale);


	transformMatrix = ((transformMatrix * translateMat) * rotateMat) * scaleMat;

	for (auto node : transformNode->children)
	{
		if (node->type == NodeTypes::Transform)
		{
			calculateAABBRecursive(static_cast<TransformNode*>(node), transformMatrix);
		}
		else if (node->type == NodeTypes::Shape)
		{
			ShapeNode* shapeNode = static_cast<ShapeNode*>(node);

			if (shapeNode->geometry == nullptr) continue;

			AABB geometryAABB = shapeNode->geometry->getAABB();

			vec3 min = transformMatrix * geometryAABB.min;
			vec3 max = transformMatrix * geometryAABB.max;

			if (shapeNode->transformFromRootMatrix != nullptr) delete shapeNode->transformFromRootMatrix;
			if (shapeNode->transformToRootMatrix != nullptr) delete shapeNode->transformToRootMatrix;

			shapeNode->transformFromRootMatrix = new Matrix(transformMatrix);
			shapeNode->transformToRootMatrix = new Matrix(transformMatrix.mInverse());
			sceneAABB.uniteWithPoint(min);
			sceneAABB.uniteWithPoint(max);
		}
	}

}

void Scene::calculateAABB()
{
	for (auto node : RootNodes)
	{
		if (node->type == NodeTypes::Transform)
		{
			calculateAABBRecursive(static_cast<TransformNode*>(node), Matrix());
		}
	}
}

AABB& Scene::getSceneAABB()
{
	if (sceneAABB.min.areEqual(vec3()) && sceneAABB.max.areEqual(vec3()))
		calculateAABB();
	return sceneAABB;
	
}


void Scene::initShapeNodeTransformMatricies()
{
	calculateAABB();
}

