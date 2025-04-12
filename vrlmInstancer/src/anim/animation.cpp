/**
@file animation.cpp
Definitions of functions for animating the scene

@author Copyright (C) 2009 Ondrej Linda
*/


#include "animation.h"
#include "error.h"

/// Global animation object, storing the information about the set animation

AnimationInfo animInfo;

AnimationList::~AnimationList(){
	AnimationNode * help1 = this->head;
	AnimationNode * help2 = this->head;

	while (help1 != NULL){
		help1 = help1->retAnimNext();
		delete  help2;
		help2 = help1;
	}
}

bool AnimationList::insertNode(AnimationNode *node){

	// if this the first node in the list, insert it to the head pf the list
	if (this->head == NULL){
		this->head = node;
		this->last = node;		
		this->nodeCount++;
		return true;
	}

	// Else we have to find the right position for the node
	AnimationNode * help = this->head;
	while ((help->retAnimTime() < node->retAnimTime()) && (help->retAnimNext() != NULL)){
		help = help->retAnimNext();
		
	}

	if (node->retAnimTime() == help->retAnimTime()){
		Error("Animation node at this time already exists in the list. ");
		return false;
	}

	// In this case the new node is supposed to be the head of the list
	if (help == this->head){
		if (node->retAnimTime() < head->retAnimTime()){

			if (this->head->retAnimNext() == NULL){
				node->setAnimNext(this->head);
				this->head->setAnimPrev(node);
				this->last = this->head;
				this->head = node;
			}
			else{
				node->setAnimNext(this->head);
				this->head->setAnimPrev(node);
				this->head = node;
			}
		}
		else{
			if (this->head->retAnimNext() == NULL){
				this->head->setAnimNext(node);
				node->setAnimPrev(this->head);
				this->last = node;
			}
			else{
				this->head->setAnimNext(node);
				node->setAnimPrev(this->head);
			}
			
		}

		this->nodeCount++;
		return true;	
	}

	// In this case, the new node is the very last node in the list
	if ((help->retAnimNext() == NULL) && (help->retAnimTime() < node->retAnimTime())){
		help->setAnimNext(node);
		node->setAnimPrev(help);
		this->last = node;
		this->nodeCount++;
		return true;
	}

	// In this case the new node is somewhere in the list, insert it
	node->setAnimNext(help);	
	node->setAnimPrev(help->retAnimPrev());
	help->setAnimPrev(node);
	node->retAnimPrev()->setAnimNext(node);
	
	this->nodeCount++;
	return true;
}

bool AnimationList::deleteNode(int index){

	// In case the index is more then the actual number of nodes
	if (index > (this->nodeCount - 1)){
		return false;
	}

	// In case there is only a single node in the list
	if (this->nodeCount == 1){
		delete this->head;
		this->head = NULL;
		this->last = NULL;
		this->nodeCount = 0;
		return true;
	}

	// In case we are deleting the first node
	if (index == 0){		
		head = head->retAnimNext();
		delete head->retAnimPrev();
		head->setAnimPrev(NULL);		
		
		this->nodeCount--;
		return true;
	}

	// In case we are deliting the very last node
	if (index == this->nodeCount - 1){
		AnimationNode * help = this->last;
		this->last = this->last->retAnimPrev();
		this->last->setAnimNext(NULL);
		delete help;
		this->nodeCount--;
		return true;
	}

	int i = 0;
	AnimationNode * help = this->head;	

	while(i < index){		
		help = help->retAnimNext();
		i++;
	}
	
	help->retAnimPrev()->setAnimNext(help->retAnimNext());
	help->retAnimNext()->setAnimPrev(help->retAnimPrev());
	delete help;

	this->nodeCount--;
	return true;
}

