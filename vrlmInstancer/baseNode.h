#pragma once
#include <string>
#include <vector>
#include "dataStructs.h"

class BaseNode {
public:
	enum NodeTypes {Transform, Shape};

	NodeTypes type;
	std::string name;
	BaseNode* parent;

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

	}

public:

	Geometry geometry;
	Material material;
	std::string textureFilePath;
	std::string textureType;
private:

};