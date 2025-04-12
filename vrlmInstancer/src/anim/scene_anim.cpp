/**
@file scene.cpp
Soubor s definicemi trid popisujicich scenu.

@author Copyright (C) 2008 Jiri Drahokoupil
*/

#define _USE_MATH_DEFINES
#include "scene.h"
#include "matrix.h"
#include "error.h"
#include "constants.h"
#include "geometryModify.h"
#include "animation.h"
#include <cmath>
# define M_PI           3.14159265358979323846

//----------------------------------------------------------------------------------------------
float linearInterp(float a, float b, float x)
//----------------------------------------------------------------------------------------------
{
	float out;
	out = a + (b - a) * x;

	return out;
}

//----------------------------------------------------------------------------------------------
float cosineInterp(float a, float b, float x)
//----------------------------------------------------------------------------------------------
{
	float ft = (float)(x * M_PI);
	float f = (float)((1 - cos(ft)) * 0.5);

	float out;
	out = a * (1 - f) + b * f;

	return out;
}

//----------------------------------------------------------------------------------------------
float cubicInterp(float v0, float v1, float v2, float v3, float x)
//----------------------------------------------------------------------------------------------
{
	float P = (v3 - v2) - (v0 - v1);
	float Q = (v0 - v1) - P;
	float R = v2 - v0;
	float S = v1;

	float out;
	out = P * x * x * x + Q * x * x + R * x + S;

	return out;
}

//----------------------------------------------------------------------------------------------
float KBSplineInterp(float v0, float v1, float v2, float v3, float x, float t, float c, float b) {
	//----------------------------------------------------------------------------------------------

	float h1 = 2 * pow(x, 3.0f) - 3 * pow(x, 2.0f) + 1;
	float h2 = (-2) * pow(x, 3.0f) + 3 * pow(x, 2.0f);
	float h3 = pow(x, 3.0f) - 2 * pow(x, 2.0f) + x;
	float h4 = pow(x, 3.0f) - pow(x, 2.0f);

	float TDix = (float)((1 - t) * (1 + c) * (1 + b) * (v1 - v0) /
		2.0 + (1 - t) * (1 - c) * (1 - b) * (v2 - v1) / 2.0);

	float TSix = (float)((1 - t) * (1 - c) * (1 + b) * (v2 - v1) /
		2.0 + (1 - t) * (1 + c) * (1 - b) * (v3 - v2) / 2.0);

	float value = h1 * v1 + h2 * v2 + h3 * TDix + h4 * TSix;

	return value;
}


//----------------------------------------------------------------------------------------------
ObjectFloor retFloor(int n)
//----------------------------------------------------------------------------------------------
{
	switch (n)
	{
	case 0:
		return _G_FLOOR;
		break;
	case 1:
		return _1_FLOOR;
		break;
	case 2:
		return _2_FLOOR;
		break;
	case 3:
		return _3_FLOOR;
		break;
	case 4:
		return _4_FLOOR;
		break;
	case 5:
		return _5_FLOOR;
		break;
	case 6:
		return _6_FLOOR;
		break;
	case 7:
		return _7_FLOOR;
		break;
	case 8:
		return _8_FLOOR;
		break;
	case 9:
		return _9_FLOOR;
		break;
	case 10:
		return _10_FLOOR;
		break;
	default:
		return _FLOOR_NDEF;
		break;
	}
}


//----------------------------------------------------------------------------------------------
ObjectSection retSection(int n)
//----------------------------------------------------------------------------------------------
{
	switch (n)
	{
	case 1:
		return _1_SECTION;
		break;
	case 2:
		return _2_SECTION;
		break;
	case 3:
		return _3_SECTION;
		break;
	case 4:
		return _4_SECTION;
		break;
	case 5:
		return _5_SECTION;
		break;
	case 6:
		return _6_SECTION;
		break;
	case 7:
		return _7_SECTION;
		break;
	case 8:
		return _8_SECTION;
		break;
	case 9:
		return _9_SECTION;
		break;
	case 10:
		return _10_SECTION;
		break;
	default:
		return _SECTION_NDEF;
		break;
	}
}


//----------------------------------------------------------------------------------------------
int ObjectInfo::retObjectFloorNumber()
//----------------------------------------------------------------------------------------------
{
	switch(objectFloor){
		case _FLOOR_NDEF:
			return -2;
			break;
		case _U_FLOOR:
			return -1;
			break;
		case _G_FLOOR:
			return 0;
			break;
		case _1_FLOOR:
			return 1;
			break;
		case _2_FLOOR:
			return 2;
			break;
		case _3_FLOOR:
			return 3;
			break;
		case _4_FLOOR:
			return 4;
			break;
		case _5_FLOOR:
			return 5;
			break;
		case _6_FLOOR:
			return 6;
			break;
		case _7_FLOOR:
			return 7;
			break;
		case _8_FLOOR:
			return 8;
			break;
		case _9_FLOOR:
			return 9;
			break;
		case _10_FLOOR:
			return 10;
			break;
		default:
			return -1;
			break;
	}
}


//----------------------------------------------------------------------------------------------
int ObjectInfo::retObjectSectionNumber()
//----------------------------------------------------------------------------------------------
{
	switch(objectSection){
		case _SECTION_NDEF:
			return -1;
			break;
		case _1_SECTION:
			return 1;
			break;
		case _2_SECTION:
			return 2;
			break;
		case _3_SECTION:
			return 3;
			break;
		case _4_SECTION:
			return 4;
			break;
		case _5_SECTION:
			return 5;
			break;
		case _6_SECTION:
			return 6;
			break;
		case _7_SECTION:
			return 7;
			break;
		case _8_SECTION:
			return 8;
			break;
		case _9_SECTION:
			return 9;
			break;
		case _10_SECTION:
			return 10;
			break;
		default:
			return -1;
			break;
	}
}

