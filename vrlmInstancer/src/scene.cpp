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

	for (size_t i = 0; i < geometries.size(); i++)
	{
		int nPoints = static_cast<int>(geometries.at(i)->coords.size());
		AABB aabb = geometries[i]->getAABB();
		vec3 centerOfGravity = geometries[i]->getCenterOfGravity();

		aabb.max = *(geometries[i]->parent->transformToRootMatrix) * aabb.max;
		aabb.min = *(geometries[i]->parent->transformToRootMatrix) * aabb.min;
		centerOfGravity = *(geometries[i]->parent->transformToRootMatrix) * centerOfGravity;

		vec3 diagonal = aabb.getDiagonal();
		vec3 vectorToGravCenter = centerOfGravity - aabb.getArithmeticCenter();

		for (size_t j = i+1; j < geometries.size(); j++)
		{
			int otherNpoints = static_cast<int>(geometries.at(j)->coords.size());
			AABB otherAabb = geometries[j]->getAABB();
			vec3 otherCenterOfGravity = geometries[j]->getCenterOfGravity();

			otherAabb.max = *(geometries[j]->parent->transformToRootMatrix) * otherAabb.max;
			otherAabb.min = *(geometries[j]->parent->transformToRootMatrix) * otherAabb.min;
			otherCenterOfGravity = *(geometries[j]->parent->transformToRootMatrix) * otherCenterOfGravity;

			if (nPoints == otherNpoints && diagonal.areEqual(otherAabb.getDiagonal()) && vectorToGravCenter.areEqual(otherCenterOfGravity - otherAabb.getArithmeticCenter())) {
				bool addAsNewConnection = true;
				for (size_t k = 0; k < geoPairs.size(); k++)
				{
					if (geoPairs[k].second == i) {
						addAsNewConnection = false;
					}
				}
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
	std::cout << "Reduced the number of geometries from " << geometries.size();
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
	std::cout << " to " << geometries.size() << std::endl;

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

		aabb.max = *(geometries[i]->parent->transformToRootMatrix) * aabb.max;
		aabb.min = *(geometries[i]->parent->transformToRootMatrix) * aabb.min;
		centerOfGravity = *(geometries[i]->parent->transformToRootMatrix) * centerOfGravity;

		vec3 diagonal = aabb.getDiagonal();
		vec3 vectorToGravCenter = centerOfGravity - aabb.getArithmeticCenter();

		for (size_t j = 0; j < otherScene->geometries.size(); j++)
		{
			int otherNpoints = static_cast<int>(otherScene->geometries.at(j)->coords.size());
			AABB otherAabb = otherScene->geometries[j]->getAABB();
			vec3 otherCenterOfGravity = otherScene->geometries[j]->getCenterOfGravity();

			otherAabb.max = *(otherScene->geometries[j]->parent->transformToRootMatrix) * otherAabb.max;
			otherAabb.min = *(otherScene->geometries[j]->parent->transformToRootMatrix) * otherAabb.min;
			otherCenterOfGravity = *(otherScene->geometries[j]->parent->transformToRootMatrix) * otherCenterOfGravity;

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

/// <summary>
/// WIP, maybe will not be finished
/// </summary>
/// <param name="diffuseComponent"></param>
/// <param name="texturePath"></param>
void Scene::findShapeNodesByTheirMaterialDiffuseComponentAndReplaceTheirTexturePath(vec3 diffuseComponent, std::string texturePath) {

	for (auto shapeNode : ShapeNodes) {
		if (shapeNode->material->compareDiffuseColor(diffuseComponent)) {
			shapeNode->textureFilePath = texturePath;
			if (shapeNode->geometry->textureCoords.size() == 0) {
				std::cout << "we are missing textures on geometry " << shapeNode->geometry->name << std::endl;
			}
		}
	}
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
	vec3 rotationAxis(transformNode->rotation[0], transformNode->rotation[1], transformNode->rotation[2]);
	if (transformNode->hasTranslation()) translateMat.mTranslate(transformNode->translation);
	if (transformNode->hasRotation())
		rotateMat.mRotate(rotationAxis, transformNode->rotation[3]);
	if (transformNode->hasScale()) scaleMat.mScale(transformNode->scale);

	transformMatrix = transformMatrix * translateMat * rotateMat * scaleMat;

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

