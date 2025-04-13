// ** Ended at 2246


/*** Stopped at checkProject at line 205

/**
@file project.cpp
Definitions of functions for project save/load

@author Copyright (C) 2009 Ondrej Linda
*/

#include <fstream>

#include "project.h"
#include "error.h"
#include "animation.h"
#include "scene.h"
#include "geometryModify.h"
#include "constants.h"





//----------------------------------------------------------------------------------------------
void Project::readProject(std::string actualLine)
//----------------------------------------------------------------------------------------------
{
	int index = 0;
	// Global Animation - Animation Length
	if(((int)actualLine.find("GA_AnimLength") > -1) && (actualLine.find("#") >= NOPE)){
		index = (int)actualLine.find(' ');
		std::string line = actualLine.substr(index + 1);
			
		animInfo->setAnimLength((float)atof(line.c_str()));
	}

	if (animInfo->retAnimLength() > 0.0){
		animInfo->setAnimFlag(true);
	}
	else{
		animInfo->setAnimFlag(false);
	}


	// Global Animation - Animation FPS
	if(((int)actualLine.find("GA_AnimFPS") > -1) && (actualLine.find("#") >= NOPE)){
		index = (int)actualLine.find(' ');
		std::string line = actualLine.substr(index + 1);
			
		animInfo->setAnimFps((float)atof(line.c_str()));
	}

	// Global Animation - Global KB Spline settings
	if(((int)actualLine.find("GA_AnimKB") > -1) && (actualLine.find("#") >= NOPE)){
		index = (int)actualLine.find(' ');
		std::string line = actualLine.substr(index + 1);

		int index = (int)line.find(' ');
		std::string t = line.substr(0, index);

		line = line.substr(index + 1);
		index = (int)line.find(' ');
		std::string c = line.substr(0, index);

		line = line.substr(index + 1);			
		std::string b = line;

		animInfo->setTension((float)atof(t.c_str()));
		animInfo->setContinuity((float)atof(c.c_str()));
		animInfo->setBias((float)atof(b.c_str()));
	}


	// Global Animation - Automatic handle animation
	if(((int)actualLine.find("GA_AnimHandles") > -1) && (actualLine.find("#") >= NOPE)){
		index = (int)actualLine.find(' ');
		std::string line = actualLine.substr(index + 1);

		int index = (int)line.find(' ');
		std::string automatic = line.substr(0, index);

		line = line.substr(index + 1);			
		std::string duration = line;

		if (atoi(automatic.c_str()) == 0){
			animInfo->setHandlesAuto(false);
		}
		else{
			animInfo->setHandlesAuto(true);
		}

		animInfo->setHandlesDuration((int)atof(duration.c_str()));
	}

	// Action - Door
	if(((int)actualLine.find("A_Door") > -1) && (actualLine.find("#") >= NOPE)){
		index = (int)actualLine.find(' ');
		std::string line = actualLine.substr(index + 1);

		ActionNodeDoor * doorNode = new ActionNodeDoor();
		doorNode->setActionType(ACTION_DOOR);

		doorNode->parseInput(line);

		this->retActionList()->insertNode(doorNode);
		this->scene->isAnimated = true;
		animInfo->setDoAnimFlag(true);
	}

	// Action - Window
	if(((int)actualLine.find("A_Window") > -1) && (actualLine.find("#") >= NOPE)){
		index = (int)actualLine.find(' ');
		std::string line = actualLine.substr(index + 1);

		ActionNodeWindow * windowNode = new ActionNodeWindow();
		windowNode->setActionType(ACTION_WINDOW);

		windowNode->parseInput(line);

		this->retActionList()->insertNode(windowNode);	
		this->scene->isAnimated = true;
		animInfo->setDoAnimFlag(true);
	}
	// Action - Shutter
	if(((int)actualLine.find("A_Shutter") > -1) && (actualLine.find("#") >= NOPE)){
		index = (int)actualLine.find(' ');
		std::string line = actualLine.substr(index + 1);

		ActionNodeShutter * shutterNode = new ActionNodeShutter();
		shutterNode->setActionType(ACTION_SHUTTER);

		shutterNode->parseInput(line);

		this->retActionList()->insertNode(shutterNode);	
		this->scene->isAnimated = true;
		animInfo->setDoAnimFlag(true);
	}

	// Action - Camera
	if(((int)actualLine.find("A_Camera") > -1) && (actualLine.find("#") >= NOPE)){
		index = (int)actualLine.find(' ');
		std::string line = actualLine.substr(index + 1);

		ActionNodeCamera * cameraNode = new ActionNodeCamera();
		cameraNode->setActionType(ACTION_CAMERA);

		cameraNode->parseInput(line);

		this->retActionList()->insertNode(cameraNode);
		scene->hasAnimatedCamera = true;
		animInfo->setDoAnimFlag(true);
	}
}


//----------------------------------------------------------------------------------------------
void Project::initProject()
//----------------------------------------------------------------------------------------------
{	
	// initialize the variables
	this->m_bInitialized = false;
	this->fileName = "";
	this->m_iNumberAllNodes = 0;
	this->m_iNumberBaseNodes = 0;
	this->m_iNumberCameras = 0;
	this->m_iNumberLights = 0;
	this->m_iNumberDoors = 0;
	this->m_iNumberWindows = 0;
	this->m_iNumberShutters = 0;
	this->list = new ActionList();
}

//----------------------------------------------------------------------------------------------
void Project::executeActions()
//----------------------------------------------------------------------------------------------
{

	// Check if the action list is empty
	if (this->retActionList()->retNumNodes() == 0){
		return;
	}

	// Iterate through the list and execute each action

	ActionNode * help = this->retActionList()->retHead();

	while (help != NULL){

		if (help->retActionType() == ACTION_DOOR){			
			ActionNodeDoor * helpD = static_cast<ActionNodeDoor*> (help);

			helpD->executeNode(scene);
		}
		else if (help->retActionType() == ACTION_WINDOW){
			ActionNodeWindow * helpW = static_cast<ActionNodeWindow*> (help);

			helpW->executeNode(scene);
		}
		else if (help->retActionType() == ACTION_SHUTTER){
			ActionNodeShutter * helpS = static_cast<ActionNodeShutter*> (help);

			helpS->executeNode(scene);
		}
		else if (help->retActionType() == ACTION_CAMERA){
			ActionNodeCamera * helpC = static_cast<ActionNodeCamera*> (help);

			helpC->executeNode(scene);
		}

		help = help->retNext();
	}
}

//----------------------------------------------------------------------------------------------
ActionList::ActionList()
//----------------------------------------------------------------------------------------------
{
	this->head = NULL;
	this->last = NULL;
	this->numNodes = 0;
}

//----------------------------------------------------------------------------------------------
void ActionList::insertNode(ActionNode * node)
//----------------------------------------------------------------------------------------------
{
	this->numNodes++;

	if (this->head == NULL){
		this->head = node;
		this->last = node;
	}
	else{
		this->last->setNext(node);
		this->last = node;
	}

	if(this->head == NULL){
	}
}


