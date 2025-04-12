/**
@file project.h
Declaration of datastructures and functions for project save/load

@author Copyright (C) 2009 Ondrej Linda
*/

#ifndef __PROJECT_H__
#define __PROJECT_H__

#include "animation.h"
#include "scene.h"
#include <string>

/// Enumeration of action nodes - door, windows, shutters, lights, cameras
typedef enum { ACTION_DOOR, ACTION_WINDOW, ACTION_SHUTTER, ACTION_LIGHT, ACTION_LIGHT_SWITCH, ACTION_LIGHT_TYPE, ACTION_CAMERA, ACTION_NONE } ActionType;

/// Enumeration of node applications types - single object, all section, all floor, all
typedef enum { APPLY_SINGLE, APPLY_SECTION, APPLY_FLOOR, APPLY_ALL } ActionApplicationMode;

/// Enumeration of light type node - set EXR Texture, set Color, set Radiance
typedef enum { SET_EXR, SET_COLOR, SET_RADIANCE } LightTypeMode;

/// Enumeration of the output action data - VRML or PBRT, Single or Animated
typedef enum { VRML_SINGLE_OUT, VRML_ANIM_OUT, PBRT_SINGLE_OUT, PBRT_ANIM_OUT } OutputActionData;

/// Enumeration of the output action type - Individiaul, Separated Main, Separated Geom, Add Static Geom, Add Animated Geom, Add Animated Camera,
/// Update Renderer, Add Lights, Duplicate Static Main, Add Output File Name
typedef enum { OUT_INDIVIDUAL, OUT_SEPARATE_MAIN, OUT_SEPARATE_GEOM, OUT_ADD_STATIC_GEOM, OUT_ADD_ANIM_GEOM, OUT_CAMERA, OUT_UPDATE_RENDER,
	OUT_ADD_LIGHTS, OUT_DUPLICATE, OUT_ADD_OUTPUT_FILE} OutputActionType;

/// Class storing the parent node for individual actions
class ActionNode{

	/// Index of the object
	int objectIndex;
	/// String with a message of this node - for debugging
	std::string message;
	/// Type of the action node
	ActionType actionType;
	/// The application mode
	ActionApplicationMode appMode;
	/// Pointer to the next node
	ActionNode * next;

public:
	/// Constructors
	ActionNode();

	/// Sets the object index number
	void setObjectIndex(int index) {this->objectIndex = index;}
	/// Returns the object index number
	int retObjectIndex(void){return this->objectIndex;}
	/// Sets the message of the node
	void setMessage(std::string mess) {this->message = mess;}
	/// Returns the message
	std::string retMessage(void) {return this->message;}
	/// Returns the pointer to the next node
	ActionNode * retNext(void){return this->next;}
	/// Sets the pointer to the next node
	void setNext(ActionNode * _next) {this->next = _next;}
	/// Sets the action node type
	void setActionType(ActionType type) {this->actionType = type;}
	/// Returns the action node type
	ActionType retActionType(void){return this->actionType;}
	/// Sets tha application mode of the door object
	void setAppMode(int i);
	/// Returns the application mode of the door object
	ActionApplicationMode retAppMode(void) {return this->appMode;}
	/// Returns the application mode number
	int retAppModeNum(void);

	/// Writes the message in the action node to the given output stream
	void writeNode(std::ostream & out);	
};


// Action node for the doors
/// It inherits from the parent ActionNode class
class ActionNodeDoor : public ActionNode{

	/// The door open rate
	int m_iOpenRate;
	/// The door handle open rate
	int m_iHandleRate;
	/// Animation flag
	bool m_bAnimFlag;
	/// Interpolation mode
	int m_iInterpolation;
	/// Pointer to the animation list of the specified door object
	AnimationList * animList;
	
public:
	/// Constructor
	ActionNodeDoor();

	/// Sets the door open rate
	void setDoorOpenRate(int rate) {this->m_iOpenRate = rate;}
	/// Returns the door open rate
	int retDoorOpenRate(void) {return this->m_iOpenRate;}
	/// Sets the door handle open rate
	void setDoorHandleRate(int rate){this->m_iHandleRate = rate;}
	/// Returns the door handle open rate
	int retDoorHandleRate(void) {return this->m_iHandleRate;}
	/// Sets the animation flag
	void setAnimFlag(bool flag) {this->m_bAnimFlag = flag;}
	/// Returns the animaton flag
	bool retAnimFlag(void) {return this->m_bAnimFlag;}
	/// Sets the interpolation mode
	void setInterpolation(int interp) {this->m_iInterpolation = interp;}
	/// Returns the interpolation mode
	int retInterpolation(void) {return this->m_iInterpolation;}
	/// Sets the animation list pointer
	void setAnimList(AnimationList * list) {this->animList = list;}
	/// Returns the animation list pointer
	AnimationList * retAnimList(void) {return this->animList;}

	/// Writes the door action node into the given output stream
	void writeDoorNode(std::ostream &out);

	/// Parses the given string and extracts individual parameters
	void parseInput(std::string input);

