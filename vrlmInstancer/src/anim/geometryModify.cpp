/**
@file geometryModify.cpp
File with auxiliary functions for modifying the geometry of particular objects

@author Copyright (C) 2009 Ondrej Linda
*/

#include "geometryModify.h"
#include "constants.h"
#include <string>
#include <iostream>
#include <fstream>


char * ToLower(const char * code){
	char * lowCode = new char[strlen(code) + 1];
	int i;
	for(i = 0; code[i] != '\0'; i++){
		lowCode[i] = (char)tolower(code[i]);
	}
	lowCode[i] = 0;

	return lowCode;
}


//----------------------------------------------------------------------------------------------
bool CheckFileNameCode(std::string file, char * code)
//----------------------------------------------------------------------------------------------
{
	// Compare the file name with the code and return the result
	std::string codeWord(code);
	char* lower_case = ToLower(code);
	if (((int)file.find(codeWord) > -1) || ((int)file.find(lower_case) > -1)){
		delete[] lower_case;
		return true;
	}	
	else{
		delete[] lower_case;
		return false;
	}
}

//----------------------------------------------------------------------------------------------
bool CheckNodeNameCode(const char * name, const char * code)
//----------------------------------------------------------------------------------------------
{
	if (name == NULL){
		return false;
	}
	std::string nameWx(name);
	std::string codeWord(code);
	char* lower_case = ToLower(code);
	// Compare the file name with the code word and return the result
	if (((int)nameWx.find(codeWord) > -1) || ((int)nameWx.find(lower_case) > -1)){
		delete[] lower_case;
		return true;
	}	
	else{
		delete[] lower_case;
		return false;
	}
}

//----------------------------------------------------------------------------------------------
int CheckListDoors(std::vector<BaseNode*>& cont)
//----------------------------------------------------------------------------------------------
{
	int numbDoors = 0;

	for(int i = 0; i < cont.size(); i++){
		// The current node
		BaseNode* temp = cont[i];

		// Find the group node of the whole floor
		if(CheckNodeNameCode(temp->name.c_str(), CODE_DOOR_FLOOR)) {

			TransformNode* tempParT = static_cast<TransformNode*> (temp);
			
			for (int j = 0; j < tempParT->children.size(); j++){
				// We are only interested in nodes that contain door objects
				BaseNode * tempCh = tempParT->children[j];

				if(CheckNodeNameCode(tempCh->name.c_str(), CODE_DOOR)) {
					// Retype to a Transform node			
					TransformNode* tempT = static_cast<TransformNode*> (tempCh);
					numbDoors++;

					// Sets the correct type
					tempT->setObjectType(_TYPE_DOOR);
					if (tempT->name != "") {
						tempT->initDoorInfo();
						tempT->retObjectInfo()->initAnimList();
						tempT->retObjectInfo()->correctObjectName(tempT->name.c_str());				
					}			
				}
			}
		}
	}

	// Return the number identified door objects
	return numbDoors;
}


//----------------------------------------------------------------------------------------------
int CheckListShutters(std::vector<BaseNode*>& cont)
//----------------------------------------------------------------------------------------------
{
	int numbShutters = 0;

	for(int i = 0; i < cont.size(); i++){
		// The current node
		BaseNode* temp = cont[i];
		// We are only interested in nodes that contain window shutter objects
		if(CheckNodeNameCode(temp->name.c_str(), CODE_WINDOW_SHUTTER)){

			// Retype to a Transform node
			TransformNode* tempT = static_cast<TransformNode*> (temp);

			// if this is the node with window shutters, first find the transaltion offset of the top part
			float transOffset = getTransalteOffsetShutters(tempT);			

			// Sets the correct type
			tempT->setObjectType(_TYPE_SHUTTERS_NODE);

			/// Not traverse the list of children and find the node that groups the moving parts
			for (int j = 0; j < tempT->children.size(); j++){
				// The current chidren node
				BaseNode* tempCh = tempT->children[j];
				// Look for the node with moving parts
				if (CheckNodeNameCode(tempCh->name.c_str(), CODE_SHUTTERS_MOVE)) {
					// Retype to a Transform node
					TransformNode* tempChT = static_cast<TransformNode*> (tempCh);
					tempChT->setObjectType(_TYPE_SHUTTERS_MOVE);

					// Now traverse the list of children of the identified node with the moving objects and set their type correctly
					for (int k = 0; k < tempChT->children.size(); k++){
						// The current chidren node
						BaseNode* tempShutCh = tempChT->children[k];

						// Make sure that this is the shutter object
						if (CheckNodeNameCode(tempShutCh->name.c_str(), CODE_SHUTTER)){
							// Retype to a Transform node
							TransformNode* tempShutChT = static_cast<TransformNode*> (tempShutCh);
							tempShutChT->setObjectType(_TYPE_SHUTTER);

							// Set the correct name and properties of the shutter object, make sure that we have the name
							if (tempShutChT->name != "") {
								tempShutChT->initShutterInfo();
								tempShutChT->retObjectInfo()->initAnimList();
								tempShutChT->retObjectInfo()->correctObjectName(tempShutChT->name.c_str());								
							}

							// Set the translation offset of the top of the shutter
							ShutterInfo* tempSInfo = static_cast<ShutterInfo*> (tempShutChT->retObjectInfo());
							tempSInfo->setTopOffset(transOffset);

							numbShutters++;
						}
					}
					
					// If we found the node with the moving parts, break from the cycle
					break;
				}
			}			
		}
	}

	// Return the number identified shutters objects
	return numbShutters;
}