//----------------------------------------------------------------------------------------------
ActionNode::ActionNode()
//----------------------------------------------------------------------------------------------
{
	this->objectIndex = 0;
	this->message = "";
	this->next = NULL;
	this->actionType = ACTION_NONE;
	this->appMode = APPLY_SINGLE;
}

//----------------------------------------------------------------------------------------------
void ActionNode::writeNode(std::ostream& out)
//----------------------------------------------------------------------------------------------
{
	out << "# 1 " << this->message << std::endl;
}

//----------------------------------------------------------------------------------------------
void ActionNode::setAppMode(int i)
//----------------------------------------------------------------------------------------------
{
	if (i == 0){
		this->appMode = APPLY_SINGLE;
	}
	else if (i == 1){
		this->appMode = APPLY_SECTION;
	}
	else if (i == 2){
		this->appMode = APPLY_FLOOR;
	}
	else{
		this->appMode = APPLY_ALL;
	}
}

//----------------------------------------------------------------------------------------------
int ActionNode::retAppModeNum(void)
//----------------------------------------------------------------------------------------------
{
	if (this->appMode == APPLY_SINGLE){
		return 0;
	}
	else if (this->appMode == APPLY_SECTION){
		return 1;
	}
	else if (this->appMode == APPLY_FLOOR){
		return 2;
	}
	else {
		return 3;
	}
}

//----------------------------------------------------------------------------------------------
ActionNodeDoor::ActionNodeDoor()
//----------------------------------------------------------------------------------------------
{		
	this->m_iOpenRate = 0;
	this->m_iHandleRate = 0;
	this->m_bAnimFlag = false;
	this->m_iInterpolation = 0;
	this->animList = new AnimationList();
};


//----------------------------------------------------------------------------------------------
void ActionNodeDoor::writeDoorNode(std::ostream& out)
//----------------------------------------------------------------------------------------------
{
	out << "# Door - Apply mode, door index, open rate, handle rate, animated, interpolation mode, number of nodes, animation list(time, door, handle) ...." 
		<< std::endl;
	out << "A_Door " << this->retAppModeNum() << " " << this->retObjectIndex() << " " << this->m_iOpenRate << " " << this->m_iHandleRate << " ";

	if (this->m_bAnimFlag){
		out << "1 ";
	}
	else{
		out << "0 ";
	}

	// If the door is being animated - insert the animation list as well
	if (this->m_bAnimFlag){
		// Insert the interpolation mode
		out << this->m_iInterpolation << " " << this->retAnimList()->retNodeCount() << " ";

		AnimationNode * help = this->retAnimList()->retListHead();

		while (help != NULL){

			AnimationNodeDoor* helpD = static_cast<AnimationNodeDoor*> (help);

			out << helpD->retAnimTime() << " " << helpD->retDoorOpen() << " " << helpD->retHandleRate() << " ";

			help = help->retAnimNext();
		}

		out << std::endl;

	}
	else{
		out << std::endl;
	}

}

//----------------------------------------------------------------------------------------------
void ActionNodeDoor::parseInput(std::string input)
//----------------------------------------------------------------------------------------------
{
	int index = (int)input.find(' ');
	std::string applyMode = input.substr(0, index);

	input = input.substr(index + 1);			
	index = (int)input.find(' ');
	std::string doorIndex = input.substr(0, index);

	input = input.substr(index + 1);			
	index = (int)input.find(' ');
	std::string openRate = input.substr(0, index);

	input = input.substr(index + 1);			
	index = (int)input.find(' ');
	std::string handleRate = input.substr(0, index);

	input = input.substr(index + 1);			
	index = (int)input.find(' ');

	std::string animF;
	if (input.find(' ') == NOPE){
		animF = input;
	}
	else{
		animF = input.substr(0, index);
	}

	this->setAppMode(atoi(applyMode.c_str()));
	this->setObjectIndex(atoi(doorIndex.c_str()));
	this->setDoorOpenRate(atoi(openRate.c_str()));
	this->setDoorHandleRate(atoi(handleRate.c_str()));

	if (atoi(animF.c_str()) == 0){
		this->setAnimFlag(false);
	}
	else{
		this->setAnimFlag(true);
	}

	// This node is animated -> extract the animation list
	if (this->retAnimFlag()){

		// Initialize the animation list of this actio node
		this->animList = new AnimationList();

		// Extract the interpolation mode
		index = (int)input.find(' ');
		input = input.substr(index + 1);			
		index = (int)input.find(' ');
		int interpMode = atoi(input.substr(0, index).c_str());

		this->animList->setInterpMode(interpMode);

		// First get the number of animation nodes
		input = input.substr(index + 1);			
		index = (int)input.find(' ');
		int nodesCount = atoi(input.substr(0, index).c_str());

		// extract all the nodes and deal separately with the last one
		for (int i = 0; i < nodesCount - 1; i++){

			AnimationNodeDoor * nodeD = new AnimationNodeDoor();

			// Get the animation time
			index = (int)input.find(' ');
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string timeAnim = input.substr(0, index);

			// Get the door open rate
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string openAnim = input.substr(0, index);

			// Get the handle rate
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string handleAnim = input.substr(0, index);

			// Store the value in the animation node
			nodeD->setAnimTime(atoi(timeAnim.c_str()));
			nodeD->setDoorOpen(atoi(openAnim.c_str()));
			nodeD->setHandleRate(atoi(handleAnim.c_str()));

			// Insert the animation node into the animation list
			this->retAnimList()->insertNode(nodeD);
		}
		// Deal with the last animation node in the same way
		AnimationNodeDoor * nodeD = new AnimationNodeDoor();

		index = (int)input.find(' ');
		input = input.substr(index + 1);			
		index = (int)input.find(' ');
		std::string timeAnim = input.substr(0, index);

		input = input.substr(index + 1);			
		index = (int)input.find(' ');
		std::string openAnim = input.substr(0, index);

		input = input.substr(index + 1);						
		std::string handleAnim = input;

		nodeD->setAnimTime(atoi(timeAnim.c_str()));
		nodeD->setDoorOpen(atoi(openAnim.c_str()));
		nodeD->setHandleRate(atoi(handleAnim.c_str()));

		// Insert the animation node into the animation list
		this->retAnimList()->insertNode(nodeD);
	}
}


