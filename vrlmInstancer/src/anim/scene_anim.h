/**
@file scene.h
Hlavickovy soubor s definicemi trid popisujicich scenu.

@author Copyright (C) 2008 Jiri Drahokoupil, modified by Ondrej Linda (2009)
*/

#ifndef _SCENE_H_
#define _SCENE_H_

#include "animation.h"
#include "dataStructs.h"



// Prototypy nekterych trid z tohoto souboru
/*class Vector3D;
class Vector4D;
class Node;
class NodeTransform;
class NodeShape;
class NodeTSensor;
class Geometry;
class Material;*/



/// Zakladni typy uzlu hierarchie VRML(omezeno pouze na ty, ktere generuje 3DS Max!)
typedef enum {_TSENSOR, _TRANSFORM, _SHAPE, _GEOMETRY, _LIGHT, _CAMERA, _NULL} NodeType;
/// Specifikation of object placement in certain floor
typedef enum {_U_FLOOR, _G_FLOOR, _1_FLOOR, _2_FLOOR, _3_FLOOR, _4_FLOOR, _5_FLOOR, _6_FLOOR, _7_FLOOR, _8_FLOOR, _9_FLOOR, _10_FLOOR, _FLOOR_NDEF} ObjectFloor;
///Specifikaction of object placement in certain section
typedef enum {_1_SECTION, _2_SECTION, _3_SECTION, _4_SECTION, _5_SECTION, _6_SECTION, _7_SECTION, _8_SECTION, _9_SECTION, _10_SECTION, _SECTION_NDEF} ObjectSection;
/// Specification of the object of the object - for determining doors, windows, ...
typedef enum {_TYPE_DOOR, _TYPE_WINDOW, _TYPE_WINDOW_OPEN, _TYPE_WINDOW_MOVE, _TYPE_WINDOW_JOINT, _TYPE_SHUTTERS_NODE, _TYPE_SHUTTERS_MOVE,
			_TYPE_SHUTTER_TOP, _TYPE_SHUTTER, _OBJECT_NDEF} ObjectType;
/// Specification of the what object info type the object is. It is stored in the objectInfo class
typedef enum {_TYPE_INFO_DOOR, _TYPE_INFO_WINDOW, _TYPE_INFO_SHUTTER, _TYPE_INFO_NDEF} ObjectInfoType;
/// Specification of the object construction type
typedef enum {_TYPE_CONSTRUCT_1, _TYPE_CONSTRUCT_2, _TYPE_CONSTRUCT_3, _TYPE_CONSTRUCT_4, _TYPE_CONSTRUCT_5, _TYPE_CONSTRUCT_6, _TYPE_CONSTRUCT_7, 
	_TYPE_CONSTRUCT_8, _TYPE_CONSTRUCT_9, _TYPE_CONSTRUCT_10, _TYPE_CONSTRUCT_11, _TYPE_CONSTRUCT_12, _TYPE_CONSTRUCT_13, _TYPE_CONSTRUCT_14,
	_TYPE_CONSTRUCT_15, _TYPE_CONSTRUCT_16, _TYPE_CONSTRUCT_17, _TYPE_CONSTRUCT_18, _TYPE_CONSTRUCT_19, _TYPE_CONSTRUCT_20, _TYPE_CONSTRUCT_21,
	_TYPE_CONSTRUCT_22, _TYPE_CONSTRUCT_23, _TYPE_CONSTRUCT_24, _TYPE_CONSTRUCT_25, _TYPE_CONSTRUCT_26, _TYPE_CONSTRUCT_27,
	_TYPE_CONSTRUCT_NDEF} ObjectConstructType;

float linearInterp(float a, float b, float x);
float cosineInterp(float a, float b, float x);
float cubicInterp(float v0, float v1, float v2, float v3, float x);
float KBSplineInterp(float v0, float v1, float v2, float v3, float x, float t, float c, float b);


