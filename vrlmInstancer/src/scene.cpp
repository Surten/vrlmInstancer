#include "scene.h"
#include <iomanip>

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
		std::cout << "center: " << a;
		std::cout << "aabb: "   << b.min;
		std::cout << b.max << std::endl;
		std::cout << "coords Size: " << geometries[i]->coords.size() << " Indices size: " << geometries[i]->facesPointsIndex.size() << std::endl;
	}
}

void Scene::findDuplicateGeometry() {

	float epsilon = 0.001;
	for (size_t i = 0; i < geometries.size(); i++)
	{
		int nPoints = (int)geometries.at(i)->coords.size();
		AABB aabb = geometries[i]->getAABB();
		vec3 diagonal = aabb.getDiagonal();
		vec3 gravCenter = geometries[i]->getCenterOfGravity();
		vec3 aritCenter = aabb.getArithmeticCenter();
		vec3 vectorToGravCenter = (gravCenter - aritCenter);
		for (size_t j = 0; j < geometries.size(); j++)
		{
			if (i == j) continue;
			int otherNpoints = geometries.at(j)->coords.size();
			AABB otherAabb = geometries[j]->getAABB();
			vec3 differenceOfDiagonals = diagonal - otherAabb.getDiagonal();
			vec3 otherGravCenter = geometries[i]->getCenterOfGravity();
			vec3 otherAritCenter = otherAabb.getArithmeticCenter();
			vec3 differenceOfGravCenters = vectorToGravCenter - (otherGravCenter - otherAritCenter);
			if (nPoints == otherNpoints && differenceOfDiagonals.len2() < epsilon && differenceOfGravCenters.len2() < epsilon) {
				std::cout << i << " " << j << std::endl;
			}
		}
	}

}

void Scene::findSimilarObjects(Scene * otherScene) {
	std::vector<std::pair<Geometry*, Geometry*>> geoPairs;
	float epsilon = 0.001;
	for (size_t i = 0; i < geometries.size(); i++)
	{
		int nPoints = (int)geometries.at(i)->coords.size();
		AABB aabb = geometries[i]->getAABB();
		vec3 diagonal = aabb.getDiagonal();
		vec3 gravCenter = geometries[i]->getCenterOfGravity();
		vec3 aritCenter = aabb.getArithmeticCenter();
		vec3 vectorToGravCenter = (gravCenter - aritCenter);
		for (size_t j = 0; j < otherScene->geometries.size(); j++)
		{
			int otherNpoints = otherScene->geometries.at(j)->coords.size();
			AABB otherAabb = otherScene->geometries[j]->getAABB();
			vec3 differenceOfDiagonals = diagonal - otherAabb.getDiagonal();
			vec3 otherGravCenter = otherScene->geometries[i]->getCenterOfGravity();
			vec3 otherAritCenter = otherAabb.getArithmeticCenter();
			vec3 differenceOfGravCenters = vectorToGravCenter - (otherGravCenter - otherAritCenter);
			if (nPoints == otherNpoints && differenceOfDiagonals.len2() < epsilon && differenceOfGravCenters.len2() < epsilon) {
				std::cout << i << " " << j << std::endl;
				geoPairs.push_back(std::pair<Geometry*, Geometry*>(geometries.at(i), otherScene->geometries.at(j)));
			}
		}
	}

}

