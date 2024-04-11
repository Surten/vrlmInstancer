#pragma once
#include <string>
#include <vector>

class BaseNode {
public:

public:

private:
	//properties of all nodes: name, enum Type
	std::string name;
	std::vector<BaseNode*> children;
private:
	BaseNode();
};