/// Class storing the information general about the object. It is a parent class for the door, window and shutter objects.
class ObjectInfo{
	/// Name of the object node
	char* name;
	/// Floor where the object is located 
	ObjectFloor objectFloor;
	/// Section where the object is located
	ObjectSection objectSection;
	/// Type of the info object
	ObjectInfoType objectInfoType;
	/// Construction type of particular object
	ObjectConstructType objectConstructType;
	/// Has the object geometry been modified
	bool mHasModified;
	/// Flag determining if the object has been animated or not
	bool mHasAnimated;
	/// Pointer to a list with animation checkpoints
	AnimationList * animList;
public:
	/// Constructors
	ObjectInfo() : name(0),objectFloor(_FLOOR_NDEF), objectSection(_SECTION_NDEF), objectInfoType(_TYPE_INFO_NDEF), objectConstructType(_TYPE_CONSTRUCT_NDEF),
		mHasModified(false), mHasAnimated(false), animList(nullptr) {}
	ObjectInfo(ObjectInfoType oType) : name(0),objectFloor(_FLOOR_NDEF), objectSection(_SECTION_NDEF), objectInfoType(oType), 
		objectConstructType(_TYPE_CONSTRUCT_NDEF), mHasModified(false), mHasAnimated(false), animList(nullptr) {}
	/// Sets the floor number, where object is located
	void setObjectFloor(ObjectFloor of) { objectFloor = of; }
	/// Returns the floor number, where the object is located
	ObjectFloor retObjectFloor(void) { return objectFloor; }
	/// Sets the section number, where the object is located
	void setObjectSection(ObjectSection os) { objectSection = os; }
	/// Returns section number, where the object is located
	ObjectSection retObjectSection(void) { return objectSection; }
	/// Sets the type of the info object
	void setObjectInfoType(ObjectInfoType oit) {objectInfoType = oit; }
	/// Returns the type of the info object
	ObjectInfoType retObjectInfoType(void) {return objectInfoType; }
	/// Sets the construction type of the object
	void setObjectConstructType(ObjectConstructType oct) { objectConstructType = oct; }
	/// Returns the construction type of the object
	ObjectConstructType retObjectConstructType(void) { return objectConstructType; }	
	/// Returns the name of the info object
	char * retObjectName(void) {return name;}
	/// Sets the name of the info object
	void setObjectName(const char * on);
	/// Returns the floor number
	int retObjectFloorNumber(void);
	/// Sets the object floor number
	void setObjectFloorNumber(int floorNum);
	/// Returns the section number
	int retObjectSectionNumber(void);
	/// Sets the section number
	void setObjectSectionNumber(int sectionNum);
	/// Returns the object construction type number
	int retObjectConstructTypeNumber(void);
	/// Sets the object constrution type number
	void setObjectConstructTypeNumber(int typeNum);
	/// Decodes the name of the node into the floor, section and type of the door, it creates a new name of the door
	void correctObjectName(const char * name);
	/// Returns if the object geometry has been modified
	bool retHasModified(void) {return mHasModified; }
	/// Sets the object geometry modification flag
	void setHasModified(bool r) {mHasModified = r;}	
	/// Sets the animation flag
	void setHasAnimated(bool flag) {mHasAnimated = flag; }
	/// Returns the animation flag
	bool retHasAnimated(void) {return mHasAnimated; }
	/// Sets the animation list
	void setAnimationList(AnimationList * list) {animList = list; }
	/// Returns the animation list
	AnimationList * retAnimationList(void) {return animList; }
	/// Initializes the animationList object
	void initAnimList(void) {animList = new AnimationList(); }
	/// Clears the animtion lists objects
	void clearAnimList(void);
};


/// Class storing the information about the door object, it will be assigned to a Transform node that is identified as a door object
/// It inherits from the parent ObjectInfo class
class DoorInfo : public ObjectInfo {
	