//----------------------------------------------------------------------------------------------
int ObjectInfo::retObjectConstructTypeNumber()
//----------------------------------------------------------------------------------------------
{
	switch(objectConstructType){
		case _TYPE_CONSTRUCT_NDEF:
			return -1;
			break;
		case _TYPE_CONSTRUCT_1:
			return 1;
			break;
		case _TYPE_CONSTRUCT_2:
			return 2;
			break;
		case _TYPE_CONSTRUCT_3:
			return 3;
			break;		
		case _TYPE_CONSTRUCT_4:
			return 4;
			break;
		case _TYPE_CONSTRUCT_5:
			return 5;
			break;
		case _TYPE_CONSTRUCT_6:
			return 6;
			break;
		case _TYPE_CONSTRUCT_7:
			return 7;
			break;
		case _TYPE_CONSTRUCT_8:
			return 8;
			break;
		case _TYPE_CONSTRUCT_9:
			return 9;
			break;
		case _TYPE_CONSTRUCT_10:
			return 10;
			break;
		case _TYPE_CONSTRUCT_11:
			return 11;
			break;
		case _TYPE_CONSTRUCT_12:
			return 12;
			break;
		case _TYPE_CONSTRUCT_13:
			return 13;
			break;
		case _TYPE_CONSTRUCT_14:
			return 14;
			break;
		case _TYPE_CONSTRUCT_15:
			return 15;
			break;
		case _TYPE_CONSTRUCT_16:
			return 16;
			break;
		case _TYPE_CONSTRUCT_17:
			return 17;
			break;
		case _TYPE_CONSTRUCT_18:
			return 18;
			break;
		case _TYPE_CONSTRUCT_19:
			return 19;
			break;
		case _TYPE_CONSTRUCT_20:
			return 20;
			break;
		case _TYPE_CONSTRUCT_21:
			return 21;
			break;
		case _TYPE_CONSTRUCT_22:
			return 22;
			break;
		case _TYPE_CONSTRUCT_23:
			return 23;
			break;
		case _TYPE_CONSTRUCT_24:
			return 24;
			break;
		case _TYPE_CONSTRUCT_25:
			return 25;
			break;
		case _TYPE_CONSTRUCT_26:
			return 26;
			break;
		case _TYPE_CONSTRUCT_27:
			return 27;
			break;
		default:
			return -1;
			break;
	}
}


//----------------------------------------------------------------------------------------------
void ObjectInfo::setObjectFloorNumber(int nn)
//----------------------------------------------------------------------------------------------
{
	switch(nn){
		case -1:
			objectFloor = _U_FLOOR;
			break;
		case 0:
			objectFloor = _G_FLOOR;
			break;
		case 1:
			objectFloor = _1_FLOOR;
			break;
		case 2:
			objectFloor = _2_FLOOR;
			break;
		case 3:
			objectFloor = _3_FLOOR;
			break;
		case 4:
			objectFloor = _4_FLOOR;
			break;
		case 5:
			objectFloor = _5_FLOOR;
			break;
		case 6:
			objectFloor = _6_FLOOR;
			break;
		case 7:
			objectFloor = _7_FLOOR;
			break;
		case 8:
			objectFloor = _8_FLOOR;
			break;
		case 9:
			objectFloor = _9_FLOOR;
			break;
		case 10:
			objectFloor = _10_FLOOR;
			break;
		default:
			objectFloor = _FLOOR_NDEF;
			break;
	}
}


//----------------------------------------------------------------------------------------------
void ObjectInfo::setObjectSectionNumber(int ns)
//----------------------------------------------------------------------------------------------
{
	switch(ns){
		case 1:
			objectSection = _1_SECTION;
			break;
		case 2:
			objectSection = _2_SECTION;
			break;
		case 3:
			objectSection = _3_SECTION;
			break;
		case 4:
			objectSection = _4_SECTION;
			break;
		case 5:
			objectSection = _5_SECTION;
			break;
		case 6:
			objectSection = _6_SECTION;
			break;
		case 7:
			objectSection = _7_SECTION;
			break;
		case 8:
			objectSection = _8_SECTION;
			break;
		case 9:
			objectSection = _9_SECTION;
			break;
		case 10:
			objectSection = _10_SECTION;
			break;
		default:
			objectSection = _SECTION_NDEF;
			break;
	}
}

//----------------------------------------------------------------------------------------------
void ObjectInfo::setObjectConstructTypeNumber(int typeNum)
//----------------------------------------------------------------------------------------------
{
	switch(typeNum){
		case 1:
			objectConstructType = _TYPE_CONSTRUCT_1;
			break;
		case 2:
			objectConstructType = _TYPE_CONSTRUCT_2;
			break;
		case 3:
			objectConstructType = _TYPE_CONSTRUCT_3;
			break;
		case 4:
			objectConstructType = _TYPE_CONSTRUCT_4;
			break;
		case 5:
			objectConstructType = _TYPE_CONSTRUCT_5;
			break;
		case 6:
			objectConstructType = _TYPE_CONSTRUCT_6;
			break;
		case 7:
			objectConstructType = _TYPE_CONSTRUCT_7;
			break;
		case 8:
			objectConstructType = _TYPE_CONSTRUCT_8;
			break;
		case 9:
			objectConstructType = _TYPE_CONSTRUCT_9;
			break;
		case 10:
			objectConstructType = _TYPE_CONSTRUCT_10;
			break;
		case 11:
			objectConstructType = _TYPE_CONSTRUCT_11;
			break;
		case 12:
			objectConstructType = _TYPE_CONSTRUCT_12;
			break;
		case 13:
			objectConstructType = _TYPE_CONSTRUCT_13;
			break;
		case 14:
			objectConstructType = _TYPE_CONSTRUCT_14;
			break;
		case 15:
			objectConstructType = _TYPE_CONSTRUCT_15;
			break;
		case 16:
			objectConstructType = _TYPE_CONSTRUCT_16;
			break;
		case 17:
			objectConstructType = _TYPE_CONSTRUCT_17;
			break;
		case 18:
			objectConstructType = _TYPE_CONSTRUCT_18;
			break;
		case 19:
			objectConstructType = _TYPE_CONSTRUCT_19;
			break;
		case 20:
			objectConstructType = _TYPE_CONSTRUCT_20;
			break;
		case 21:
			objectConstructType = _TYPE_CONSTRUCT_21;
			break;
		case 22:
			objectConstructType = _TYPE_CONSTRUCT_22;
			break;
		case 23:
			objectConstructType = _TYPE_CONSTRUCT_23;
			break;
		case 24:
			objectConstructType = _TYPE_CONSTRUCT_24;
			break;
		case 25:
			objectConstructType = _TYPE_CONSTRUCT_25;
			break;
		case 26:
			objectConstructType = _TYPE_CONSTRUCT_26;
			break;
		case 27:
			objectConstructType = _TYPE_CONSTRUCT_27;
			break;
		default:
			objectConstructType = _TYPE_CONSTRUCT_NDEF;
			break;				
	}
}