//----------------------------------------------------------------------------------------------
void AnimationList::setHandlesDoorAuto(void)
//----------------------------------------------------------------------------------------------
{

	// First go through the whole list and remove previously added automatic nodes
	AnimationNode * helpD = this->head;

	while(helpD != NULL){
		if (helpD->retAutoNode()){
			
			if (helpD == this->head){
				helpD = helpD->retAnimNext();

				delete helpD->retAnimPrev();

				helpD->setAnimPrev(NULL);
				this->head = helpD;

				this->nodeCount--;
			}
			else if (helpD == this->last){
				helpD = helpD->retAnimPrev();

				delete helpD->retAnimNext();

				helpD->setAnimNext(NULL);
				this->last = helpD;

				this->nodeCount--;

				helpD = NULL;
			}
			else{
				AnimationNode * helpD2 = helpD->retAnimNext();

				helpD->retAnimPrev()->setAnimNext(helpD->retAnimNext());
				helpD->retAnimNext()->setAnimPrev(helpD->retAnimPrev());

				delete helpD;

				this->nodeCount--;

				helpD = helpD2;
			}
		}
		else{
			helpD = helpD->retAnimNext();
		}
	}


	AnimationNode * help = this->head;

	AnimationNodeDoor* tempD = static_cast<AnimationNodeDoor*> (help);

	// Add the first node
	if (tempD->retDoorOpen() == 0){

		tempD->setHandleRate(50);

		AnimationNodeDoor * node = new AnimationNodeDoor();

		// Set the correct parameters
		node->setAnimTime(tempD->retAnimTime() - animInfo.retHandlesDuration());
		node->setDoorOpen(0);
		node->setHandleRate(0);
		node->setAutoNode(true);
		
		this->insertNode(node);
		
	}

	help = help->retAnimNext();

	while(help != NULL){

		AnimationNodeDoor* tempD1 = static_cast<AnimationNodeDoor*> (help->retAnimPrev());
		AnimationNodeDoor* tempD2 = static_cast<AnimationNodeDoor*> (help);

		if ((tempD2->retDoorOpen() == 0) && (tempD1->retDoorOpen() == 0)){
			if ((help->retAnimTime() - help->retAnimPrev()->retAnimTime()) > animInfo.retHandlesDuration()){

				tempD2->setHandleRate(50);

				AnimationNodeDoor * node = new AnimationNodeDoor();

				// Set the correct parameters
				node->setAnimTime(tempD2->retAnimTime() - animInfo.retHandlesDuration());
				node->setDoorOpen(0);
				node->setHandleRate(0);
				node->setAutoNode(true);
				
				this->insertNode(node);
			}
			else{
				tempD2->setHandleRate(50);
			}
		}

		help = help->retAnimNext();
		
	}
}