//----------------------------------------------------------------------------------------------
void ActionNodeDoor::executeNode(Scene* scene)
//----------------------------------------------------------------------------------------------
{
	// branch according to the application mode
	// First have to identify the door object with the same index, it is used to extract its section and floor

	bool found = false;
	int doorCounter = 0;
	TransformNode* tempT = NULL;
	DoorInfo* tempDInfo = NULL;

	for(int i = 0; i < scene->RootNodes.size(); i++){
		BaseNode * tempN = scene->RootNodes[i];
		// Find the parent that contains the list of doors in certain floor
		if (CheckNodeNameCode(tempN->name.c_str(), CODE_DOOR_FLOOR)){
			TransformNode* tempParT = static_cast<TransformNode*> (tempN);

			for (int j = 0; j < tempParT->children.size(); j++) {
				BaseNode* tempCh = tempParT->children[j];
				// We are only interested in nodes that define doors
				if (CheckNodeNameCode(tempCh->name.c_str(), CODE_DOOR)) {

					tempT = static_cast<TransformNode*> (tempCh);

					tempDInfo = static_cast<DoorInfo*> (tempT->retObjectInfo());

					if (this->retObjectIndex() == doorCounter) {

						// The correct object was found


						tempDInfo->setRotateDeg(this->retDoorOpenRate());
						tempDInfo->setHandleRate(this->retDoorHandleRate());

						if (this->retDoorOpenRate() != 0) {
							tempDInfo->setHasModified(true);

							tempT->setDoorAxis();
						}

						tempDInfo->setHasAnimated(this->retAnimFlag());

						if (this->retAnimFlag()) {
							// Set the correct joint axis
							tempT->setDoorAxis();
						}

						tempDInfo->copyAnimList(this->retAnimList());

						tempDInfo->retAnimationList()->setInterpMode(this->retInterpolation());

						found = true;
						break;
					}
					doorCounter++;
					
				}
			}
			if (found) {
				break;
			}
		}
	}


	// Single door object
	if (this->retAppMode() == APPLY_SINGLE) {
		return;
	}
	// Whole section
	else if (this->retAppMode() == APPLY_SECTION) {

		// Iterate throught the list of doors again and set the proper values for all doors in the same section
		for (int i = 0; i < scene->RootNodes.size(); i++) {
			// Point to the current node
			BaseNode* temp = scene->RootNodes[i];
			if (CheckNodeNameCode(temp->name.c_str(), CODE_DOOR_FLOOR)) {

				TransformNode* tempParT = static_cast<TransformNode*> (temp);

				for (int j = 0; j < tempParT->children.size(); j++) {
					BaseNode* tempCh = tempParT->children[j];
					// We are only interested in nodes that define doors
					if (CheckNodeNameCode(tempCh->name.c_str(), CODE_DOOR)) {
						// Retype the node to the transform node
						TransformNode* tempTAll = static_cast<TransformNode*> (tempCh);

						// Make sure it is not the current node
						if (tempT != tempTAll) {

							DoorInfo* tempDInfoAll = static_cast<DoorInfo*> (tempTAll->retObjectInfo());

							// Check if the door is in the same section and floor
							if ((tempDInfo->retObjectFloor() == tempDInfoAll->retObjectFloor()) &&
								(tempDInfo->retObjectSection() == tempDInfoAll->retObjectSection())) {

								tempDInfoAll->setRotateDeg(this->retDoorOpenRate());
								tempDInfoAll->setHandleRate(this->retDoorHandleRate());

								if (this->retDoorOpenRate() != 0) {
									tempDInfoAll->setHasModified(true);

									tempTAll->setDoorAxis();
								}

								tempDInfoAll->setHasAnimated(this->retAnimFlag());


								if (this->retAnimFlag()) {
									// Set the correct joint axis
									tempTAll->setDoorAxis();
								}


								// Copy the animation list
								tempDInfoAll->copyAnimList(this->retAnimList());

								tempDInfoAll->retAnimationList()->setInterpMode(this->retInterpolation());
							}
						}
					}
				}
			}
		}
	}
	// Whole floor
	else if (this->retAppMode() == APPLY_FLOOR) {

		// Iterate throught the list of doors again and set the proper values for all doors in the same section
		for (int i = 0; i < scene->RootNodes.size(); i++) {
			// Point to the current node
			BaseNode* temp = scene->RootNodes[i];
			if (CheckNodeNameCode(temp->name.c_str(), CODE_DOOR_FLOOR)) {

				TransformNode* tempParT = static_cast<TransformNode*> (temp);

				for (int j = 0; j < tempParT->children.size(); j++) {
					BaseNode* tempCh = tempParT->children[j];
					// We are only interested in nodes that define doors
					if (CheckNodeNameCode(tempCh->name.c_str(), CODE_DOOR)) {
						// Retype the node to the transform node
						TransformNode* tempTAll = static_cast<TransformNode*> (tempCh);

						// Make sure it is not the current node
						if (tempT != tempTAll) {

							DoorInfo* tempDInfoAll = static_cast<DoorInfo*> (tempTAll->retObjectInfo());

							// Check if the door is in the same section and floor
							if (tempDInfo->retObjectFloor() == tempDInfoAll->retObjectFloor()) {

								tempDInfoAll->setRotateDeg(this->retDoorOpenRate());
								tempDInfoAll->setHandleRate(this->retDoorHandleRate());

								if (this->retDoorOpenRate() != 0) {
									tempDInfoAll->setHasModified(true);

									tempTAll->setDoorAxis();
								}

								tempDInfoAll->setHasAnimated(this->retAnimFlag());


								if (this->retAnimFlag()) {
									// Set the correct joint axis
									tempTAll->setDoorAxis();
								}

								// Copy the animation list
								tempDInfoAll->copyAnimList(this->retAnimList());

								tempDInfoAll->retAnimationList()->setInterpMode(this->retInterpolation());
							}
						}
					}
				}
			}
		}

	}
	// All doors
	else {

		// Iterate throught the list of doors again and set the proper values for all doors in the same section
		for (int i = 0; i < scene->RootNodes.size(); i++) {
			// Point to the current node
			BaseNode* temp = scene->RootNodes[i];
			if (CheckNodeNameCode(temp->name.c_str(), CODE_DOOR_FLOOR)) {

				TransformNode* tempParT = static_cast<TransformNode*> (temp);

				for (int j = 0; j < tempParT->children.size(); j++) {
					BaseNode* tempCh = tempParT->children[j];
					// We are only interested in nodes that define doors
					if (CheckNodeNameCode(tempCh->name.c_str(), CODE_DOOR)) {
						// Retype the node to the transform node
						TransformNode* tempTAll = static_cast<TransformNode*> (tempCh);

						// Make sure it is not the current node
						if (tempT != tempTAll) {

							DoorInfo* tempDInfoAll = static_cast<DoorInfo*> (tempTAll->retObjectInfo());

							tempDInfoAll->setRotateDeg(this->retDoorOpenRate());
							tempDInfoAll->setHandleRate(this->retDoorHandleRate());

							if (this->retDoorOpenRate() != 0) {
								tempDInfoAll->setHasModified(true);

								tempTAll->setDoorAxis();
							}

							tempDInfoAll->setHasAnimated(this->retAnimFlag());

							if (this->retAnimFlag()) {
								// Set the correct joint axis
								tempTAll->setDoorAxis();
							}

							// Copy the animation list
							tempDInfoAll->copyAnimList(this->retAnimList());

							tempDInfoAll->retAnimationList()->setInterpMode(this->retInterpolation());
						}
					}
				}
			}
		}
	}
}