//----------------------------------------------------------------------------------------------
float getTransalteOffsetShutters(TransformNode* trans)
//----------------------------------------------------------------------------------------------
{
	// traverse all children of the node and find the one with the top shutters
	for (int i = 0; i < trans->children.size(); i++) {
		BaseNode* temp = trans->children[i];

		if (CheckNodeNameCode(temp->name.c_str(), CODE_SHUTTER_TOP)) {
			// Retype to a Transform node
			TransformNode* tempT = static_cast<TransformNode*> (temp);

			// Set the type of the node, in case we need it later
			tempT->setObjectType(_TYPE_SHUTTER_TOP);

			vec3 translate = tempT->translation;

			return translate.y;
		}
	}

	return 0;
}


//----------------------------------------------------------------------------------------------
int CheckListWindows(std::vector<BaseNode*>& cont)
//----------------------------------------------------------------------------------------------
{
	int numbWindows = 0;


	for (int i = 0; i < cont.size(); i++) {
		// The current node
		BaseNode* temp = cont[i];
		// We are only interested in nodes that contain window objects
		if (CheckNodeNameCode(temp->name.c_str(), CODE_WINDOW)) {
			// Retype to a Transform node			
			TransformNode* tempT = static_cast<TransformNode*> (temp);

			// Traverse all children of the identified window node and look for the ones with opening windows

			for (int j = 0; j < tempT->children.size(); j++) {
				BaseNode* tempW = tempT->children[j];

				// Check that this is the correct node with opening window
				if (CheckNodeNameCode(tempW->name.c_str(), CODE_WINDOW_OPEN)) {
					// Retype to a Transform node			
					TransformNode* tempWT = static_cast<TransformNode*> (tempW);

					numbWindows++;
					// Sets the correct type
					tempWT->setObjectType(_TYPE_WINDOW);
					if (tempWT->name.c_str() != NULL) {
						tempWT->initWindowInfo();
						tempWT->retObjectInfo()->initAnimList();
						tempWT->retObjectInfo()->correctObjectName(tempWT->name.c_str());
					}
				}
			}
		}
	}

	// Return the number identified window objects
	return numbWindows;
}

//----------------------------------------------------------------------------------------------
void setShutterRoll(TransformNode* node, int rate)
//----------------------------------------------------------------------------------------------
{

	// find the children, which contains the shutter layers - SHUTTER_ROLL
	for (int i = 0; i < node->children.size(); i++) {
		BaseNode* temp = node->children[i];

		// Check if it is the desired node
		if (CheckNodeNameCode(temp->name.c_str(), CODE_SHUTTER_ROTATE)) {

			TransformNode* tempT = static_cast<TransformNode*> (temp);
			// Iterate through all children
			for (int j = 0; j < tempT->children.size(); j++) {

				BaseNode* tempTRoll = tempT->children[j];

				TransformNode* tempTRollT = static_cast<TransformNode*> (tempTRoll);

				// Set the rotation of the given transformation node
				vec4 rot;

				if (node->retObjectInfo()->retObjectConstructTypeNumber() == 1){
					rot = vec4(0.0f, 0.0f, 1.0f, -1.57f * ((float)rate / 100.0f));
				}
				else if (node->retObjectInfo()->retObjectConstructTypeNumber() == 2){
					rot = vec4(0.0f, 0.0f, 1.0f, 2.0f * ((float)rate / 100.0f));
				}
				else if (node->retObjectInfo()->retObjectConstructTypeNumber() == 3){
					rot = vec4(1.0f, 0.0f, 0.0f, -2.0f * ((float)rate / 100.0f));
				}
				else if (node->retObjectInfo()->retObjectConstructTypeNumber() == 4){
					rot = vec4(0.0f, 0.0f, 1.0f, 2.0f * ((float)rate / 100.0f));
				}
				tempTRollT->rotation = rot;

			}

			// we are done
			return;
		}
	}
}

//----------------------------------------------------------------------------------------------
int numbLines(std::string file)
//----------------------------------------------------------------------------------------------
{
	std::ifstream f(file.c_str());

	if (!f.is_open()){
		std::cout << "File " << file.c_str() << " could not be opened."  << std::endl;
		return -1;
	}

	int numb = 0;
	std::string line;
	while (!f.eof()){
		getline(f, line);		
		numb++;
	}

	return numb;
}