//----------------------------------------------------------------------------------------------
void AnimationList::setHandlesWindowsAuto(void)
//----------------------------------------------------------------------------------------------
{
	// First go through the whole list and remove previously added automatic nodes
	AnimationNode * helpW = this->head;

	while(helpW != NULL){
		if (helpW->retAutoNode()){
			
			if (helpW == this->head){
				helpW = helpW->retAnimNext();

				delete helpW->retAnimPrev();

				helpW->setAnimPrev(NULL);
				this->head = helpW;

				this->nodeCount--;
			}
			else if (helpW == this->last){
				helpW = helpW->retAnimPrev();

				delete helpW->retAnimNext();

				helpW->setAnimNext(NULL);
				this->last = helpW;

				this->nodeCount--;

				helpW = NULL;
			}
			else{
				AnimationNode * helpW2 = helpW->retAnimNext();

				helpW->retAnimPrev()->setAnimNext(helpW->retAnimNext());
				helpW->retAnimNext()->setAnimPrev(helpW->retAnimPrev());

				delete helpW;

				this->nodeCount--;

				helpW = helpW2;
			}
		}
		else{
			helpW = helpW->retAnimNext();
		}
	}	

	AnimationNode * help = this->head;

	AnimationNodeWindow* tempW = static_cast<AnimationNodeWindow*> (help);

	// Add the first node
	if (tempW->retWindowOpen() == 0){

		if (tempW->retOpenType() == 0){
			tempW->setHandleRate(50);
		}
		else{
			tempW->setHandleRate(100);
		}

		AnimationNodeWindow * node = new AnimationNodeWindow();

		// Set the correct parameters
		node->setAnimTime(tempW->retAnimTime() - animInfo.retHandlesDuration());
		node->setWindowOpen(0);
		node->setHandleRate(0);
		node->setAutoNode(true);
		node->setOpenType(tempW->retOpenType());
		
		this->insertNode(node);		
	}

	help = help->retAnimNext();

	while(help != NULL){

		AnimationNodeWindow* tempW1 = static_cast<AnimationNodeWindow*> (help->retAnimPrev());
		AnimationNodeWindow* tempW2 = static_cast<AnimationNodeWindow*> (help);

		if ((tempW2->retWindowOpen() == 0) && (tempW1->retWindowOpen() == 0)){
			if ((help->retAnimTime() - help->retAnimPrev()->retAnimTime()) > animInfo.retHandlesDuration()){

				tempW2->setHandleRate(50);

				AnimationNodeWindow * node = new AnimationNodeWindow();

				// Set the correct parameters
				node->setAnimTime(tempW2->retAnimTime() - animInfo.retHandlesDuration());
				node->setWindowOpen(0);
				node->setHandleRate(0);
				node->setAutoNode(true);
				node->setOpenType(tempW2->retOpenType());
				
				this->insertNode(node);
			}
			else{
				tempW2->setHandleRate(50);
			}
		}
		else if((tempW2->retWindowOpen() == 0) && (tempW1->retWindowOpen() > 0)){
			bool addNew = false;
			if (help->retAnimNext() != NULL){
				if ((help->retAnimNext()->retAnimTime() - help->retAnimTime()) > animInfo.retHandlesDuration()){
					addNew = true;
				}
			}
			else{
				if ((animInfo.retAnimLength() * animInfo.retAnimFps() - help->retAnimTime()) > animInfo.retHandlesDuration()){
					addNew = true;
				}

			}
			if (addNew){

				if (tempW2->retOpenType() == 0){
					tempW2->setHandleRate(50);
				}
				else{
					tempW2->setHandleRate(100);
				}

				AnimationNodeWindow * node = new AnimationNodeWindow();

				// Set the correct parameters
				node->setAnimTime(tempW2->retAnimTime() + animInfo.retHandlesDuration());
				node->setWindowOpen(0);
				node->setHandleRate(0);
				node->setAutoNode(true);
				node->setOpenType(tempW2->retOpenType());
				
				this->insertNode(node);
			}
			else{				
				if (tempW2->retOpenType() == 0){
					tempW2->setHandleRate(50);
				}
				else{
					tempW2->setHandleRate(100);
				}
			}
		}
		else if (tempW2->retWindowOpen() > 0){
			if (tempW2->retOpenType() == 0){
				tempW2->setHandleRate(50);
			}
			else{
				tempW2->setHandleRate(100);
			}
		}

		help = help->retAnimNext();

		// Iterate to the next node, which is not the automatic node
		bool good = false;
		while (!good){
			if (help == NULL){
				good = true;
			}
			else if (help->retAutoNode()){
				help = help->retAnimNext();				
			}
			else{
				good = true;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
AnimationNodeDoor::AnimationNodeDoor() : AnimationNode() 
//----------------------------------------------------------------------------------------------
{
	// Set the initial values
	doorOpen = 0;	
	doorHandle = 0;
}

//----------------------------------------------------------------------------------------------
AnimationNodeShutter::AnimationNodeShutter() : AnimationNode() 
//----------------------------------------------------------------------------------------------
{
	// Set the initial values
	shutterOpen = 0;	
	shutterRotate = 0;
}

//----------------------------------------------------------------------------------------------
AnimationNodeWindow::AnimationNodeWindow() : AnimationNode() 
//----------------------------------------------------------------------------------------------
{
	// Set the initial values
	windowOpen = 0;	
}

//----------------------------------------------------------------------------------------------
AnimationNodeCamera::AnimationNodeCamera() : AnimationNode() 
//----------------------------------------------------------------------------------------------
{
	// Set the initial values
	this->cameraPosition.x = 0.0;
	this->cameraPosition.y = 0.0;
	this->cameraPosition.z = 0.0;

	this->cameraOrientation = vec4(0.0, 0.0, 0.0, 0.0);

	this->tension = 0.0;
	this->continuity = 0.0;
	this->bias = 0.0;
}

//----------------------------------------------------------------------------------------------
AnimationNodeLight::AnimationNodeLight() : AnimationNode() 
//----------------------------------------------------------------------------------------------
{
	// Set the initial values
	lightOn = true;
	lightIntensity = 100;
}