//----------------------------------------------------------------------------------------------
ActionNodeWindow::ActionNodeWindow()
//----------------------------------------------------------------------------------------------
{
	this->m_iOpenRate = 0;
	this->m_iHandleRate = 0;
	this->m_iOpenMode = 0;
	this->m_bAnimFlag = false;
	this->m_iInterpolation = 0;
	this->animList = new AnimationList();
};


//----------------------------------------------------------------------------------------------
void ActionNodeWindow::writeWindowNode(std::ostream& out)
//----------------------------------------------------------------------------------------------
{
	out << "# Window - Apply mode, window index, open rate, handle rate, open mode, animated, interpolation mode, number of nodes, animation list(time, door, handle, mode) ...."
		<< std::endl;
	out << "A_Window " << this->retAppModeNum() << " " << this->retObjectIndex() << " " << this->m_iOpenRate << " " <<
		this->m_iHandleRate << " " << this->m_iOpenMode << " ";

	if (this->m_bAnimFlag) {
		out << "1 ";
	}
	else {
		out << "0 ";
	}

	// If the window is being animated - insert the animation list as well
	if (this->m_bAnimFlag) {
		// Insert the interpolation mode
		out << this->m_iInterpolation << " " << this->retAnimList()->retNodeCount() << " ";

		AnimationNode* help = this->retAnimList()->retListHead();

		while (help != NULL) {

			AnimationNodeWindow* helpW = static_cast<AnimationNodeWindow*> (help);

			out << helpW->retAnimTime() << " " << helpW->retWindowOpen() << " " << helpW->retHandleRate() << " " << helpW->retOpenType() << " ";

			help = help->retAnimNext();
		}

		out << std::endl;

	}
	else {
		out << std::endl;
	}

}

//----------------------------------------------------------------------------------------------
void ActionNodeWindow::parseInput(std::string input)
//----------------------------------------------------------------------------------------------
{
	int index = (int)input.find(' ');
	std::string applyMode = input.substr(0, index);

	input = input.substr(index + 1);
	index = (int)input.find(' ');
	std::string windowIndex = input.substr(0, index);

	input = input.substr(index + 1);
	index = (int)input.find(' ');
	std::string openRate = input.substr(0, index);

	input = input.substr(index + 1);
	index = (int)input.find(' ');
	std::string handleRate = input.substr(0, index);

	input = input.substr(index + 1);
	index = (int)input.find(' ');
	std::string openMode = input.substr(0, index);

	input = input.substr(index + 1);
	index = (int)input.find(' ');

	std::string animF;
	if (input.find(' ') == NOPE) {
		animF = input;
	}
	else {
		animF = input.substr(0, index);
	}

	this->setAppMode(atoi(applyMode.c_str()));
	this->setObjectIndex(atoi(windowIndex.c_str()));
	this->setWindowOpenRate(atoi(openRate.c_str()));
	this->setWindowHandleRate(atoi(handleRate.c_str()));
	this->setWindowOpenMode(atoi(openMode.c_str()));

	if (atoi(animF.c_str()) == 0) {
		this->setAnimFlag(false);
	}
	else {
		this->setAnimFlag(true);
	}

	// This node is animated -> extract the animation list
	if (this->retAnimFlag()) {

		// Initialize the animation list of this actio node
		this->animList = new AnimationList();

		// Extract the interpolation mode
		index = (int)input.find(' ');
		input = input.substr(index + 1);
		index = (int)input.find(' ');
		int interpMode = atoi(input.substr(0, index).c_str());

		this->animList->setInterpMode(interpMode);

		// First get the number of animation nodes
		input = input.substr(index + 1);
		index = (int)input.find(' ');
		int nodesCount = atoi(input.substr(0, index).c_str());

		// extract all the nodes and deal separately with the last one
		for (int i = 0; i < nodesCount - 1; i++) {

			AnimationNodeWindow* nodeW = new AnimationNodeWindow();

			// Get the animation time
			index = (int)input.find(' ');
			input = input.substr(index + 1);
			index = (int)input.find(' ');
			std::string timeAnim = input.substr(0, index);

			// Get the door open rate
			input = input.substr(index + 1);
			index = (int)input.find(' ');
			std::string openAnim = input.substr(0, index);

			// Get the handle rate
			input = input.substr(index + 1);
			index = (int)input.find(' ');
			std::string handleAnim = input.substr(0, index);

			// Get the open mode
			input = input.substr(index + 1);
			index = (int)input.find(' ');
			std::string openMode = input.substr(0, index);

			// Store the value in the animation node
			nodeW->setAnimTime(atoi(timeAnim.c_str()));
			nodeW->setWindowOpen(atoi(openAnim.c_str()));
			nodeW->setHandleRate(atoi(handleAnim.c_str()));
			nodeW->setOpenType(atoi(openMode.c_str()));

			// Insert the animation node into the animation list
			this->retAnimList()->insertNode(nodeW);
		}
		// Deal with the last animation node in the same way
		AnimationNodeWindow* nodeW = new AnimationNodeWindow();

		index = (int)input.find(' ');
		input = input.substr(index + 1);
		index = (int)input.find(' ');
		std::string timeAnim = input.substr(0, index);

		input = input.substr(index + 1);
		index = (int)input.find(' ');
		std::string openAnim = input.substr(0, index);

		input = input.substr(index + 1);
		index = (int)input.find(' ');
		std::string handleAnim = input.substr(0, index);

		input = input.substr(index + 1);
		std::string openMode = input;

		nodeW->setAnimTime(atoi(timeAnim.c_str()));
		nodeW->setWindowOpen(atoi(openAnim.c_str()));
		nodeW->setHandleRate(atoi(handleAnim.c_str()));
		nodeW->setOpenType(atoi(openMode.c_str()));

		// Insert the animation node into the animation list
		this->retAnimList()->insertNode(nodeW);
	}
}