//----------------------------------------------------------------------------------------------
ObjectConstructType retOCType(int n)
//----------------------------------------------------------------------------------------------
{
	switch(n)
	{
	case 1:
		return _TYPE_CONSTRUCT_1;
		break;
	case 2:
		return _TYPE_CONSTRUCT_2;
		break;
	case 3:
		return _TYPE_CONSTRUCT_3;
		break;
	case 4:
		return _TYPE_CONSTRUCT_4;
		break;
	case 5:
		return _TYPE_CONSTRUCT_5;
		break;
	case 6:
		return _TYPE_CONSTRUCT_6;
		break;
	case 7:
		return _TYPE_CONSTRUCT_7;
		break;
	case 8:
		return _TYPE_CONSTRUCT_8;
		break;
	case 9:
		return _TYPE_CONSTRUCT_9;
		break;
	case 10:
		return _TYPE_CONSTRUCT_10;
		break;	
	case 11:
		return _TYPE_CONSTRUCT_11;
		break;	
	case 12:
		return _TYPE_CONSTRUCT_12;
		break;	
	case 13:
		return _TYPE_CONSTRUCT_13;
		break;	
	case 14:
		return _TYPE_CONSTRUCT_14;
		break;	
	case 15:
		return _TYPE_CONSTRUCT_15;
		break;	
	case 16:
		return _TYPE_CONSTRUCT_16;
		break;	
	case 17:
		return _TYPE_CONSTRUCT_17;
		break;	
	case 18:
		return _TYPE_CONSTRUCT_18;
		break;	
	case 19:
		return _TYPE_CONSTRUCT_19;
		break;	
	case 20:
		return _TYPE_CONSTRUCT_20;
		break;	
	case 21:
		return _TYPE_CONSTRUCT_21;
		break;	
	case 22:
		return _TYPE_CONSTRUCT_22;
		break;	
	case 23:
		return _TYPE_CONSTRUCT_23;
		break;	
	case 24:
		return _TYPE_CONSTRUCT_24;
		break;	
	case 25:
		return _TYPE_CONSTRUCT_25;
		break;	
	case 26:
		return _TYPE_CONSTRUCT_26;
		break;	
	case 27:
		return _TYPE_CONSTRUCT_27;
		break;	
	default:
		return _TYPE_CONSTRUCT_NDEF;
		break;
	}
}

//----------------------------------------------------------------------------------------------
void ObjectInfo::setObjectName(const char* n)
//----------------------------------------------------------------------------------------------
{
     if(name!=NULL) {
	  delete [] name;
	  name = 0;
     }
	if(n!=NULL) {
		int numbChar = (int)strlen(n);
		name = new char[numbChar+1];
		strcpy_s(name, numbChar+1, n);
	}
	else
	  name = NULL;
}

//----------------------------------------------------------------------------------------------
void ObjectInfo::clearAnimList(void)
//----------------------------------------------------------------------------------------------
{
	delete this->animList;
	this->animList = new AnimationList();
}

//----------------------------------------------------------------------------------------------
void ObjectInfo::correctObjectName(const char * name)
//----------------------------------------------------------------------------------------------
{
	const char* nName = name;
	// Test if the name is not NULL
	if(nName!=NULL) {
		int stringLength = (int)strlen(nName);
		// According to the definition there has to be 4x '_' + min(1x 'S' && 1x 'F' && 1x 'N' && 1x 'T')
		// Check the string and find the number of particular characters
		int numOf_ = 0;
		int numOfS = 0;
		int numOfF = 0;
		int numOfN = 0;
		int numOfT = 0;
		for(int i = 0; i < stringLength; i++) {
			char ch = nName[i];
			switch(ch) {
				case '_': 
					numOf_++;
					break;
				case 'S': 
				case 's':
					numOfS++;
					break;
				case 'F': 
				case 'f':
					numOfF++;
					break;
				case 'N': 
				case 'n':
					numOfN++;
					break;
				case 'T':
				case 't':
					numOfT++;
					break;
				default:
					break;
			}
		}

		bool nameMaybeValid = false;

		if(numOf_>=4 && numOfS >=1 && numOfF>=1 && numOfN>=1 && numOfT >=1){
			nameMaybeValid = true;			
		}

		// If the node's name can be correct - check it
		// Otherwise leave the dafualt values
		if (nameMaybeValid){
			//printf("1 .. nName = %s\n", nName);
		  
			// Find the first occurance of character "_F"
		        int posF = (int)(strstr(nName,"_F") - nName);
			// Pred timto prvnim vyskytem vse ponecham jako nove jmeno
			// a oriznu aktualni jmeno uzlu na kratsi verzi
			int length1 = posF + 5;
			char* newName = new char[length1];
			for(int i=0;i<posF;i++)
			  newName[i] = nName[i]; // copy the initial string
			
			// section
			int posS = (int)(strstr(nName,"_S") - nName);
			char str1[10];
			bool flag = 0;
			int i;
			for (i = 0; i < 10; i++){
			  str1[i] = nName[posS + i + 2];
			  if (str1[i] == '_') {flag = 1; str1[i]=0; break;}
			} // for i
			if (!flag) {
			  printf("Problem parsing the info about object - section \n");
			  abort();
			}
			int sN = atoi(str1);
			if ((sN < 0)&&(sN > 10)){
				Error("sN index out of bounds");
			}
			//assert((sN>= 0)&&(sN <= 10));
			//printf("str1 = %s posS = %d sN=%d\n", str1, posS, sN);
			// floor
			flag = 0;
			for (i = 0; i < 10; i++){
			  str1[i] = nName[posF + i + 2];
			  if (str1[i] == '_') {flag = 1; str1[i]=0; break;}
			} // for i
			if (!flag) {
			  printf("Problem parsing the info about object - floor\n");
			  abort();
			}
			int fN = atoi(str1);
			if((fN < -1)&&(sN > 7)){
				Error("fN index out of bounds");
			}			
			//printf("2 .. nName = %s\n", nName);
			
			// type
			int posT = (int)(strstr(nName,"_T") - nName);
			flag = 0;
			for (i = 0; i < 10; i++){
			  str1[i] = nName[posT + i + 2];
			  if (str1[i] == '_') {flag = 1; str1[i]=0; break;}
			} // for i
			if (!flag) {
			  printf("Problem parsing the info about object - floor\n");
			  abort();
			}
			int dT = atoi(str1);
			if ((dT < -1)&&(dT > 30)){
				Error("dT index out of bounds");
			}
			//assert((dT>= -1)&&(dT <= 30));
							
			//printf("nName = %s posF = %d newName = %s\n", nName, posF, newName);
			//printf("fN = %d sN = %d lT = %d\n", fN, sN, lT);
			int posLNumber = (int)(strstr(nName,"_N") - nName);
			
			// Put the number behind the name of the object and actualize the name
			newName[posF] = '_';
			posF++;
			posLNumber += 2;
			//printf("posL = %d posF = %d\n", posLNumber, posF);
			for(i = 0; i < length1 ; i++) {
			  newName[posF+i] = nName[posLNumber+i];
			  //printf("%c\n", newName[posF+i]);
			  if (newName[posF+i] == 0)
			    break;
			} // for i;
			//printf("3 .. nName = %s\n", nName);
			
			//printf("nName = %s posF = %d newName = %s\n", nName, posF, newName);
			//printf("fN = %d sN = %d lT = %d\n", fN, sN, dT);
			this->setObjectName(newName); 
			delete [] newName;
			//printf("3 .. nName = %s\n", nName);
			
			objectFloor = retFloor(fN);
			objectSection = retSection(sN);
			objectConstructType = retOCType(dT);				
		}
		else{
			this->setObjectName(name);
			objectFloor = _FLOOR_NDEF;
			objectSection = _SECTION_NDEF;
			objectConstructType = _TYPE_CONSTRUCT_NDEF;
		}
	}
	else {
		this->setObjectName(name);
		objectFloor = _FLOOR_NDEF;
		objectSection = _SECTION_NDEF;
		objectConstructType = _TYPE_CONSTRUCT_NDEF;
	}
}


