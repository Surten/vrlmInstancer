#pragma once
#include <string>
#include <vector>
#include "scene_anim.h"
#include "dataStructs.h"
#include "matData.h"
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
	TransformNode(std::string name) : BaseNode(name, NodeTypes::Transform), translation(), scale(1.0f, 1.0f, 1.0f),
		rotation(1,0,0,0), scaleOrientation(1,0,0,0), objectInfo(nullptr), objectType(_OBJECT_NDEF)
	{
	}
	TransformNode(std::string name, float* translation, float* rotation, float* scale, float* scaleOrientation)
		: BaseNode(name, NodeTypes::Transform), translation(translation[0], translation[1], translation[2]), scale(scale[0], scale[1], scale[2]),
		rotation(rotation[0], rotation[1], rotation[2], rotation[3]), scaleOrientation(scaleOrientation[0], scaleOrientation[1], scaleOrientation[2], scaleOrientation[3]),
		objectInfo(nullptr), objectType(_OBJECT_NDEF)
	{
		this->name = name;
	}
	void setTranslation(float* translation) {
		this->translation.setVector(translation[0], translation[1], translation[2]);
	}
	void setRotation(float* rotation) {
		this->rotation = vec4(rotation[0], rotation[1], rotation[2], rotation[3]);
	}
	void setScale(float* scale) {
		this->scale.setVector(scale[0], scale[1], scale[2]);
	}
	void setScaleOrientation(float* scaleOrientation) {
		this->scaleOrientation = vec4(scaleOrientation[0], scaleOrientation[1], scaleOrientation[2], scaleOrientation[3]);
	}

	bool hasTranslation() {
		if (translation.areEqual(vec3()))
			return false;
		return true;
	}
	bool hasRotation() {
		float eps = 0.0001f;
		if (std::abs(rotation.par) < eps)
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
		if (std::abs(scaleOrientation.par) < eps)
			return false;
		return true;
	}



	// ANIM----------
	/// Sets the type of the object
	void setObjectType(ObjectType ot) { objectType = ot; }
	/// Returns the type of the object
	ObjectType retObjectType(void) { return objectType; }
	/// Returns pointer to the object info object
	ObjectInfo* retObjectInfo(void) { return objectInfo; }
	/// Traverse the list of children, find the transform node that contains the moving parts of the door object and set the correct rotation
	void setRotationDoor(void);
	/// Traverse the list of children, sets the joint axis for rotating the door
	void setDoorAxis(void);
	/// Traverse the list of children, find the transform node that contains the moving parts of the window object and set the correct rotation
	void setRotationWindow(void);
	/// Traverse the list of children, sets the joint axis for rotating the window
	void setWindowAxis(void);
	/// Initializes the objectInfo pointer to the Door info object
	void initDoorInfo(void) { objectInfo = new DoorInfo(); }
	/// Initializes the objectInfo pointer to the Window info object
	void initWindowInfo(void) { objectInfo = new WindowInfo(); }
	/// Initializes the objectInfo pointer to the Shutter info object
	void initShutterInfo(void) { objectInfo = new ShutterInfo(); }
	//END ANIM
	
public:
	vec3 translation;
	vec4 rotation;
	vec3 scale;
	vec4 scaleOrientation;

	std::vector<BaseNode*> children;

	/// Type of the transform node object
	ObjectType objectType;
	/// Pointer to the objectInfo object, if this node is not an recognized object, this pointer is set to NULL
	ObjectInfo* objectInfo;
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
		transformFromRootMatrix = nullptr;
		transformToRootMatrix = nullptr;
		material = nullptr;
		exportMaterial = nullptr;
	}

	~ShapeNode()
	{
		delete transformFromRootMatrix;
		delete transformToRootMatrix;
		delete material;
	}
public:
	Geometry* geometry;
	Material* material;
	Mat* exportMaterial;
	Matrix* transformFromRootMatrix; // only initialized after a special call to a Scene's method
	Matrix* transformToRootMatrix; // only initialized after a special call to a Scene's method
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


	/// Determines if the camera is on - if it will be included in the output VRML file
	bool m_bIsOn = false;
	/// Determines if the camera has been animated
	bool m_bHasAnimated = false;
	/// Pointer to a list with animation checkpoints
	AnimationList* animList;
	/// Flag determining if the camera has been selected for PBRT - only one can be selected
	bool m_bPBRTSelect = false;

	ViewPointNode() : BaseNode("", NodeTypes::ViewPoint), position(), orientation(), fieldOfView(0), description(""), animList(nullptr){}

	//----------------------------------------------------------------------------------------------
	void computeLookAt(AnimationInfo* animInfo, vec3& loc, vec3& dir, vec3& up) const;

	/// Initializes the animationList object
	void initAnimList(void) { animList = new AnimationList(); }
	/// Clears the animtion lists objects
	void clearAnimList(void);
	/// Copies the provided animation list with doors
	void copyAnimList(AnimationList* list);
	/// Sets the PBRT selection flag
	void setPBRTSelect(bool select) { m_bPBRTSelect = select; }
	/// Returns the PBRT selection flag
	bool retPBRTSelect(void) { return m_bPBRTSelect; }
	/// Returns the current position of the camera during animation
	vec3 retCurrentPosition(AnimationInfo* animInfo) const;
	/// Returns the current orientation of the camera during animation
	vec4 retCurrentOrientation(AnimationInfo* animInfo) const;

};