//----------------------------------------------------------------------------------------------
void ActionNodeWindow::executeNode(Scene* scene)
//----------------------------------------------------------------------------------------------
{
	// branch according to the application mode

	// First have to identify the window object with the same index, it is used to extract its section and floor
	bool found = false;
	int windowCounter = 0;
	TransformNode* tempWT = NULL;
	WindowInfo* tempWInfo = NULL;

	for (int i = 0; i < scene->RootNodes.size(); i++) {
		BaseNode* tempN = scene->RootNodes[i];
		// Test if it is the window object
		if (CheckNodeNameCode(tempN->name.c_str(), CODE_WINDOWS)) {
			// Retype the node to the correct transform node
			TransformNode* tempT = static_cast<TransformNode*> (tempN);

			// Iterate through all the children and find the moving windows
			for (int j = 0; j < tempT->children.size(); j++) {

				BaseNode* tempW = tempT->children[j];

				// Check if this the node with the moving windows
				if (CheckNodeNameCode(tempW->name.c_str(), CODE_WINDOW_OPEN))
				{
					// Retype the node to the correct transform node
					tempWT = static_cast<TransformNode*> (tempW);
					tempWInfo = static_cast<WindowInfo*> (tempWT->retObjectInfo());

					if ((windowCounter == this->retObjectIndex())) {

						tempWInfo->setRotateDeg(this->retWindowOpenRate());
						tempWInfo->setHandleRate(this->retWindowHandleRate());
						tempWInfo->setOpenType(this->retWindowOpenMode());

						if (this->retWindowOpenRate() != 0) {
							tempWInfo->setHasModified(true);

							tempWT->setWindowAxis();
						}

						tempWInfo->setHasAnimated(this->retAnimFlag());

						if (this->retAnimFlag()) {
							tempWT->setWindowAxis();
						}

						tempWInfo->copyAnimList(this->retAnimList());

						found = true;
						break;
					}
					windowCounter++;
				}
				
			}

		}

		if (found) {
			break;
		}
	}

	// Single window object
	if (this->retAppMode() == APPLY_SINGLE) {
		return;
	}

	// Whole section
	else if (this->retAppMode() == APPLY_SECTION) {

		// Iterate throught the list of windows again and set the proper values for all windows in the same section		
		for (int i = 0; i < scene->AllNodes.size(); i++) {
			// Point to the current node
			BaseNode* temp = scene->AllNodes[i];
			// We are only interested in nodes that define windows
			if (CheckNodeNameCode(temp->name.c_str(), CODE_WINDOW_OPEN)) {
				// Retype the node to the transform node
				TransformNode* tempT = static_cast<TransformNode*> (temp);

				// Make sure it is not the current node
				if (tempT != tempWT) {

					WindowInfo* tempWInfoAll = static_cast<WindowInfo*> (tempT->retObjectInfo());

					// Check if the window is in the same section and floor
					if ((tempWInfo->retObjectFloor() == tempWInfoAll->retObjectFloor()) &&
						(tempWInfo->retObjectSection() == tempWInfoAll->retObjectSection())) {

						tempWInfoAll->setRotateDeg(this->retWindowOpenRate());
						tempWInfoAll->setHandleRate(this->retWindowHandleRate());
						tempWInfoAll->setOpenType(this->retWindowOpenMode());

						if (this->retWindowOpenRate() != 0) {
							tempWInfoAll->setHasModified(true);

							tempT->setWindowAxis();
						}

						tempWInfoAll->setHasAnimated(this->retAnimFlag());

						if (this->retAnimFlag()) {
							// Set the correct joint axis
							tempT->setWindowAxis();
						}

						// Copy the animation list
						tempWInfoAll->copyAnimList(this->retAnimList());
					}
				}
			}
		}


	}
	// Whole floor
	else if (this->retAppMode() == APPLY_FLOOR) {

		// Iterate throught the list of windows again and set the proper values for all windows in the same floor
		for (int i = 0; i < scene->AllNodes.size(); i++) {
			// Point to the current node
			BaseNode* temp = scene->AllNodes[i];
			// We are only interested in nodes that define windows
			if (CheckNodeNameCode(temp->name.c_str(), CODE_WINDOW_OPEN)) {
				// Retype the node to the transform node
				TransformNode* tempT = static_cast<TransformNode*> (temp);

				// Make sure it is not the current node
				if (tempT != tempWT) {

					WindowInfo* tempWInfoAll = static_cast<WindowInfo*> (tempT->retObjectInfo());

					// Check if the window is in the same section and floor
					if (tempWInfo->retObjectFloor() == tempWInfoAll->retObjectFloor()) {

						tempWInfoAll->setRotateDeg(this->retWindowOpenRate());
						tempWInfoAll->setHandleRate(this->retWindowHandleRate());
						tempWInfoAll->setOpenType(this->retWindowOpenMode());

						if (this->retWindowOpenRate() != 0) {
							tempWInfoAll->setHasModified(true);

							tempT->setWindowAxis();
						}

						tempWInfoAll->setHasAnimated(this->retAnimFlag());

						if (this->retAnimFlag()) {
							// Set the correct joint axis
							tempT->setWindowAxis();
						}

						// Copy the animation list
						tempWInfoAll->copyAnimList(this->retAnimList());
					}
				}
			}
		}

	}
	// All windows
	else {

		// Iterate throught the list of windows again
		for (int i = 0; i < scene->AllNodes.size(); i++) {
			// Point to the current node
			BaseNode* temp = scene->AllNodes[i];
			// We are only interested in nodes that define windows
			if (CheckNodeNameCode(temp->name.c_str(), CODE_WINDOW_OPEN)) {
				// Retype the node to the transform node
				TransformNode* tempT = static_cast<TransformNode*> (temp);

				// Make sure it is not the current node
				if (tempT != tempWT) {

					WindowInfo* tempWInfoAll = static_cast<WindowInfo*> (tempT->retObjectInfo());

					tempWInfoAll->setRotateDeg(this->retWindowOpenRate());
					tempWInfoAll->setHandleRate(this->retWindowHandleRate());
					tempWInfoAll->setOpenType(this->retWindowOpenMode());

					if (this->retWindowOpenRate() != 0) {
						tempWInfoAll->setHasModified(true);

						tempT->setWindowAxis();
					}

					tempWInfoAll->setHasAnimated(this->retAnimFlag());

					if (this->retAnimFlag()) {
						// Set the correct joint axis
						tempT->setWindowAxis();
					}

					// Copy the animation list
					tempWInfoAll->copyAnimList(this->retAnimList());
				}
			}
		}
	}
}


//----------------------------------------------------------------------------------------------
ActionNodeShutter::ActionNodeShutter()
//----------------------------------------------------------------------------------------------
{
	this->m_iScaleRate = 0;
	this->m_iRotateRate = 0;
	this->m_bAnimFlag = false;
	this->m_iInterpolation = 0;
	this->animList = new AnimationList();
};


//----------------------------------------------------------------------------------------------
void ActionNodeShutter::writeShutterNode(std::ofstream& out)
//----------------------------------------------------------------------------------------------
{
	out << "# Shutter - Apply mode, shutter index, scale rate, rotate rate, animated, interpolation mode, number of nodes, animation list(time, scale, rotate) ...."
		<< std::endl;
	out << "A_Shutter " << this->retAppModeNum() << " " << this->retObjectIndex() << " " << this->m_iScaleRate << " " << this->m_iRotateRate << " ";

	if (this->m_bAnimFlag) {
		out << "1 ";
	}
	else {
		out << "0 ";
	}

	// If the shutter is being animated - insert the animation list as well
	if (this->m_bAnimFlag) {
		// Insert the interpolation mode
		out << this->m_iInterpolation << " " << this->retAnimList()->retNodeCount() << " ";

		AnimationNode* help = this->retAnimList()->retListHead();

		while (help != NULL) {

			AnimationNodeShutter* helpS = static_cast<AnimationNodeShutter*> (help);

			out << helpS->retAnimTime() << " " << helpS->retShutterOpen() << " " << helpS->retShutterRotate() << " ";

			help = help->retAnimNext();
		}

		out << std::endl;

	}
	else {
		out << std::endl;
	}

}