//----------------------------------------------------------------------------------------------
DoorInfo::DoorInfo() : ObjectInfo(_TYPE_INFO_DOOR) 
//----------------------------------------------------------------------------------------------
{
	// Set the initial values
	mRotateDeg = 0;
	mHandleRate = 0;

}

//----------------------------------------------------------------------------------------------
void DoorInfo::copyAnimList(AnimationList * list)
//----------------------------------------------------------------------------------------------
{
	this->clearAnimList();

	this->retAnimationList()->setInterpMode(list->retInterpMode());

	// If the provided list is empty, there is nothing to copy
	if (list->retNodeCount() == 0){
		
		return;
	}

	// Iterate through the list and create copies of the nodes
	AnimationNode * help = list->retListHead();

	while (help != NULL){
		AnimationNodeDoor * tempD = static_cast<AnimationNodeDoor*> (help);

		AnimationNodeDoor * newNodeDoor = new AnimationNodeDoor();

		newNodeDoor->setAnimTime(tempD->retAnimTime());
		newNodeDoor->setDoorOpen(tempD->retDoorOpen());
		newNodeDoor->setHandleRate(tempD->retHandleRate());

		// Insert the node to the list
		this->retAnimationList()->insertNode(newNodeDoor);

		help = help->retAnimNext();
	}
}

//----------------------------------------------------------------------------------------------
float DoorInfo::retCurrentOpening(AnimationInfo* animInfo)
//----------------------------------------------------------------------------------------------
{
	// if the object is not animated, then just return the normal opening rate
	if (!this->retHasAnimated() || !animInfo->retDoAnimFlag()){
		return (float)mRotateDeg;
	}


	// In case there is only 1 animation checkpoint, return it as a static opeing rate of the door
	if (this->retAnimationList()->retNodeCount() == 1){
		AnimationNodeDoor* tempAnimDoor = static_cast<AnimationNodeDoor*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimDoor->retDoorOpen());
	}


	// If the animation has not started yet, return the intial position
	if (animInfo->retCurrentFrame() < this->retAnimationList()->retListHead()->retAnimTime()){
		AnimationNodeDoor* tempAnimDoor = static_cast<AnimationNodeDoor*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimDoor->retDoorOpen());
	}


	// If the animation is over, return the final position
	if (animInfo->retCurrentFrame() >= this->retAnimationList()->retListLast()->retAnimTime()){
		AnimationNodeDoor* tempAnimDoor = static_cast<AnimationNodeDoor*> (this->retAnimationList()->retListLast());
		return (float)(tempAnimDoor->retDoorOpen());
	}	

	// Else calculate the exact door opening, based on the current time and the animation properties

	// 1) First find the two surrounding animation checkpoints
	AnimationNode * help = this->retAnimationList()->retListHead();

	while (animInfo->retCurrentFrame() > help->retAnimTime()){
		help = help->retAnimNext();
	}

	// Check if we are at the very begining of the animation
	if (help == this->retAnimationList()->retListHead()){
		AnimationNodeDoor* tempDCurr = static_cast<AnimationNodeDoor*> (help);
		return (float)(tempDCurr->retDoorOpen());
	}

	AnimationNodeDoor* tempDPrev = static_cast<AnimationNodeDoor*> (help->retAnimPrev());
	AnimationNodeDoor* tempDCurr = static_cast<AnimationNodeDoor*> (help);

	// 2) Calculate the time fration between the identified neighboring checkpoints
	float frameRel = (animInfo->retCurrentFrame() - tempDPrev->retAnimTime()) / 
		(float)(tempDCurr->retAnimTime() - tempDPrev->retAnimTime());

	// 3) Based on the current interpolation mode, calculate the opening rate based on the time fraction 
	float openRate = 0;

	// Linear interpolation
	if (this->retAnimationList()->retInterpMode() == 0){

		openRate = (float)(linearInterp((float)(tempDPrev->retDoorOpen()), (float)(tempDCurr->retDoorOpen()), frameRel));		

	}
	// Cosine interpolation
	else if (this->retAnimationList()->retInterpMode() == 1){

		openRate = (float)(cosineInterp((float)(tempDPrev->retDoorOpen()), (float)(tempDCurr->retDoorOpen()), frameRel));		

	}
	// Cubic interpolation
	else {
		float v1 = (float)(tempDPrev->retDoorOpen());
		float v2 = (float)(tempDCurr->retDoorOpen());
		float v0 = v1;

		if (tempDPrev->retAnimPrev() != NULL){
			AnimationNodeDoor* tempDPrevPrev = static_cast<AnimationNodeDoor*> (tempDPrev->retAnimPrev());
			v0 = (float)(tempDPrevPrev->retDoorOpen());
		}

		float v3 = v2;

		if (tempDCurr->retAnimNext() != NULL){
			AnimationNodeDoor* tempDCurrCurr = static_cast<AnimationNodeDoor*> (tempDCurr->retAnimNext());
			v0 = (float)(tempDCurrCurr->retDoorOpen());
		}

		openRate = cubicInterp(v0, v1, v2, v3, frameRel);
		
	}
	
	return openRate;
}


