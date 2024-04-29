#pragma once
#include <string>
#include <vector>
#include "dataStructs.h"

enum NodeTypes { Transform, Shape };

class BaseNode {
public:


	NodeTypes type;
	std::string name;
	BaseNode* parent;
	int nodeDepth;

public:
	BaseNode(std::string name, NodeTypes type) {
		this->name = name;
		this->type = type;
		parent = nullptr;
	}
protected:

private:
};


class TransformNode : public BaseNode {
public:
	TransformNode(std::string name) : BaseNode(name, Transform) {
		translation[0] = 0; translation[1] = 0; translation[2] = 0;
		rotation[0] = 0; rotation[1] = 0; rotation[2] = 0; rotation[3] = 0;
		scale[0] = 1; scale[1] = 1; scale[2] = 1;
		scaleOrientation[0] = 0; scaleOrientation[1] = 0; scaleOrientation[2] = 0; scaleOrientation[3] = 0;
	}
	TransformNode(std::string name, float* translation, float* rotation, float* scale, float* scaleOrientation)
		: BaseNode(name, Transform)
	{
		this->name = name;
		std::memcpy(this->translation, translation, sizeof(float) * 3);
		std::memcpy(this->rotation, rotation, sizeof(float) * 4);
		std::memcpy(this->scale, scale, sizeof(float) * 3);
		std::memcpy(this->scaleOrientation, scaleOrientation, sizeof(float) * 4);
		
	}
	void setTranslation(float* translation) {
		std::memcpy(this->translation, translation, sizeof(float) * 3);
	}
	void setRotation(float* rotation) {
		std::memcpy(this->rotation, rotation, sizeof(float) * 4);
	}
	void setScale(float* scale) {
		std::memcpy(this->scale, scale, sizeof(float) * 3);
	}
	void setScaleOrientation(float* scaleOrientation) {
		std::memcpy(this->scaleOrientation, scaleOrientation, sizeof(float) * 4);
	}

	bool hasTranslation() {
		float eps = 0.0001;
		if (std::abs(translation[0]) < eps && std::abs(translation[1]) < eps && std::abs(translation[2]) < eps)
			return false;
		return true;
	}
	bool hasRotation() {
		float eps = 0.0001;
		if (std::abs(rotation[0]) < eps && std::abs(rotation[1]) < eps && std::abs(rotation[2]) < eps && std::abs(rotation[3]) < eps)
			return false;
		return true;
	}
	bool hasScale() {
		float eps = 0.0001;
		if (std::abs(scale[0] - 1.0f) < eps && std::abs(scale[1] - 1.0f) < eps && std::abs(scale[2] - 1.0f) < eps)
			return false;
		return true;
	}
	bool hasScaleOrientation() {
		float eps = 0.0001;
		if (std::abs(scaleOrientation[0]) < eps && std::abs(scaleOrientation[1]) < eps && std::abs(scaleOrientation[2]) < eps && std::abs(scaleOrientation[3]) < eps)
			return false;
		return true;
	}
	
public:
	float translation[3];
	float rotation[4];
	float scale[3];
	float scaleOrientation[4];

	std::vector<BaseNode*> children;
};

class ShapeNode : public BaseNode {
public:
	ShapeNode() : BaseNode("", Shape) {
		geometry = nullptr;
	}

public:


	Geometry *geometry;
	Material material;
	std::string textureFilePath;
	std::string textureType;
	bool usesOtherGeometry = false;
private:

};