//----------------------------------------------------------------------------------------------
void ActionNodeShutter::parseInput(std::string input)
//----------------------------------------------------------------------------------------------
{
	int index = (int)input.find(' ');
	std::string applyMode = input.substr(0, index);

	input = input.substr(index + 1);
	index = (int)input.find(' ');
	std::string shutterIndex = input.substr(0, index);

	input = input.substr(index + 1);
	index = (int)input.find(' ');
	std::string scaleRate = input.substr(0, index);

	input = input.substr(index + 1);
	index = (int)input.find(' ');
	std::string rotateRate = input.substr(0, index);

	input = input.substr(index + 1);
	index = (int)input.find(' ');

	std::string animF;
	if (input.find(' ') == NOPE) {
		animF = input;
	}
	else {
		animF = input.substr(0, index);
	}

	this->setAppMode(atoi(applyMode.c_str()));
	this->setObjectIndex(atoi(shutterIndex.c_str()));
	this->setShutterScaleRate(atoi(scaleRate.c_str()));
	this->setShutterRotateRate(atoi(rotateRate.c_str()));

	if (atoi(animF.c_str()) == 0) {
		this->setAnimFlag(false);
	}
	else {
		this->setAnimFlag(true);
	}

	// This node is animated -> extract the animation list
	if (this->retAnimFlag()) {

		// Initialize the animation list of this actio node
		this->animList = new AnimationList();

		// Extract the interpolation mode
		index = (int)input.find(' ');
		input = input.substr(index + 1);
		index = (int)input.find(' ');
		int interpMode = atoi(input.substr(0, index).c_str());

		this->animList->setInterpMode(interpMode);

		// First get the number of animation nodes
		input = input.substr(index + 1);
		index = (int)input.find(' ');
		int nodesCount = atoi(input.substr(0, index).c_str());

		// extract all the nodes and deal separately with the last one
		for (int i = 0; i < nodesCount - 1; i++) {

			AnimationNodeShutter* nodeS = new AnimationNodeShutter();

			// Get the animation time
			index = (int)input.find(' ');
			input = input.substr(index + 1);
			index = (int)input.find(' ');
			std::string timeAnim = input.substr(0, index);

			// Get the shutter scale rate
			input = input.substr(index + 1);
			index = (int)input.find(' ');
			std::string scaleAnim = input.substr(0, index);

			// Get the shutter rotate rate
			input = input.substr(index + 1);
			index = (int)input.find(' ');
			std::string rotateAnim = input.substr(0, index);

			// Store the value in the animation node
			nodeS->setAnimTime(atoi(timeAnim.c_str()));
			nodeS->setShutterOpen(atoi(scaleAnim.c_str()));
			nodeS->setShutterRotate(atoi(rotateAnim.c_str()));

			// Insert the animation node into the animation list
			this->retAnimList()->insertNode(nodeS);
		}
		// Deal with the last animation node in the same way
		AnimationNodeShutter* nodeS = new AnimationNodeShutter();

		index = (int)input.find(' ');
		input = input.substr(index + 1);
		index = (int)input.find(' ');
		std::string timeAnim = input.substr(0, index);

		input = input.substr(index + 1);
		index = (int)input.find(' ');
		std::string scaleAnim = input.substr(0, index);

		input = input.substr(index + 1);
		std::string rotateAnim = input;

		nodeS->setAnimTime(atoi(timeAnim.c_str()));
		nodeS->setShutterOpen(atoi(scaleAnim.c_str()));
		nodeS->setShutterRotate(atoi(rotateAnim.c_str()));

		// Insert the animation node into the animation list
		this->retAnimList()->insertNode(nodeS);
	}
}