//----------------------------------------------------------------------------------------------
float DoorInfo::retCurrentHandleRate(AnimationInfo* animInfo)
//----------------------------------------------------------------------------------------------
{
	// if the object is not animated, then just return the normal handle rate
	if (!this->retHasAnimated() || !animInfo->retDoAnimFlag()){

		// Check if the handles are being set automatically
		if (!animInfo->retHandlesAuto()){
			return (float)(this->mHandleRate);
		}
		else{
			if (this->retRotateDeg() == 0){
				return 0;
			}
			else{
				return 50;
			}
		}
	}

	// In case there is only 1 animation checkpoint, return it as a static handle rate
	if (this->retAnimationList()->retNodeCount() == 1){
		AnimationNodeDoor* tempAnimDoor = static_cast<AnimationNodeDoor*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimDoor->retHandleRate());
	}


	// If the animation has not started yet, return the intial position
	if (animInfo->retCurrentFrame() < this->retAnimationList()->retListHead()->retAnimTime()){
		AnimationNodeDoor* tempAnimDoor = static_cast<AnimationNodeDoor*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimDoor->retHandleRate());
	}


	// If the animation is over, return the final position
	if (animInfo->retCurrentFrame() >= this->retAnimationList()->retListLast()->retAnimTime()){
		AnimationNodeDoor* tempAnimDoor = static_cast<AnimationNodeDoor*> (this->retAnimationList()->retListLast());
		return (float)(tempAnimDoor->retHandleRate());
	}	

	// Else calculate the exact door opening, based on the current time and the animation properties

	// 1) First find the two surrounding animation checkpoints
	AnimationNode * help = this->retAnimationList()->retListHead();

	while (animInfo->retCurrentFrame() > help->retAnimTime()){
		help = help->retAnimNext();
	}

	// Check if we are at the very begining of the animation
	if (help == this->retAnimationList()->retListHead()){
		AnimationNodeDoor* tempDCurr = static_cast<AnimationNodeDoor*> (help);
		return (float)(tempDCurr->retHandleRate());
	}

	AnimationNodeDoor* tempDPrev = static_cast<AnimationNodeDoor*> (help->retAnimPrev());
	AnimationNodeDoor* tempDCurr = static_cast<AnimationNodeDoor*> (help);

	// 2) Calculate the time fration between the identified neighboring checkpoints
	float frameRel = (animInfo->retCurrentFrame() - tempDPrev->retAnimTime()) / 
		(float)(tempDCurr->retAnimTime() - tempDPrev->retAnimTime());

	// 3) Based on the current interpolation mode, calculate the opening rate based on the time fraction 

	float handleRate = 0;
	// Linear interpolation
	if (this->retAnimationList()->retInterpMode() == 0){

		handleRate = linearInterp((float)(tempDPrev->retHandleRate()), (float)(tempDCurr->retHandleRate()), frameRel);		

	}
	// Cosine interpolation
	else if (this->retAnimationList()->retInterpMode() == 1){

		handleRate = cosineInterp((float)(tempDPrev->retHandleRate()), (float)(tempDCurr->retHandleRate()), frameRel);		

	}
	// Cubic interpolation
	else {
		float v1 = (float)(tempDPrev->retHandleRate());
		float v2 = (float)(tempDCurr->retHandleRate());
		float v0 = v1;

		if (tempDPrev->retAnimPrev() != NULL){
			AnimationNodeDoor* tempDPrevPrev = static_cast<AnimationNodeDoor*> (tempDPrev->retAnimPrev());
			v0 = (float)(tempDPrevPrev->retHandleRate());
		}

		float v3 = v2;

		if (tempDCurr->retAnimNext() != NULL){
			AnimationNodeDoor* tempDCurrCurr = static_cast<AnimationNodeDoor*> (tempDCurr->retAnimNext());
			v0 = (float)(tempDCurrCurr->retHandleRate());
		}

		handleRate = cubicInterp(v0, v1, v2, v3, frameRel);
		
	}
	
	return handleRate;
}


//----------------------------------------------------------------------------------------------
ShutterInfo::ShutterInfo() : ObjectInfo(_TYPE_INFO_SHUTTER) 
//----------------------------------------------------------------------------------------------
{
	// Set the initial values
	mScaleRate = 100;
	mTopOffset = 0;
	mRotateRate = 0;
}

