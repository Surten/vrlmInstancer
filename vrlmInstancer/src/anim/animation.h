/**
@file animation.h
Declaration of functions for animation of the scene

@author Copyright (C) 2009 Ondrej Linda
*/


#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "dataStructs.h"

/// Class storing the information about the global animation settings and several other global settings
class AnimationInfo{
	/// Length of the animation in seconds [seconds]
	float mAnimLength;
	/// Frame rate of the animation [frames per second]
	float mAnimFps;
	/// Has the animation been set
	bool mHasBeenSet;
	/// Additional flag determining if we are performing the animation during the output
	bool mDoAnimate;
	/// The current time during the animation [seconds]
	float mCurrentTime;
	/// The current frame number during the animation
	int mCurrentFrame;
	/// Time step (1/fps)
	float mTimeStep;

	/// Flag determining if the handles should be animated automatically
	bool mHandlesAuto;
	/// The duration of handle animation [frames]
	int mHandlesDuration;

	/// Global parameters for adjusting the KB Splines for camera interpolation
	/// Tension
	float tension;
	/// Continuity
	float continuity;
	// Bias
	float bias;

	/// Flag determining that absolute path should be used for including the ies light definitions
	bool mAbsPathLight;

	/// Flag determining if the VRML animation should be in one or multiple files
	bool mVRMLOne;

	/// Flag determining if the file path has been set
	bool pathSet;

	/// Stores the last inserted file path for the directory dialogs
	std::string lastPath;

	/// Stores file name and path to the currenly opened dataset
	std::string datasetFile;

	/// Flag determining if the extended VRML format should be used during export
	bool mVRMLExtend;

		/// Flag determining if the material file should be used
	bool mLoadMaterial;

	/// File name of the material file used
	std::string matFile;

	
public:
	/// Constructors
	AnimationInfo() : mAnimLength(0.0), mAnimFps(0.0), mHasBeenSet(false), mDoAnimate(false), mCurrentTime(0.0), mCurrentFrame(0), mTimeStep(0.0),
		mHandlesAuto(false), mHandlesDuration(0), tension(0.0), continuity(0.0), bias(0.0), mAbsPathLight(false), mVRMLOne(false), pathSet(false),
		datasetFile(" "), mVRMLExtend(false), mLoadMaterial(false), matFile(" ")
	{}

	AnimationInfo(AnimationInfo& animInfo) : mAnimLength(animInfo.mAnimLength), mAnimFps(animInfo.mAnimFps), mHasBeenSet(animInfo.mHasBeenSet), mDoAnimate(animInfo.mDoAnimate), mCurrentTime(animInfo.mCurrentTime), mCurrentFrame(animInfo.mCurrentFrame), mTimeStep(animInfo.mTimeStep),
		mHandlesAuto(animInfo.mHandlesAuto), mHandlesDuration(animInfo.mHandlesDuration), tension(animInfo.tension), continuity(animInfo.continuity), bias(animInfo.bias), mAbsPathLight(animInfo.mAbsPathLight), mVRMLOne(animInfo.mVRMLOne), pathSet(animInfo.pathSet),
		datasetFile(animInfo.datasetFile), mVRMLExtend(animInfo.mVRMLExtend), mLoadMaterial(animInfo.mLoadMaterial), matFile(animInfo.matFile)
	{}
	/// Sets the length of the animation
	void setAnimLength(float len) { mAnimLength = len; }
	/// Returns the length of the animation
	float retAnimLength(void) { return mAnimLength; }
	/// Sets the frame rate of the animation
	void setAnimFps(float fps) { mAnimFps = fps; setTimeStep(); }
	/// Returns the frame rate
	float retAnimFps(void) { return mAnimFps; }
	/// Sets the has been set flag
	void setAnimFlag(bool flag) { mHasBeenSet = flag; }
	/// Returns the has been set flag
	bool retAnimFlag(void) { return mHasBeenSet; }	
	/// Sets the do animate flag
	void setDoAnimFlag(bool flag) { mDoAnimate = flag; }
	/// Returns the do animation flag
	bool retDoAnimFlag(void) { return mDoAnimate; }	
	/// Sets the current time of the animation
	void setCurrentTime(float time) { mCurrentTime = time; }
	/// Returns the current time of the animation
	float retCurrentTime(void) {return mCurrentTime; }
	/// Sets the current frame number
	void setCurrentFrame(int fps) {mCurrentFrame = fps;}
	/// Returns the current frame number
	int retCurrentFrame(void) {return mCurrentFrame; }
	/// Sets the time step
	void setTimeStep(void) {mTimeStep = 1.0f / mAnimFps;}
	/// Returns the time step
	float retTimeStep(void) {return mTimeStep; }
	/// Increases the current time
	void incCurrentTime(void) {mCurrentTime += mTimeStep;}
	/// Increases the current frame number
	void incCurrentFrame(void) {mCurrentFrame++;}
	/// Sets the automatic handles manipulation flag
	void setHandlesAuto(bool flag) {mHandlesAuto = flag;}
	/// Returns the automatic handles manipulation flag
	bool retHandlesAuto(void) {return mHandlesAuto;}
	/// Sets the duration of the automatic handles animation
	void setHandlesDuration(int frames) {mHandlesDuration = frames;}
	/// Returns the duration of the automatic handles animation
	int retHandlesDuration(void) {return mHandlesDuration;}
	/// Sets the tension parameter
	void setTension(float t) {this->tension = t;}
	/// Returns the tension parameter
	float retTension(void) {return this->tension;}
	/// Sets the continuity parameter
	void setContinuity(float c) {this->continuity = c;}
	/// Returns the continuity parameter
	float retContinuity(void) {return this->continuity;}
	/// Sets the bias parameter
	void setBias(float b) {this->bias = b;}
	/// Returns the bias parameter
	float retBias(void) {return this->bias;}
	/// Sets the abs path flag
	void setAbsPathLight(bool flag) {this->mAbsPathLight = flag;}
	/// Returns the abs path flag
	bool retAbsPathLight(void) {return this->mAbsPathLight; }
	/// Sets the VRML animation flag
	void setVRMLOne(bool flag) {this->mVRMLOne = flag;}
	/// Returns the VRML animation flag
	bool retVRMLOne(void) {return this->mVRMLOne;}
	/// Sets the pathSet flag
	void setPathSet(bool flag) {this->pathSet = flag;}
	/// Returns the pathSet flag
	bool retPathSet(void) {return this->pathSet;}
	/// Sets the last path variable
	void setLastPath(std::string path) {this->lastPath = path;}
	/// returns the last path variable
	std::string retLastPath(void){return this->lastPath;}
	/// Sets the dataset file path
	void setDataFile(std::string data) {this->datasetFile = data;}
	/// returns the dataset file path
	std::string retDataFile(void){return this->datasetFile;}
	/// Sets the VRML extended format flag
	void setVRMLExtend(bool flag) {this->mVRMLExtend = flag;}
	/// Returns the VRML extended format flag
	bool retVRMLExtend(void) {return this->mVRMLExtend;}
	/// Sets the load material file flag
	void setLoadMatFlag(bool flag) {this->mLoadMaterial = flag;}
	/// Returns the load material file flag
	bool retLoadMatFlag(void) {return this->mLoadMaterial;}
	/// Sets load material file name
	void setMatFile(std::string file) {this->matFile = file;}
	/// Returns the load material file name
	std::string retMatFile(void) {return this->matFile;}
};


