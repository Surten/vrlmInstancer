#pragma once
#include <string>
#include <vector>
#include "dataStructs.h"
#include "matrix.h"

// enum to remember the different type of nodes inside base node,
// so that they can be easily static_casted
enum class NodeTypes { Transform, Shape, Light, ViewPoint };

/// <summary>
/// Base node class to be derived from in the favor of polymorpism
/// </summary>
class BaseNode {
public:

	NodeTypes type;
	std::string name;
	BaseNode* parent;	// for now, parent nodes are always Transform nodes, but in the favor of extensibility we use BaseNode
	int nodeDepth;		// how deep is the node in the structure when loaded from a file

public:
	BaseNode(std::string name, NodeTypes type) {
		this->name = name;
		this->type = type;
		parent = nullptr;
		nodeDepth = -1;
	}
protected:

private:
};

/// <summary>
/// Class representing the Transform node of a VRML file
/// Contains translation, rotation, scale and scaleOrientation
/// Has array of children
/// Has basic self-explanatory methods to operate the properties
/// </summary>
class TransformNode : public BaseNode {
public:
	TransformNode(std::string name) : BaseNode(name, NodeTypes::Transform), translation(), scale(1.0f, 1.0f, 1.0f){
		rotation[0] = 0; rotation[1] = 0; rotation[2] = 0; rotation[3] = 0;
		scaleOrientation[0] = 0; scaleOrientation[1] = 0; scaleOrientation[2] = 0; scaleOrientation[3] = 0;
	}
	TransformNode(std::string name, float* translation, float* rotation, float* scale, float* scaleOrientation)
		: BaseNode(name, NodeTypes::Transform), translation(translation[0], translation[1], translation[2]), scale(scale[0], scale[1], scale[2])
	{
		this->name = name;
		std::memcpy(this->rotation, rotation, sizeof(float) * 4);
		std::memcpy(this->scaleOrientation, scaleOrientation, sizeof(float) * 4);
		
	}
	void setTranslation(float* translation) {
		this->translation.setVector(translation[0], translation[1], translation[2]);
	}
	void setRotation(float* rotation) {
		std::memcpy(this->rotation, rotation, sizeof(float) * 4);
	}
	void setScale(float* scale) {
		this->scale.setVector(scale[0], scale[1], scale[2]);
	}
	void setScaleOrientation(float* scaleOrientation) {
		std::memcpy(this->scaleOrientation, scaleOrientation, sizeof(float) * 4);
	}

	bool hasTranslation() {
		if (translation.areEqual(vec3()))
			return false;
		return true;
	}
	bool hasRotation() {
		float eps = 0.0001f;
		if (std::abs(rotation[0]) < eps && std::abs(rotation[1]) < eps && std::abs(rotation[2]) < eps && std::abs(rotation[3]) < eps)
			return false;
		return true;
	}
	bool hasScale() {
		if (scale.areEqual(vec3(1.0f,1.0f,1.0f)))
			return false;
		return true;
	}
	bool hasScaleOrientation() {
		float eps = 0.0001f;
		if (std::abs(scaleOrientation[0]) < eps && std::abs(scaleOrientation[1]) < eps && std::abs(scaleOrientation[2]) < eps && std::abs(scaleOrientation[3]) < eps)
			return false;
		return true;
	}
	
public:
	vec3 translation;
	float rotation[4];
	vec3 scale;
	float scaleOrientation[4];

	std::vector<BaseNode*> children;
};

/// <summary>
/// Class representing the Shape node of a VRML file
/// Contains Material, pointer to geometry and texture file path
/// Knows if it is responsible for it's geometry or is just referencing it
/// meaning that some other Shape node is reponsible for it
/// </summary>
class ShapeNode : public BaseNode {
public:
	ShapeNode() : BaseNode("", NodeTypes::Shape) {
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

/// <summary>
/// Class representing the Light node of a VRML file
/// For now, we only use SpotLights, so the properties are for spotlight only
/// </summary>
class LightNode : public BaseNode {
public:
	float intensity;
	vec3 color;
	vec3 location;
	vec3 direction;
	float cutOffAngle;
	float beamWidth;
	bool on;
	float radius;
	std::string url;

	enum class LightType {
		SPOTLIGHT,
		GONIOLIGHT,
		ENVIROMENTAL_LIGHT	// only default for pbrt, parsing not supported
	};
	LightType lightType;
	


	LightNode() : BaseNode("", NodeTypes::Light), intensity(0), color(), location(), direction(),
		cutOffAngle(0), beamWidth(0), on(false), radius(0), lightType(LightType::SPOTLIGHT) {}

private:
};

/// <summary>
/// Class representing the ViewPoint node of a VRML file
/// </summary>
class ViewPointNode : public BaseNode {
public:
	
	vec3 position;
	float orientation[4];
	float fieldOfView;
	std::string description;

	ViewPointNode() : BaseNode("", NodeTypes::ViewPoint), position(), orientation(), fieldOfView(0), description("") {}

	//----------------------------------------------------------------------------------------------
	void computeLookAt(vec3& loc, vec3& dir, vec3& up) const
		//----------------------------------------------------------------------------------------------
	{
		loc = position;


		vec3 vec(orientation[0], orientation[1], orientation[2]);

		// Normalizuju
		vec.normalize();

		// Matice identity
		Matrix mat;
		// Vytvorim rotacni matici kolem obecneho vektoru definovaneho pomoci 'dir' a uhlu v 'orientation'
		mat = mat.mRotate(vec, orientation[3]);

		// Urcim smer pohledu kamery
		vec = mat * vec3(0.0, 0.0, -1.0);
		// Urcim up-vector kamery
		up = mat * vec3(0.0, 1.0, 0.0);

		// Zbyva urcit bod, na ktery bude kamera centrovana. Pokud znam smerovy vektor
		// pohledu kamery, staci vzit libovolne cislo a dosadit za parametr 't' v parametricke
		// rci primky.
		const float t = 10.0f;

		// Get the current position of the camera
		vec3 currentPosition = position;
		dir.x = currentPosition.x + vec.x * t;
		dir.y = currentPosition.y + vec.y * t;
		dir.z = currentPosition.z + vec.z * t;
	}

};