//----------------------------------------------------------------------------------------------
float ShutterInfo::retCurrentOpening(AnimationInfo* animInfo)
//----------------------------------------------------------------------------------------------
{
	// if the object is not animated, then just return the normal opening rate
	if (!this->retHasAnimated() || !animInfo->retDoAnimFlag()){
		return (float)(mScaleRate);
	}


	// In case there is only 1 animation checkpoint, return it as a static opeing rate of the shutter object
	if (this->retAnimationList()->retNodeCount() == 1){
		AnimationNodeShutter* tempAnimShutter = static_cast<AnimationNodeShutter*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimShutter->retShutterOpen());
	}


	// If the animation has not started yet, return the intial position
	if (animInfo->retCurrentFrame() < this->retAnimationList()->retListHead()->retAnimTime()){
		AnimationNodeShutter* tempAnimShutter = static_cast<AnimationNodeShutter*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimShutter->retShutterOpen());
	}


	// If the animation is over, return the final position
	if (animInfo->retCurrentFrame() >= this->retAnimationList()->retListLast()->retAnimTime()){
		AnimationNodeShutter* tempAnimShutter = static_cast<AnimationNodeShutter*> (this->retAnimationList()->retListLast());
		return (float)(tempAnimShutter->retShutterOpen());
	}	

	// Else calculate the exact shutter opening, based on the current time and the animation properties

	// 1) First find the two surrounding animation checkpoints
	AnimationNode * help = this->retAnimationList()->retListHead();

	while (animInfo->retCurrentFrame() > help->retAnimTime()){
		help = help->retAnimNext();
	}

	// Check if we are at the very begining of the animation
	if (help == this->retAnimationList()->retListHead()){
		AnimationNodeShutter* tempSCurr = static_cast<AnimationNodeShutter*> (help);
		return (float)(tempSCurr->retShutterOpen());
	}

	AnimationNodeShutter* tempSPrev = static_cast<AnimationNodeShutter*> (help->retAnimPrev());
	AnimationNodeShutter* tempSCurr = static_cast<AnimationNodeShutter*> (help);

	// 2) Calculate the time fration between the identified neighboring checkpoints
	float frameRel = (animInfo->retCurrentFrame() - tempSPrev->retAnimTime()) / 
		(float)(tempSCurr->retAnimTime() - tempSPrev->retAnimTime());


	// 3) Based on the current interpolation mode, calculate the opening rate based on the time fraction 

	float openRate = 0;
	// Linear interpolation
	if (this->retAnimationList()->retInterpMode() == 0){

		openRate = linearInterp((float)(tempSPrev->retShutterOpen()), (float)(tempSCurr->retShutterOpen()), frameRel);		

	}
	// Cosine interpolation
	else if (this->retAnimationList()->retInterpMode() == 1){

		openRate = cosineInterp((float)(tempSPrev->retShutterOpen()), (float)(tempSCurr->retShutterOpen()), frameRel);		

	}
	// Cubic interpolation
	else {
		float v1 = (float)(tempSPrev->retShutterOpen());
		float v2 = (float)(tempSCurr->retShutterOpen());
		float v0 = v1;

		if (tempSPrev->retAnimPrev() != NULL){
			AnimationNodeShutter* tempSPrevPrev = static_cast<AnimationNodeShutter*> (tempSPrev->retAnimPrev());
			v0 = (float)(tempSPrevPrev->retShutterOpen());
		}

		float v3 = v2;

		if (tempSCurr->retAnimNext() != NULL){
			AnimationNodeShutter* tempSCurrCurr = static_cast<AnimationNodeShutter*> (tempSCurr->retAnimNext());
			v0 = (float)(tempSCurrCurr->retShutterOpen());
		}

		openRate = cubicInterp(v0, v1, v2, v3, frameRel);
		
	}
	
	return openRate;
}

//----------------------------------------------------------------------------------------------
float ShutterInfo::retCurrentRotation(AnimationInfo* animInfo)
//----------------------------------------------------------------------------------------------
{
	// if the object is not animated, then just return the normal rotation rate
	if (!this->retHasAnimated() || !animInfo->retDoAnimFlag()){
		return (float)(mRotateRate);
	}


	// In case there is only 1 animation checkpoint, return it as a static opeing rate of the shutter object
	if (this->retAnimationList()->retNodeCount() == 1){
		AnimationNodeShutter* tempAnimShutter = static_cast<AnimationNodeShutter*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimShutter->retShutterRotate());
	}


	// If the animation has not started yet, return the intial rotation
	if (animInfo->retCurrentFrame() < this->retAnimationList()->retListHead()->retAnimTime()){
		AnimationNodeShutter* tempAnimShutter = static_cast<AnimationNodeShutter*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimShutter->retShutterRotate());
	}


	// If the animation is over, return the final rotation
	if (animInfo->retCurrentFrame() >= this->retAnimationList()->retListLast()->retAnimTime()){
		AnimationNodeShutter* tempAnimShutter = static_cast<AnimationNodeShutter*> (this->retAnimationList()->retListLast());
		return (float)(tempAnimShutter->retShutterRotate());
	}	

	// Else calculate the exact shutter rotation rate, based on the current time and the animation properties

	// 1) First find the two surrounding animation checkpoints
	AnimationNode * help = this->retAnimationList()->retListHead();

	while (animInfo->retCurrentFrame() > help->retAnimTime()){
		help = help->retAnimNext();
	}

	// Check if we are at the very begining of the animation
	if (help == this->retAnimationList()->retListHead()){
		AnimationNodeShutter* tempSCurr = static_cast<AnimationNodeShutter*> (help);
		return (float)(tempSCurr->retShutterRotate());
	}

	AnimationNodeShutter* tempSPrev = static_cast<AnimationNodeShutter*> (help->retAnimPrev());
	AnimationNodeShutter* tempSCurr = static_cast<AnimationNodeShutter*> (help);

	// 2) Calculate the time fration between the identified neighboring checkpoints
	float frameRel = (animInfo->retCurrentFrame() - tempSPrev->retAnimTime()) / 
		(float)(tempSCurr->retAnimTime() - tempSPrev->retAnimTime());

	// 3) Based on the current interpolation mode, calculate the opening rate based on the time fraction 

	float rotateRate = 0;
	// Linear interpolation
	if (this->retAnimationList()->retInterpMode() == 0){

		rotateRate = linearInterp((float)(tempSPrev->retShutterRotate()), (float)(tempSCurr->retShutterRotate()), frameRel);		

	}
	// Cosine interpolation
	else if (this->retAnimationList()->retInterpMode() == 1){

		rotateRate = cosineInterp((float)(tempSPrev->retShutterRotate()), (float)(tempSCurr->retShutterRotate()), frameRel);		

	}
	// Cubic interpolation
	else {
		float v1 = (float)(tempSPrev->retShutterRotate());
		float v2 = (float)(tempSCurr->retShutterRotate());
		float v0 = v1;

		if (tempSPrev->retAnimPrev() != NULL){
			AnimationNodeShutter* tempSPrevPrev = static_cast<AnimationNodeShutter*> (tempSPrev->retAnimPrev());
			v0 = (float)(tempSPrevPrev->retShutterRotate());
		}

		float v3 = v2;

		if (tempSCurr->retAnimNext() != NULL){
			AnimationNodeShutter* tempSCurrCurr = static_cast<AnimationNodeShutter*> (tempSCurr->retAnimNext());
			v0 = (float)(tempSCurrCurr->retShutterRotate());
		}

		rotateRate = cubicInterp(v0, v1, v2, v3, frameRel);
		
	}
	
	return rotateRate;
}


