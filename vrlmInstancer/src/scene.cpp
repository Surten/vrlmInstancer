#include "scene.h"
#include <iomanip>
#include <algorithm>

void Scene::loadSceneFromVrmlFile(std::string filePath) {
	vrmlParser.parseFile(filePath.c_str());
}

void Scene::saveSceneToVrmlFile(std::string outFilePath) {
	vrmlSaver.saveLoadedToVrml(outFilePath.c_str());
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

void Scene::findDuplicateGeometry(std::vector<std::pair<int, int>> & geoPairs) {

	for (size_t i = 0; i < geometries.size(); i++)
	{
		int nPoints = static_cast<int>(geometries.at(i)->coords.size());
		AABB aabb = geometries[i]->getAABB();
		vec3 diagonal = aabb.getDiagonal();
		vec3 vectorToGravCenter = geometries[i]->getCenterOfGravity() - aabb.getArithmeticCenter();
		for (size_t j = i+1; j < geometries.size(); j++)
		{
			int otherNpoints = static_cast<int>(geometries.at(j)->coords.size());
			AABB otherAabb = geometries[j]->getAABB();
			//if (i == 13) {
			//	std::cout << "breakPoint" << std::endl;
			//}
			if (nPoints == otherNpoints && diagonal.areEqual(otherAabb.getDiagonal()) && vectorToGravCenter.areEqual(geometries[j]->getCenterOfGravity() - otherAabb.getArithmeticCenter())) {
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

void Scene::findAndUseDuplicateGeometry() {
	std::vector<std::pair<int, int>> geoPairs;
	findDuplicateGeometry(geoPairs);
	for (size_t i = 0; i < geoPairs.size(); i++)
	{
		std::cout << geoPairs[i].first << " " << geoPairs[i].second << std::endl;
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

}


void Scene::findSimilarObjects(Scene * otherScene) {
	std::vector<std::pair<Geometry*, Geometry*>> geoPairs;
	float epsilon = 0.001f;
	for (size_t i = 0; i < geometries.size(); i++)
	{
		int nPoints = static_cast<int>(geometries.at(i)->coords.size());
		AABB aabb = geometries[i]->getAABB();
		vec3 diagonal = aabb.getDiagonal();
		vec3 vectorToGravCenter = (geometries[i]->getCenterOfGravity() - aabb.getArithmeticCenter());
		for (size_t j = 0; j < otherScene->geometries.size(); j++)
		{
			int otherNpoints = static_cast<int>(otherScene->geometries.at(j)->coords.size());
			AABB otherAabb = otherScene->geometries[j]->getAABB();
			if (nPoints == otherNpoints && diagonal.areEqual(otherAabb.getDiagonal()) && vectorToGravCenter.areEqual(otherScene->geometries[j]->getCenterOfGravity() - otherAabb.getArithmeticCenter())) {
				geoPairs.push_back(std::pair<Geometry*, Geometry*>(geometries.at(i), otherScene->geometries.at(j)));
			}
		}
	}
	for (size_t i = 0; i < geoPairs.size(); i++)
	{
		std::cout << geoPairs[i].first->name << " " << geoPairs[i].second->name << std::endl;
		geoPairs[i].second->textureCoords = geoPairs[i].first->textureCoords;
		geoPairs[i].second->facesTextureIndex = geoPairs[i].first->facesTextureIndex;
	}


}
