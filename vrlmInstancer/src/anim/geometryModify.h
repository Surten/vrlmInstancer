/**
@file geometryModify.h
Header file with auxiliary function delcaration for modifying the geometry of the objects

@author Copyright (C) 2009 Ondrej Linda
*/

#ifndef __GEOMETRY_MODIFY_H__
#define __GEOMETRY_MODIFY_H__

#include <vector>
#include "baseNode.h"


/// Converts the char * into lower case
/**
	@return Returns the converted char *
*/

char * ToLower(const char * code);

/// Check if the file name contains code the given code word
/**
	@return Returns true if the file name contains the given code word, returns false otherwise
*/
bool CheckFileNameCode(std::string file, char * code);

/// Check if the node name contains code the given code word
/**
	@return Returns true if the node name contains the given code word, returns false otherwise
*/
bool CheckNodeNameCode(const char * name, const char * code);

/// Iterates through the given list of nodes and looks for door objects
/**
	@param cont Vector with nodes
	@return Returns the number of identified door nodes
*/
int CheckListDoors(std::vector<BaseNode*>& cont);

/// Checks the list of nodes and assignes Window shutter type to the one containing shutter objects, then it goes to its children, finds the 
/// moving parts and assignes the Moving shutters type to them, finally it go through all shutters and assignes the Shutter type to them
/**
	@param cont Vector with nodes
	@return Returns the number of identified door nodes
*/
int CheckListShutters(std::vector<BaseNode*>& cont);

/// Checks the list children of the identified window shutters node, finds the one that contains the top of the shutter, it extracts the 
/// y translation offset of the shutter
/**
	@param cont Vector with nodes
	@return Returns the number of identified window nodes
*/
float getTransalteOffsetShutters(TransformNode * trans);

/// Checks the list of nodes and assignes Window object type to the one containing window objects, then it goes to its children, finds the 
/// moving parts and assignes the Moving window type to them, finally it finds the window joints
/**
	@param cont Vector with nodes
	@return Returns the number of identified window nodes
*/
int CheckListWindows(std::vector<BaseNode*>& cont);

/// Sets the correct rotation of all of the shutter layers
/**
	@param node Transformation node with the shutter object
	@param rate Rotation rate of the shutter
*/
void setShutterRoll(TransformNode * node, int rate );

/// Returns the number of lines in a file
/**
	@param file name of the file
*/

int numbLines(std::string file);



#endif // __GEOMETRY_MODIFY_H__