	/// Executes this action node
	void executeNode(Scene* scene);
};


// Action node for windows
/// It inherits from the parent ActionNode class
class ActionNodeWindow : public ActionNode{

	/// The window open rate
	int m_iOpenRate;
	/// The window handle open rate
	int m_iHandleRate;
	/// Opening mode of the window
	int m_iOpenMode;
	/// Animation flag
	bool m_bAnimFlag;
	/// Interpolation mode
	int m_iInterpolation;

	/// Pointer to the animation list of the specified window object
	AnimationList * animList;
	
public:
	/// Constructor
	ActionNodeWindow();
	
	/// Sets the window open rate
	void setWindowOpenRate(int rate) {this->m_iOpenRate = rate;}
	/// Returns the window open rate
	int retWindowOpenRate(void) {return this->m_iOpenRate;}
	/// Sets the window handle open rate
	void setWindowHandleRate(int rate){this->m_iHandleRate = rate;}
	/// Returns the window handle open rate
	int retWindowHandleRate(void) {return this->m_iHandleRate;}
	/// Sets the window opening mode
	void setWindowOpenMode(int mode){this->m_iOpenMode = mode;}
	/// Returns the window open mode
	int retWindowOpenMode(void) {return this->m_iOpenMode;}
	/// Sets the animation flag
	void setAnimFlag(bool flag) {this->m_bAnimFlag = flag;}
	/// Returns the animaton flag
	bool retAnimFlag(void) {return this->m_bAnimFlag;}
	/// Sets the interpolation mode
	void setInterpolation(int interp) {this->m_iInterpolation = interp;}
	/// Returns the interpolation mode
	int retInterpolation(void) {return this->m_iInterpolation;}
	/// Sets the animation list pointer
	void setAnimList(AnimationList * list) {this->animList = list;}
	/// Returns the animation list pointer
	AnimationList * retAnimList(void) {return this->animList;}

	/// Writes the window action node into the given output stream
	void writeWindowNode(std::ostream & out);

	/// Parses the given string and extracts individual parameters
	void parseInput(std::string input);

	/// Executes this action node
	void executeNode(Scene* scene);
};



// Action node for the shutters
/// It inherits from the parent ActionNode class
class ActionNodeShutter : public ActionNode{

	/// The shutter scale rate
	int m_iScaleRate;
	/// The shutter rotate rate
	int m_iRotateRate;
	/// Animation flag
	bool m_bAnimFlag;
	/// Interpolation mode
	int m_iInterpolation;
	/// Pointer to the animation list of the specified shutter object
	AnimationList * animList;
	
public:
	/// Constructor
	ActionNodeShutter();

	/// Sets the shutter scale rate
	void setShutterScaleRate(int rate) {this->m_iScaleRate = rate;}
	/// Returns the shutter scale rate
	int retShutterScaleRate(void) {return this->m_iScaleRate;}
	/// Sets the shutter rotate rate
	void setShutterRotateRate(int rate){this->m_iRotateRate = rate;}
	/// Returns the shutter rotate rate
	int retShutterRotateRate(void) {return this->m_iRotateRate;}
	/// Sets the animation flag
	void setAnimFlag(bool flag) {this->m_bAnimFlag = flag;}
	/// Returns the animaton flag
	bool retAnimFlag(void) {return this->m_bAnimFlag;}
	/// Sets the interpolation mode
	void setInterpolation(int interp) {this->m_iInterpolation = interp;}
	/// Returns the interpolation mode
	int retInterpolation(void) {return this->m_iInterpolation;}
	/// Sets the animation list pointer
	void setAnimList(AnimationList * list) {this->animList = list;}
	/// Returns the animation list pointer
	AnimationList * retAnimList(void) {return this->animList;}

	/// Writes the shutter action node into the given output stream
	void writeShutterNode(std::ofstream & out);

	/// Parses the given string and extracts individual parameters
	void parseInput(std::string input);

	/// Executes this action node
	void executeNode(Scene* scene);
};


// Action node for the cameras
/// It inherits from the parent ActionNode class
class ActionNodeCamera : public ActionNode{

	/// Camera on
	bool m_bOn;
	/// PBRT Selected
	bool m_bPBRTSelect;
	/// Animation flag
	bool m_bAnimFlag;
	/// Interpolation mode
	int m_iInterpolation;
	/// Pointer to the animation list of the specified light object
	AnimationList * animList;
	
public:
	/// Constructor
	ActionNodeCamera();

	/// Sets the camera on flag
	void setOn(bool flag) {this->m_bOn = flag;}
	/// Returns the camera on flag
	bool retOn(void){return this->m_bOn;}
	/// Sets the PBRT select flag
	void setPBRTSelect(bool flag) {this->m_bPBRTSelect = flag;}
	/// Returns the PBRT select flag
	bool retPBRTSelect(void) {return this->m_bPBRTSelect;}
	/// Sets the animation flag
	void setAnimFlag(bool flag) {this->m_bAnimFlag = flag;}
	/// Returns the animaton flag
	bool retAnimFlag(void) {return this->m_bAnimFlag;}
	/// Sets the interpolation mode
	void setInterpolation(int interp) {this->m_iInterpolation = interp;}
	/// Returns the interpolation mode
	int retInterpolation(void) {return this->m_iInterpolation;}
	/// Sets the animation list pointer
	void setAnimList(AnimationList * list) {this->animList = list;}
	/// Returns the animation list pointer
	AnimationList * retAnimList(void) {return this->animList;}

