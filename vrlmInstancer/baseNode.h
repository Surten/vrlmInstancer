#pragma once
#include <string>
#include <vector>

class BaseNode {
public:
	enum NodeTypes {Transform, Shape};
public:

protected:
	std::string name;


	std::vector<BaseNode*> children;
	BaseNode* parent;

private:
};

class TransformNode : BaseNode {
public:
	TransformNode(std::string name, float* translation, float* rotation, float* scale, float* scaleOrientation);
	
public:
	float translation[3];
	float rotation[4];
	float scale[3];
	float scaleOrientation[4];

};