	/// The amount of rotation
	int mRotateDeg;
	/// The axis of the rotation (the coorinates of the joints)
	vec3 axis;
	/// The amount of handle opening
	int mHandleRate;
	
public:
	/// Constructors
	DoorInfo();
	/// Returns the rotation angle of the door [degrees]
	int retRotateDeg(void) {return mRotateDeg; }
	/// Sets the rotation angle of the door [degress]
	void setRotateDeg(int deg) {mRotateDeg = deg; }
	/// Sets the axis position 
	void setAxis(vec3 a) {axis = a; }
	/// Sets the axis position
	vec3 retAxis(void) {return axis; }
	/// Returns the current opening of the door based on the provided animation time and properties
	float retCurrentOpening(AnimationInfo* animInfo);
	/// Returns the current opening rate for the door handle
	float retCurrentHandleRate(AnimationInfo* animInfo);
	/// Copies the provided animation list with doors
	void copyAnimList(AnimationList * list);
	/// Sets the rate of handle opening
	void setHandleRate(int rate) {mHandleRate = rate;}
	/// Returns the rate of handle opening
	int retHandleRate(void) {return mHandleRate;}
};

/// Class storing the information about the shutter object, it will be assigned to a Transform node that is identified as a moving shutter object
/// It inherits from the parent ObjectInfo class
class ShutterInfo : public ObjectInfo {
	
	/// The amount of scaling
	int mScaleRate;
	/// Y translation offset of the top of the shutter
	float mTopOffset;
	/// The shutter rotate opening rate
	int mRotateRate;
		
public:
	/// Constructors
	ShutterInfo();
	/// Returns the scaling rate of the shutter  [%]
	int retScaleRate(void) {return mScaleRate; }
	/// Sets the the scaling ration of the shutter [%]
	void setScaleRate(int deg) {mScaleRate = deg; }
	/// Returns the y translation offset of the top of the shutter
	float retTopOffset(void) {return mTopOffset; }
	/// Sets the y translation offset of the top of the shutter
	void setTopOffset(float top) { mTopOffset = top; }
	/// Returns the current opening of the window shutter based on the provided animation time and properties
	float retCurrentOpening(AnimationInfo* animInfo);
	/// Copies the provided animation list with doors
	void copyAnimList(AnimationList * list);
	/// Sets the shutter rotate opening rate
	void setRotateRate(int rate) {mRotateRate = rate; }
	/// Returns the shutter rotate rate
	int retRotateRate(void) {return mRotateRate; }
	/// Returns the current rotation rate of the window shutter based on the provided animation time and properties
	float retCurrentRotation(AnimationInfo* animInfo);

};

/// Class storing the information about the window object, it will be assigned to a Transform node that is identified as a moving window object
/// It inherits from the parent ObjectInfo class
class WindowInfo : public ObjectInfo{
	
	/// The amount of rotation
	int mRotateDeg;	
	/// The amount of handle opening
	int mHandleRate;
	/// The axis of the rotation (the coorinates of the window joints)
	vec3 axis;
	/// type of the opening - horizontal - 0, vertical - 1
	int mOpenType;
	
public:
	/// Constructors
	WindowInfo();
	/// Returns the rotation rate of the Window  [degrees]
	int retRotateDeg(void) {return mRotateDeg; }
	/// Sets the the rotation of the Window [degrees]
	void setRotateDeg(int deg) {mRotateDeg = deg; }	
	/// Sets the axis position 
	void setAxis(vec3 a) {axis = a; }
	/// Sets the axis position
	vec3 retAxis(void) {return axis; }
	/// Retunrs the current opening of the window window based on the provided animation time and properties
	float retCurrentOpening(AnimationInfo* animInfo);
	/// Retunrs the current opening of the window handle
	float retCurrentHandleRate(AnimationInfo* animInfo);
	/// Copies the provided animation list with windows
	void copyAnimList(AnimationList * list);
	/// Sets the rate of handle opening
	void setHandleRate(int rate) {mHandleRate = rate;}
	/// Returns the rate of handle opening
	int retHandleRate(void) {return mHandleRate;}
	/// Sets the opening type
	void setOpenType(int type) {mOpenType = type; }
	/// Returns the opening type
	int retOpenType(void) {return mOpenType;}
	/// Returns the current opening type during the animation
	int retCurrentOpenType(AnimationInfo* animInfo);
};



#endif