	/// Writes the camera action node into the given output stream
	void writeCameraNode(std::ofstream & out);

	/// Parses the given string and extracts individual parameters
	void parseInput(std::string input);

	/// Executes this action node
	void executeNode(Scene* scene);
};



///Class storing a linked list of actions performed by the user
class ActionList{
	/// Number of action nodes in the action list
	int numNodes;
	/// head of the list
	ActionNode * head;
	/// last node in the list
	ActionNode * last;

public:
	/// Constructors
	ActionList();
	/// returns the head of the list
	ActionNode * retHead(void) {return this->head; }
	/// Inserts node into the list
	void insertNode(ActionNode * node);	
	/// returns the number of nodes in the list
	int retNumNodes(void ) {return this->numNodes;}
};

/// Class storing the information about the global animation settings
class Project{

	/// Has the project been initialized
	bool m_bInitialized;

	/// The filepath + file name of the project
	std::string fileName;

	/// The file path to the dataset for this project
	std::string dataFile;

	/// Parameters for determining if the currently opened data set is the same as the one used for the project
	/// Number of all nodes
	int m_iNumberAllNodes;
	/// Number of base nodes
	int m_iNumberBaseNodes;
	/// Number of Lights
	int m_iNumberLights;
	/// Number of Cameras
	int m_iNumberCameras;
	/// Number of Doors
	int m_iNumberDoors;
	/// Number of Windows
	int m_iNumberWindows;
	/// Number of Shutters
	int m_iNumberShutters;

	/// Pointer to the ActionList object
	ActionList * list;

	AnimationInfo* animInfo;

	Scene* scene;

public:
	/// Constructors
	Project(AnimationInfo* animInfo) : m_bInitialized(false), fileName(""), dataFile(""), m_iNumberAllNodes(0), m_iNumberBaseNodes(0), m_iNumberLights(0), 
		m_iNumberCameras(0), m_iNumberDoors(0), m_iNumberWindows(0), m_iNumberShutters(0)
	{
		this->animInfo = animInfo;
		list = nullptr;
		scene = nullptr;
	}

	/// Set the initialzed flag
	void setInitialized(bool flag) {this->m_bInitialized = flag;}
	/// Return the initialized flag
	bool retInitialized(void) {return this->m_bInitialized;}
	/// Sets the project filename
	void setProjectName(std::string fileName) {this->fileName = fileName;}
	/// Returns the project filename
	std::string retProjectName(void) {return this->fileName;}
	/// Sets the dataset filename
	void setDatasetName(std::string dataName) {this->dataFile = dataName;}
	/// Returns the dataset filename
	std::string retDatasetName(void) {return this->dataFile;}
	/// Sets the number of all nodes
	void setNumAllNodes(int num) {this->m_iNumberAllNodes = num;}
	/// Returns the number of all nodes
	int retNumAllNodes(void) {return this->m_iNumberAllNodes;}
	/// Sets the number of base nodes
	void setNumBaseNodes(int num) {this->m_iNumberBaseNodes = num;}
	/// Returns the number of base nodes
	int retNumBaseNodes(void) {return this->m_iNumberBaseNodes;}
	/// Sets the number of cameras
	void setNumCameras(int num) {this->m_iNumberCameras = num;}
	/// Returns the number of cameras
	int retNumCameras(void) {return this->m_iNumberCameras;}
	/// Sets the number of lights
	void setNumLights(int num) {this->m_iNumberLights = num;}
	/// Returns the number of lights
	int retNumLights(void) {return this->m_iNumberLights;}
	/// Sets the number of doors
	void setNumDoors(int num) {this->m_iNumberDoors = num;}
	/// Returns the number of doors
	int retNumDoors(void) {return this->m_iNumberDoors;}
	/// Sets the number of Windows
	void setNumWindows(int num) {this->m_iNumberWindows = num;}
	/// Returns the number of windows
	int retNumWindows(void) {return this->m_iNumberWindows;}
	/// Sets the number of Shutters
	void setNumShutters(int num) {this->m_iNumberShutters = num;}
	/// Returns the number of shutters
	int retNumShutters(void) {return this->m_iNumberShutters;}

	// Initializes the project data structure - used when new project is created or when new dataset is loaded
	void initProject(void);

	/// Parses the input project file, initialzes global structure according to it (render, global animation object) and initializes the list of actions
	/**  
	 @param fileName File name of the file to be loaded
	 @param render Pointer to the renderer object
	*/
	void readProject(std::string actualLine, Scene* scene);

	/// Returns the action list
	ActionList * retActionList(void){return this->list;}

	/// Executes the sequence of actions defined in the action list
	void executeActions();

};


#endif //__PROJECT_H__