/// Class implementing a single animation checkpoint. It is a parent class for animation checkpoints of each object.
class AnimationNode{
	
	/// Time of the animation [frame]
	int animTime;

	/// Flag determining if this is an automatic node
	bool autoNode;

	/// pointer to the next Animation node in the list
	AnimationNode * animNext;

	/// pointer to the previous Animation node in the list
	AnimationNode * animPrev;

public:
	/// Constructor
	AnimationNode() : animTime(0), autoNode(false), animNext(NULL), animPrev(NULL) {}
	/// Sets the animation time
	void setAnimTime(int time) {animTime = time; }
	/// Returns the animation time
	int retAnimTime(void) {return animTime; }
	/// Sets the next animation node
	void setAnimNext(AnimationNode * node) {animNext = node; }
	/// Returns the the next animation node
	AnimationNode * retAnimNext(void) {return animNext;}
	/// Sets the previous animation node
	void setAnimPrev(AnimationNode * node) {animPrev = node; }
	/// Returns the the previous animation node
	AnimationNode * retAnimPrev(void) {return animPrev; }
	/// Sets the autoNode flag
	void setAutoNode(bool flag) {this->autoNode = flag; }
	/// Returns the autoNode flag
	bool retAutoNode(void) {return this->autoNode;}
};

/// Class implementing the list of animation nodes (animation checkpoints) - it is a linked list of animation nodes
class AnimationList{
	/// head of the list
	AnimationNode * head;

	/// Pointer to the last node in the list
	AnimationNode * last;

	/// Mode of the interpolation used during this animation
	/// 0 - linear, 1 - cosine, 2 - cubic, 3- KB Splines
	int interpMode;

	/// Number of nodes in the list
	int nodeCount;
public:
	/// Constructor
	AnimationList() : head(NULL), last(NULL), interpMode(0), nodeCount(0) {}
	/// Destructor
	~AnimationList();
	/// Returns the head of the list
	AnimationNode * retListHead(void) {return head; }
	/// Returns the last node of the list
	AnimationNode * retListLast(void) {return last; }
	/// Returns the number of nodes in the list
	int retNodeCount(void) {return nodeCount;}
	/// Inserts node into the list - nodes are automatically ordered according to the time
	/// @return False if the node cannot be inserted (even time)
	bool insertNode(AnimationNode * node);
	/// Deletes node with particular index number
	/// @return True if the deletion went fine, false otherwise
	bool deleteNode(int index);
	/// Returns the interpolation mode
	int retInterpMode(void) {return interpMode;}
	/// Sets the interpolation mode
	void setInterpMode(int mode){interpMode = mode;}
	/// Goes through the animation list and adds automatic handle animation for doors
	void setHandlesDoorAuto(void);
	/// Goes through the animation list and adds automatic handle animation for windows
	void setHandlesWindowsAuto(void);
};