//----------------------------------------------------------------------------------------------
void ShutterInfo::copyAnimList(AnimationList * list)
//----------------------------------------------------------------------------------------------
{
	this->clearAnimList();

	this->retAnimationList()->setInterpMode(list->retInterpMode());

	// If the provided list is empty, there is nothing to copy
	if (list->retNodeCount() == 0){
		return;
	}

	// Iterate through the list and create copies of the nodes
	AnimationNode * help = list->retListHead();

	while (help != NULL){
		AnimationNodeShutter * tempS = static_cast<AnimationNodeShutter*> (help);

		AnimationNodeShutter * newNodeShutter = new AnimationNodeShutter();

		newNodeShutter->setAnimTime(tempS->retAnimTime());
		newNodeShutter->setShutterOpen(tempS->retShutterOpen());
		newNodeShutter->setShutterRotate(tempS->retShutterRotate());

		// Insert the node to the list
		this->retAnimationList()->insertNode(newNodeShutter);

		help = help->retAnimNext();
	}
}

//----------------------------------------------------------------------------------------------
WindowInfo::WindowInfo() : ObjectInfo(_TYPE_INFO_WINDOW) 
//----------------------------------------------------------------------------------------------
{
	// Set the initial values
	mRotateDeg = 0;
	mHandleRate = 0;
	mOpenType = 0;

}

//----------------------------------------------------------------------------------------------
float WindowInfo::retCurrentOpening(AnimationInfo* animInfo)
//----------------------------------------------------------------------------------------------
{
	// if the object is not animated, then just return the normal opening rate
	if (!this->retHasAnimated() || !animInfo->retDoAnimFlag()){
		return (float)(mRotateDeg);
	}


	// In case there is only 1 animation checkpoint, return it as a static opeing rate of the window
	if (this->retAnimationList()->retNodeCount() == 1){
		AnimationNodeWindow* tempAnimWindow = static_cast<AnimationNodeWindow*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimWindow->retWindowOpen());
	}


	// If the animation has not started yet, return the intial position
	if (animInfo->retCurrentFrame() < this->retAnimationList()->retListHead()->retAnimTime()){
		AnimationNodeWindow* tempAnimWindow = static_cast<AnimationNodeWindow*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimWindow->retWindowOpen());
	}


	// If the animation is over, return the final position
	if (animInfo->retCurrentFrame() >= this->retAnimationList()->retListLast()->retAnimTime()){
		AnimationNodeWindow* tempAnimWindow = static_cast<AnimationNodeWindow*> (this->retAnimationList()->retListLast());
		return (float)(tempAnimWindow->retWindowOpen());
	}	

	// Else calculate the exact door opening, based on the current time and the animation properties

	// 1) First find the two surrounding animation checkpoints
	AnimationNode * help = this->retAnimationList()->retListHead();

	while (animInfo->retCurrentFrame() > help->retAnimTime()){
		help = help->retAnimNext();
	}

	// Check if we are at the very begining of the animation
	if (help == this->retAnimationList()->retListHead()){
		AnimationNodeWindow* tempWCurr = static_cast<AnimationNodeWindow*> (help);
		return (float)(tempWCurr->retWindowOpen());
	}

	AnimationNodeWindow* tempWPrev = static_cast<AnimationNodeWindow*> (help->retAnimPrev());
	AnimationNodeWindow* tempWCurr = static_cast<AnimationNodeWindow*> (help);

	// 2) Calculate the time fration between the identified neighboring checkpoints
	float frameRel = (animInfo->retCurrentFrame() - tempWPrev->retAnimTime()) / 
		(float)(tempWCurr->retAnimTime() - tempWPrev->retAnimTime());

	// 3) Based on the current interpolation mode, calculate the opening rate based on the time fraction 

	float openRate = 0;
	// Linear interpolation
	if (this->retAnimationList()->retInterpMode() == 0){

		openRate = linearInterp((float)(tempWPrev->retWindowOpen()), (float)(tempWCurr->retWindowOpen()), frameRel);		

	}
	// Cosine interpolation
	else if (this->retAnimationList()->retInterpMode() == 1){

		openRate = cosineInterp((float)(tempWPrev->retWindowOpen()), (float)(tempWCurr->retWindowOpen()), frameRel);		

	}
	// Cubic interpolation
	else {
		float v1 = (float)(tempWPrev->retWindowOpen());
		float v2 = (float)(tempWCurr->retWindowOpen());
		float v0 = v1;

		if (tempWPrev->retAnimPrev() != NULL){
			AnimationNodeWindow* tempWPrevPrev = static_cast<AnimationNodeWindow*> (tempWPrev->retAnimPrev());
			v0 = (float)(tempWPrevPrev->retWindowOpen());
		}

		float v3 = v2;

		if (tempWCurr->retAnimNext() != NULL){
			AnimationNodeWindow* tempWCurrCurr = static_cast<AnimationNodeWindow*> (tempWCurr->retAnimNext());
			v0 = (float)(tempWCurrCurr->retWindowOpen());
		}

		openRate = cubicInterp(v0, v1, v2, v3, frameRel);
		
	}
	
	return openRate;
}