//----------------------------------------------------------------------------------------------
void ActionNodeShutter::executeNode(Scene* scene)
//----------------------------------------------------------------------------------------------
{
	// branch according to the application mode

	// First have to identify the shutter object with the same index, it is used to extract its section and floor
	bool found = false;
	int shutterCounter = 0;
	TransformNode* tempShutChT = NULL;
	ShutterInfo* tempSInfo = NULL;
	bool rotSet = false;

	for (int i = 0; i < scene->RootNodes.size(); i++) {
		BaseNode* tempN = scene->RootNodes[i];
		// Test if it is the node with the shutters
		if (CheckNodeNameCode(tempN->name.c_str(), CODE_WINDOW_SHUTTER)) {
			// Retype the node to the correct transform node
			TransformNode* tempT = static_cast<TransformNode*> (tempN);

			// Iterate through all its children and finde where the moving shutters are
			for (int j = 0; j < tempT->children.size(); j++) {
				BaseNode* tempCh = tempT->children[j];

				// Check if this the moving window node
				if (CheckNodeNameCode(tempCh->name.c_str(), CODE_SHUTTERS_MOVE)) {
					// Retype the node to the correct transform node
					TransformNode* tempChT = static_cast<TransformNode*> (tempCh);

					// Iterate through all the children and find the one, that we are looking for (the index has to match)
					for (int k = 0; k < tempChT->children.size(); k++) {
						// Get the current node
						BaseNode* tempShutCh = tempChT->children[k];

						if ((CheckNodeNameCode(tempShutCh->name.c_str(), CODE_SHUTTER))) {

							// Retype the node to the correct transform node
							tempShutChT = static_cast<TransformNode*> (tempShutCh);
							tempSInfo = static_cast<ShutterInfo*> (tempShutChT->retObjectInfo());

							if (this->retObjectIndex() == shutterCounter)
							{


								tempSInfo->setScaleRate(this->retShutterScaleRate());
								tempSInfo->setRotateRate(this->retShutterRotateRate());
								tempSInfo->setHasAnimated(this->retAnimFlag());

								if ((this->retShutterRotateRate() != 0) || (this->retShutterScaleRate() != 0)) {
									tempSInfo->setHasModified(true);
								}

								// If a new rotation rate was set, then apply it to the shutter object
								if (tempSInfo->retRotateRate() != 0) {
									setShutterRoll(tempShutChT, this->retShutterRotateRate());
									rotSet = true;
								}

								tempSInfo->copyAnimList(this->retAnimList());

								tempSInfo->retAnimationList()->setInterpMode(this->retInterpolation());

								// Set the flag, that we had opened the dialog
								found = true;
								break;
							}
							shutterCounter++;
						}
					}
				}
				if (found)
				{
					break;
				}
			}
		}
	}

	// Single shutter object
	if (this->retAppMode() == APPLY_SINGLE) {
		return;
	}

	// Whole section
	else if (this->retAppMode() == APPLY_SECTION) {

		// Iterate throught the list of shutters again and set the proper values for all shutters in the same section		
		for (int i = 0; i < scene->AllNodes.size(); i++) {
			// Point to the current node
			BaseNode* temp = scene->AllNodes[i];
			// We are only interested in nodes that define shutters
			if (CheckNodeNameCode(temp->name.c_str(), CODE_SHUTTER)) {
				// Retype the node to the transform node
				TransformNode* tempT = static_cast<TransformNode*> (temp);

				// Make sure it is not the current node
				if (tempT != tempShutChT) {

					ShutterInfo* tempSInfoAll = static_cast<ShutterInfo*> (tempT->retObjectInfo());

					// Check if the shutter is in the same section and floor
					if ((tempSInfo->retObjectFloor() == tempSInfoAll->retObjectFloor()) &&
						(tempSInfo->retObjectSection() == tempSInfoAll->retObjectSection())) {

						tempSInfoAll->setScaleRate(this->retShutterScaleRate());
						tempSInfoAll->setRotateRate(this->retShutterRotateRate());

						if ((this->retShutterScaleRate() != 0) || (this->retShutterRotateRate())) {
							tempSInfoAll->setHasModified(true);
						}

						tempSInfoAll->setHasAnimated(this->retAnimFlag());

						if (rotSet) {
							setShutterRoll(tempT, m_iRotateRate);
						}

						// Copy the animation list
						tempSInfoAll->copyAnimList(tempSInfo->retAnimationList());

						tempSInfoAll->retAnimationList()->setInterpMode(this->retInterpolation());
					}
				}
			}
		}

	}
	// Whole floor
	else if (this->retAppMode() == APPLY_FLOOR) {

		// Iterate throught the list of shutters again and set the proper values for all shutters in the same floor
		for (int i = 0; i < scene->AllNodes.size(); i++) {
			// Point to the current node
			BaseNode* temp = scene->AllNodes[i];
			// We are only interested in nodes that define shutters
			if (CheckNodeNameCode(temp->name.c_str(), CODE_SHUTTER)) {
				// Retype the node to the transform node
				TransformNode* tempT = static_cast<TransformNode*> (temp);

				// Make sure it is not the current node
				if (tempT != tempShutChT) {

					ShutterInfo* tempSInfoAll = static_cast<ShutterInfo*> (tempT->retObjectInfo());

					// Check if the shutter is in the same section and floor
					if (tempSInfo->retObjectFloor() == tempSInfoAll->retObjectFloor()) {

						tempSInfoAll->setScaleRate(this->retShutterScaleRate());
						tempSInfoAll->setRotateRate(this->retShutterRotateRate());

						if ((this->retShutterScaleRate() != 0) || (this->retShutterRotateRate())) {
							tempSInfoAll->setHasModified(true);
						}

						tempSInfoAll->setHasAnimated(this->retAnimFlag());

						if (rotSet) {
							setShutterRoll(tempT, m_iRotateRate);
						}

						// Copy the animation list
						tempSInfoAll->copyAnimList(tempSInfo->retAnimationList());

						tempSInfoAll->retAnimationList()->setInterpMode(this->retInterpolation());
					}
				}
			}
		}

	}
	// All windows
	else {

		// Iterate throught the list of shutters again
		for (int i = 0; i < scene->AllNodes.size(); i++) {
			// Point to the current node
			BaseNode* temp = scene->AllNodes[i];
			// We are only interested in nodes that define shutters
			if (CheckNodeNameCode(temp->name.c_str(), CODE_SHUTTER)) {
				// Retype the node to the transform node
				TransformNode* tempT = static_cast<TransformNode*> (temp);

				// Make sure it is not the current node
				if (tempT != tempShutChT){

					ShutterInfo* tempSInfoAll = static_cast<ShutterInfo*> (tempT->retObjectInfo());

					tempSInfoAll->setScaleRate(this->retShutterScaleRate());
					tempSInfoAll->setRotateRate(this->retShutterRotateRate());

					if ((this->retShutterScaleRate() != 0) || (this->retShutterRotateRate())){
						tempSInfoAll->setHasModified(true);
					}	

					tempSInfoAll->setHasAnimated(this->retAnimFlag());											

					if (rotSet){
						setShutterRoll(tempT, m_iRotateRate);
					}						

					// Copy the animation list
					tempSInfoAll->copyAnimList(tempSInfo->retAnimationList());

					tempSInfoAll->retAnimationList()->setInterpMode(this->retInterpolation());										
				}
			}
		}
	}
}




//----------------------------------------------------------------------------------------------
ActionNodeCamera::ActionNodeCamera()
//----------------------------------------------------------------------------------------------
{		
	this->m_bOn = false;
	this->m_bPBRTSelect = false;
	this->m_bAnimFlag = false;
	this->m_iInterpolation = 0;
	this->animList = new AnimationList();
};


//----------------------------------------------------------------------------------------------
void ActionNodeCamera::writeCameraNode(std::ofstream& out)
//----------------------------------------------------------------------------------------------
{
	out << "# Camera - Apply mode, camera index, on, PBRT Select, animated, interpolation mode, number of nodes, animation list(time, position, orientation, T, C, B) ...." 
		<< std::endl;
	out << "A_Camera " << this->retAppModeNum() << " " << this->retObjectIndex() << " ";

	if (this->m_bOn){
		out << "1 ";
	}
	else{
		out << "0 ";
	}

	if (this->m_bPBRTSelect){
		out << "1 ";
	}
	else{
		out << "0 ";
	}

	if (this->m_bAnimFlag){
		out << "1 ";
	}
	else{
		out << "0 ";
	}

	// If the camera is being animated - insert the animation list as well
	if (this->m_bAnimFlag){
		// Insert the interpolation mode
		out << this->m_iInterpolation << " " << this->retAnimList()->retNodeCount() << " ";

		AnimationNode * help = this->retAnimList()->retListHead();

		while (help != NULL){

			AnimationNodeCamera* helpC = static_cast<AnimationNodeCamera*> (help);

			out << helpC->retAnimTime() << " " << helpC->retCameraPosition().x << " " << helpC->retCameraPosition().y << " " <<
				helpC->retCameraPosition().z << " " << helpC->retCameraOrientation().x << " " << helpC->retCameraOrientation().y << " " <<
				helpC->retCameraOrientation().z << " " << helpC->retCameraOrientation().par << " " << helpC->retTension() << " " << 
				helpC->retContinuity() << " " << helpC->retBias() << " ";

			help = help->retAnimNext();
		}

		out << std::endl;

	}
	else{
		out << std::endl;
	}

}