// Animation node for the doors
/// It inherits from the parent Animation Node class
class AnimationNodeDoor : public AnimationNode{
	/// Opening rate of the door
	int doorOpen;
	/// Opening rate of the door handle
	int doorHandle;
public:
	/// Constructor
	AnimationNodeDoor();
	/// Sets the opening rate for the door
	void setDoorOpen(int open) {doorOpen = open; }
	/// Returns the opening rate for the door
	int retDoorOpen(void) {return doorOpen; }
	/// Sets the opening rate for the door handle
	void setHandleRate(int rate) {doorHandle = rate; }
	/// Returns the opening rate for the door handle
	int retHandleRate(void) {return doorHandle; }
};

// Animation node for the shutters
/// It inherits from the parent Animation Node class
class AnimationNodeShutter : public AnimationNode{
	/// Opening rate of the shutter
	int shutterOpen;
	/// Rotation rate of the shutter
	int shutterRotate;
public:
	/// Constructor
	AnimationNodeShutter();
	/// Sets the opening rate for the shutter
	void setShutterOpen(int open) {shutterOpen = open; }
	/// Returns the opening rate for the shutter
	int retShutterOpen(void) {return shutterOpen; }
	/// Sets the rotation rate for the shutter
	void setShutterRotate(int rate) {shutterRotate = rate; }
	/// Returns the rotation rate for the shutter
	int retShutterRotate(void) {return shutterRotate; }
};

// Animation node for the windows
/// It inherits from the parent Animation Node class
class AnimationNodeWindow : public AnimationNode{
	/// Opening rate of the window
	int windowOpen;
	/// Opening rate of the window handle
	int windowHandle;
	/// Opening type of the window
	int windowOpenType;
public:
	/// Constructor
	AnimationNodeWindow();
	/// Sets the opening rate for the window
	void setWindowOpen(int open) {windowOpen = open; }
	/// Returns the opening rate for the window
	int retWindowOpen(void) {return windowOpen; }
	/// Sets the window handle opening rate
	void setHandleRate(int rate) {windowHandle = rate; }
	/// Returns the window handle opening rate
	int retHandleRate(void) {return windowHandle; }
	/// Sets the opening type of the window
	void setOpenType(int type) {windowOpenType = type; }
	/// Returns the opening type of the window
	int retOpenType(void) {return windowOpenType; }
};

// Animation node for the cameras
/// It inherits from the parent Animation Node class
class AnimationNodeCamera : public AnimationNode{
	/// Position of the camera
	vec3 cameraPosition;
	/// Orientation of the camera
	vec4 cameraOrientation;
	/// Parameters for the local  adjustment of the KB Spline
	/// Tension
	float tension;
	/// Continuity
	float continuity;
	// Bias
	float bias;

public:
	/// Constructor
	AnimationNodeCamera();
	/// Sets the position of the camera
	void setCameraPosition(vec3 pos) {cameraPosition = pos; }
	/// Returns the position of the camera
	vec3 retCameraPosition(void) {return cameraPosition; }
	/// Sets the orientation of the camera
	void setCameraOrientation(vec4 orient) {cameraOrientation = orient; }
	/// Returns the orientation of the camera
	vec4 retCameraOrientation(void) {return cameraOrientation; }
	/// Sets the tension parameter
	void setTension(float t) {this->tension = t;}
	/// Returns the tension parameter
	float retTension(void) {return this->tension;}
	/// Sets the continuity parameter
	void setContinuity(float c) {this->continuity = c;}
	/// Returns the continuity parameter
	float retContinuity(void) {return this->continuity;}
	/// Sets the bias parameter
	void setBias(float b) {this->bias = b;}
	/// Returns the bias parameter
	float retBias(void) {return this->bias;}
	/// Sets the abs path flag
	
};

// Animation node for the light
/// It inherits from the parent Animation Node class
class AnimationNodeLight : public AnimationNode{
	/// Is the light on/off
	bool lightOn;
	/// The intensity of the light
	int lightIntensity;
public:
	/// Constructor
	AnimationNodeLight();
	/// Sets the light on/off flag
	void setLigthOn(bool flag) {lightOn = flag; }
	/// Returns the ligth on/off flag
	bool retLightOn(void) {return lightOn; }
	/// Sets the light intensity
	void setLightIntensity(int intensity) {lightIntensity = intensity; }
	/// Returns the light intensity
	int retLightIntensity(void) {return lightIntensity;}
};

#endif //__ANIMATION_H__