//----------------------------------------------------------------------------------------------
float WindowInfo::retCurrentHandleRate(AnimationInfo* animInfo)
//----------------------------------------------------------------------------------------------
{
	// if the object is not animated, then just return the normal handle opening rate
	if (!this->retHasAnimated() || !animInfo->retDoAnimFlag()){

		// Check if the handles are supposed to be set automatically
		if (!animInfo->retHandlesAuto()){
			return (float)(this->mHandleRate);
		}
		else{
			if (this->retRotateDeg() == 0){
				return 0;
			}
			else{
				if (this->retOpenType() == 0){
					return 50;
				}
				else{
					return 100;
				}
			}
		}
	}

	// In case there is only 1 animation checkpoint, return it as a static opeing rate of the window handle
	if (this->retAnimationList()->retNodeCount() == 1){
		AnimationNodeWindow* tempAnimWindow = static_cast<AnimationNodeWindow*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimWindow->retHandleRate());
	}


	// If the animation has not started yet, return the intial position
	if (animInfo->retCurrentFrame() < this->retAnimationList()->retListHead()->retAnimTime()){
		AnimationNodeWindow* tempAnimWindow = static_cast<AnimationNodeWindow*> (this->retAnimationList()->retListHead());
		return (float)(tempAnimWindow->retHandleRate());
	}


	// If the animation is over, return the final position
	if (animInfo->retCurrentFrame() >= this->retAnimationList()->retListLast()->retAnimTime()){
		AnimationNodeWindow* tempAnimWindow = static_cast<AnimationNodeWindow*> (this->retAnimationList()->retListLast());
		return (float)(tempAnimWindow->retHandleRate());
	}	

	// Else calculate the exact window handle opening, based on the current time and the animation properties

	// 1) First find the two surrounding animation checkpoints
	AnimationNode * help = this->retAnimationList()->retListHead();

	while (animInfo->retCurrentFrame() > help->retAnimTime()){
		help = help->retAnimNext();
	}

	// Check if we are at the very begining of the animation
	if (help == this->retAnimationList()->retListHead()){
		AnimationNodeWindow* tempWCurr = static_cast<AnimationNodeWindow*> (help);
		return (float)(tempWCurr->retHandleRate());
	}

	AnimationNodeWindow* tempWPrev = static_cast<AnimationNodeWindow*> (help->retAnimPrev());
	AnimationNodeWindow* tempWCurr = static_cast<AnimationNodeWindow*> (help);

	// 2) Calculate the time fration between the identified neighboring checkpoints
	float frameRel = (animInfo->retCurrentFrame() - tempWPrev->retAnimTime()) / 
		(float)(tempWCurr->retAnimTime() - tempWPrev->retAnimTime());

	// 3) Based on the current interpolation mode, calculate the opening rate based on the time fraction 

	float handleRate = 0;
	// Linear interpolation
	if (this->retAnimationList()->retInterpMode() == 0){

		handleRate = linearInterp((float)(tempWPrev->retHandleRate()), (float)(tempWCurr->retHandleRate()), frameRel);		

	}
	// Cosine interpolation
	else if (this->retAnimationList()->retInterpMode() == 1){

		handleRate = cosineInterp((float)(tempWPrev->retHandleRate()), (float)(tempWCurr->retHandleRate()), frameRel);		

	}
	// Cubic interpolation
	else {
		float v1 = (float)(tempWPrev->retHandleRate());
		float v2 = (float)(tempWCurr->retHandleRate());
		float v0 = v1;

		if (tempWPrev->retAnimPrev() != NULL){
			AnimationNodeWindow* tempWPrevPrev = static_cast<AnimationNodeWindow*> (tempWPrev->retAnimPrev());
			v0 = (float)(tempWPrevPrev->retHandleRate());
		}

		float v3 = v2;

		if (tempWCurr->retAnimNext() != NULL){
			AnimationNodeWindow* tempWCurrCurr = static_cast<AnimationNodeWindow*> (tempWCurr->retAnimNext());
			v0 = (float)(tempWCurrCurr->retHandleRate());
		}

		handleRate = cubicInterp(v0, v1, v2, v3, frameRel);
	}
	
	return handleRate;
}


//----------------------------------------------------------------------------------------------
int WindowInfo::retCurrentOpenType(AnimationInfo* animInfo)
//----------------------------------------------------------------------------------------------
{
	// if the object is not animated, then just return the normal opening type
	if (!this->retHasAnimated() || !animInfo->retDoAnimFlag()){
		return this->mOpenType;
	}


	// In case there is only 1 animation checkpoint, return it as a static opening type
	if (this->retAnimationList()->retNodeCount() == 1){
		AnimationNodeWindow* tempAnimWindow = static_cast<AnimationNodeWindow*> (this->retAnimationList()->retListHead());
		return tempAnimWindow->retOpenType();
	}


	// If the animation has not started yet, return the intial open type
	if (animInfo->retCurrentFrame() < this->retAnimationList()->retListHead()->retAnimTime()){
		AnimationNodeWindow* tempAnimWindow = static_cast<AnimationNodeWindow*> (this->retAnimationList()->retListHead());
		return tempAnimWindow->retOpenType();
	}


	// If the animation is over, return the final open type
	if (animInfo->retCurrentFrame() >= this->retAnimationList()->retListLast()->retAnimTime()){
		AnimationNodeWindow* tempAnimWindow = static_cast<AnimationNodeWindow*> (this->retAnimationList()->retListLast());
		return tempAnimWindow->retOpenType();
	}	

	// Else calculate the exact window open type, based on the current time

	// 1) First find the two surrounding animation checkpoints
	AnimationNode * help = this->retAnimationList()->retListHead();

	while (animInfo->retCurrentFrame() > help->retAnimTime()){
		help = help->retAnimNext();
	}

	// Check if we are at the very begining of the animation
	if (help == this->retAnimationList()->retListHead()){
		AnimationNodeWindow* tempWCurr = static_cast<AnimationNodeWindow*> (help);
		return tempWCurr->retOpenType();
	}

	AnimationNodeWindow* tempWCurr = static_cast<AnimationNodeWindow*> (help->retAnimPrev());

	return tempWCurr->retOpenType();	
}

//----------------------------------------------------------------------------------------------
void WindowInfo::copyAnimList(AnimationList * list)
//----------------------------------------------------------------------------------------------
{
	this->clearAnimList();

	this->retAnimationList()->setInterpMode(list->retInterpMode());

	// If the provided list is empty, there is nothing to copy
	if (list->retNodeCount() == 0){
		return;
	}

	// Iterate through the list and create copies of the nodes
	AnimationNode * help = list->retListHead();

	while (help != NULL){
		AnimationNodeWindow * tempW = static_cast<AnimationNodeWindow*> (help);

		AnimationNodeWindow * newNodeWindow = new AnimationNodeWindow();

		newNodeWindow->setAnimTime(tempW->retAnimTime());
		newNodeWindow->setWindowOpen(tempW->retWindowOpen());
		newNodeWindow->setHandleRate(tempW->retHandleRate());
		newNodeWindow->setOpenType(tempW->retOpenType());

		// Insert the node to the list
		this->retAnimationList()->insertNode(newNodeWindow);

		help = help->retAnimNext();
	}
}