//----------------------------------------------------------------------------------------------
void ActionNodeCamera::parseInput(std::string input)
//----------------------------------------------------------------------------------------------
{
	int index = (int)input.find(' ');
	std::string applyMode = input.substr(0, index);

	input = input.substr(index + 1);			
	index = (int)input.find(' ');
	std::string cameraIndex = input.substr(0, index);

	input = input.substr(index + 1);			
	index = (int)input.find(' ');
	std::string cameraOn = input.substr(0, index);

	input = input.substr(index + 1);			
	index = (int)input.find(' ');
	std::string cameraPBRTSelect = input.substr(0, index);

	input = input.substr(index + 1);			
	index = (int)input.find(' ');

	std::string animF;
	if (input.find(' ') == NOPE){
		animF = input;
	}
	else{
		animF = input.substr(0, index);
	}

	this->setAppMode(atoi(applyMode.c_str()));
	this->setObjectIndex(atoi(cameraIndex.c_str()));
	
	if (atoi(cameraOn.c_str()) == 0){
		this->setOn(false);
	}
	else{
		this->setOn(true);
	}

	if (atoi(cameraPBRTSelect.c_str()) == 0){
		this->setPBRTSelect(false);
	}
	else{
		this->setPBRTSelect(true);
	}

	if (atoi(animF.c_str()) == 0){
		this->setAnimFlag(false);
	}
	else{
		this->setAnimFlag(true);
	}

	// This node is animated -> extract the animation list
	if (this->retAnimFlag()){

		// Initialize the animation list of this actio node
		this->animList = new AnimationList();

		// Extract the interpolation mode
		index = (int)input.find(' ');
		input = input.substr(index + 1);			
		index = (int)input.find(' ');
		int interpMode = atoi(input.substr(0, index).c_str());

		this->animList->setInterpMode(interpMode);
		this->setInterpolation(interpMode);

		// First get the number of animation nodes
		input = input.substr(index + 1);			
		index = (int)input.find(' ');
		int nodesCount = atoi(input.substr(0, index).c_str());

		// extract all the nodes and deal separately with the last one
		for (int i = 0; i < nodesCount - 1; i++){

			AnimationNodeCamera * nodeC = new AnimationNodeCamera();			

			// Get the animation time
			index = (int)input.find(' ');
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string timeAnim = input.substr(0, index);

			// Get the camera position X
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string posX = input.substr(0, index);

			// Get the camera position Y
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string posY = input.substr(0, index);

			// Get the camera position Z
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string posZ = input.substr(0, index);

			// Get the camera orientation X
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string orientX = input.substr(0, index);

			// Get the camera orientation Y
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string orientY = input.substr(0, index);

			// Get the camera orientation Z
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string orientZ = input.substr(0, index);

			// Get the camera orientation Rot
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string orientRot = input.substr(0, index);

			// Get the KB Spline - Tension
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string KBTension = input.substr(0, index);

			// Get the KB Spline - Continuity
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string KBContinuity = input.substr(0, index);

			// Get the KB Spline - Bias
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string KBBias = input.substr(0, index);

			// Store the value in the animation node
			nodeC->setAnimTime(atoi(timeAnim.c_str()));
			
			vec3 camPos = vec3((float)atof(posX.c_str()), (float)atof(posY.c_str()), (float)atof(posZ.c_str()));
			nodeC->setCameraPosition(camPos);

			vec4 camOrient = vec4((float)atof(orientX.c_str()), (float)atof(orientY.c_str()), (float)atof(orientZ.c_str()), (float)atof(orientRot.c_str()));
			nodeC->setCameraOrientation(camOrient);

			nodeC->setTension((float)atof(KBTension.c_str()));
			nodeC->setContinuity((float)atof(KBContinuity.c_str()));
			nodeC->setBias((float)atof(KBBias.c_str()));

			// Insert the animation node into the animation list
			this->retAnimList()->insertNode(nodeC);
		}
		// Deal with the last animation node in the same way
		AnimationNodeCamera * nodeC = new AnimationNodeCamera();

		// Get the animation time
			index = (int)input.find(' ');
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string timeAnim = input.substr(0, index);

			// Get the camera position X
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string posX = input.substr(0, index);

			// Get the camera position Y
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string posY = input.substr(0, index);

			// Get the camera position Z
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string posZ = input.substr(0, index);

			// Get the camera orientation X
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string orientX = input.substr(0, index);

			// Get the camera orientation Y
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string orientY = input.substr(0, index);

			// Get the camera orientation Z
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string orientZ = input.substr(0, index);

			// Get the camera orientation Rot
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string orientRot = input.substr(0, index);

			// Get the KB Spline - Tension
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string KBTension = input.substr(0, index);

			// Get the KB Spline - Continuity
			input = input.substr(index + 1);			
			index = (int)input.find(' ');
			std::string KBContinuity = input.substr(0, index);

			// Get the KB Spline - Bias
			input = input.substr(index + 1);						
			std::string KBBias = input;

			// Store the value in the animation node
			nodeC->setAnimTime(atoi(timeAnim.c_str()));
			
			vec3 camPos = vec3((float)atof(posX.c_str()), (float)atof(posY.c_str()), (float)atof(posZ.c_str()));
			nodeC->setCameraPosition(camPos);

			vec4 camOrient = vec4((float)atof(orientX.c_str()), (float)atof(orientY.c_str()), (float)atof(orientZ.c_str()), (float)atof(orientRot.c_str()));
			nodeC->setCameraOrientation(camOrient);

			nodeC->setTension((float)atof(KBTension.c_str()));
			nodeC->setContinuity((float)atof(KBContinuity.c_str()));
			nodeC->setBias((float)atof(KBBias.c_str()));

		// Insert the animation node into the animation list
		this->retAnimList()->insertNode(nodeC);
	}
}


//----------------------------------------------------------------------------------------------
void ActionNodeCamera::executeNode(Scene* scene)
//----------------------------------------------------------------------------------------------
{	
	// We have to identify the camera object with the same index
	if (scene->Cameras.size() == 0)
	{
		std::cout << "no cameras, there is no camera to animate from this scene" << std::endl;
		return;
	}
	scene->Cameras[this->retObjectIndex()]->m_bIsOn = this->retOn();

	// Deal with the PBRT selected camera, select the current one and make sure it is the only one selected
	if (this->retPBRTSelect()){
		scene->Cameras[this->retObjectIndex()]->setPBRTSelect(true);

		// Turn off all the other ones
		for (int i = 0; i < scene->Cameras.size(); i++){
			ViewPointNode * temp = scene->Cameras[i];

			if (i != this->retObjectIndex()){
				temp->setPBRTSelect(false);
			}
		}

		// Also make sure that the PBRT selected camera is on
		scene->Cameras[this->retObjectIndex()]->m_bIsOn = true;
	}

	scene->Cameras[this->retObjectIndex()]->m_bHasAnimated = this->retAnimFlag();

	scene->Cameras[this->retObjectIndex()]->copyAnimList(this->retAnimList());
	scene->Cameras[this->retObjectIndex()]->animList->setInterpMode(this->retInterpolation());
}


