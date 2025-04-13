#include "baseNode.h"

#include "geometryModify.h"
#include "error.h"
#include "constants.h"




//----------------------------------------------------------------------------------------------
void TransformNode::setRotationDoor(void)
//----------------------------------------------------------------------------------------------
{
	// Create the rotation vector
	DoorInfo* tempDInfo = static_cast<DoorInfo*> (objectInfo);

	vec4 tempRot(0, 1, 0, (float)(DEG_TO_RAD(90.0 * (tempDInfo->retRotateDeg() / 100.0f))));
	// traverse the list of children nodes
	for (int i = 0; i < children.size(); i++) {
		BaseNode* temp = children[i];

		// Check if this is the node with the moving parts
		if (CheckNodeNameCode(temp->name.c_str(), CODE_DOOR_MOVE)) {
			// Retype the node to the transform node
			TransformNode* tempT = static_cast<TransformNode*> (temp);

			// Set the rotation matrix
			tempT->rotation =  tempRot;

			return;
		}
	}

	Error("Could not find the node with moving parts of the door object!");
}

//----------------------------------------------------------------------------------------------
void TransformNode::setDoorAxis(void)
//----------------------------------------------------------------------------------------------
{
	if (((this->objectInfo->retObjectConstructTypeNumber() > 2) && (this->objectInfo->retObjectConstructTypeNumber() < 7)) ||
		(this->objectInfo->retObjectConstructTypeNumber() == 9) || (this->objectInfo->retObjectConstructTypeNumber() == 12) ||
		(this->objectInfo->retObjectConstructTypeNumber() == 13) || (this->objectInfo->retObjectConstructTypeNumber() == 14)) {
		return;
	}
	// traverse the list of children nodes
	int numChildNodes = (int)children.size();
	for (int i = 0; i < numChildNodes; i++) {
		BaseNode* temp = children[i];
		// We are only interested in the transformation nodes
		if (temp->type == NodeTypes::Transform) {
			// Retype the node to the transform node
			TransformNode* tempT = static_cast<TransformNode*> (temp);

			// Check if this is the node with the door joints (according to its name)
			if (CheckNodeNameCode(tempT->name.c_str(), CODE_DOOR_AXIS)) {

				DoorInfo* tempDInfo = static_cast<DoorInfo*> (objectInfo);
				tempDInfo->setAxis(tempT->translation);

				return;
			}
		}
	}

	Error("Could not find the node with door joints");
}

//----------------------------------------------------------------------------------------------
void TransformNode::setRotationWindow(void)
//----------------------------------------------------------------------------------------------
{

	// Create the rotation vector
	WindowInfo* tempWInfo = static_cast<WindowInfo*> (objectInfo);

	vec4 tempRot(0, 1, 0, DEG_TO_RAD(tempWInfo->retRotateDeg() / 100.0f));
	// traverse the list of children nodes
	for (int i = 0; i < children.size(); i++) {
		BaseNode* temp = children[i];

		// Check if this is the node with the moving parts
		if (CheckNodeNameCode(temp->name.c_str(), CODE_WINDOW_MOVE)) {
			// Retype the node to the transform node
			TransformNode* tempT = static_cast<TransformNode*> (temp);

			// Set the rotation matrix
			tempT->rotation = tempRot;

			return;
		}
	}

	Error("Could not find the node with the moving window parts");

}

//----------------------------------------------------------------------------------------------
void TransformNode::setWindowAxis(void)
//----------------------------------------------------------------------------------------------
{
	// traverse the list of children nodes
	int numChildNodes = (int)children.size();
	for (int i = 0; i < numChildNodes; i++) {
		BaseNode* temp = children[i];
		// Check if this is the node with the joints 
		if (CheckNodeNameCode(temp->name.c_str(), CODE_WINDOW_JOINTS)) {
			// Retype the node to the transform node			
			TransformNode* tempT = static_cast<TransformNode*> (temp);

			// just in case we need it later, retype the node correctly
			tempT->setObjectType(_TYPE_WINDOW_JOINT);
			// Set the joint axis

			WindowInfo* tempWInfo = static_cast<WindowInfo*> (objectInfo);
			tempWInfo->setAxis(tempT->translation);

			return;
		}
	}

	Error("Could not find the node with window joints");
}



//----------------------------------------------------------------------------------------------
void ViewPointNode::computeLookAt(AnimationInfo* animInfo, vec3 & loc, vec3 & dir, vec3 & up) const
//----------------------------------------------------------------------------------------------
{
	loc = retCurrentPosition(animInfo);

	// Get the current orientation of camera
	vec4 currentOrient = this->retCurrentOrientation(animInfo);

	vec3 vec(currentOrient.x, currentOrient.y, currentOrient.z);

	// Normalizuju
	vec.normalize();

	// Matice identity
	Matrix mat;
	// Vytvorim rotacni matici kolem obecneho vektoru definovaneho pomoci 'dir' a uhlu v 'orientation'
	mat = mat.mRotate(vec, currentOrient.par);

	// Urcim smer pohledu kamery
	vec = mat * vec3(0.0, 0.0, -1.0);
	// Urcim up-vector kamery
	up = mat * vec3(0.0, 1.0, 0.0);

	// Zbyva urcit bod, na ktery bude kamera centrovana. Pokud znam smerovy vektor
	// pohledu kamery, staci vzit libovolne cislo a dosadit za parametr 't' v parametricke
	// rci primky.
	const float t = 10.0f;

	// Get the current position of the camera
	vec3 currentPosition = loc;
	dir.x = currentPosition.x + vec.x * t;
	dir.y = currentPosition.y + vec.y * t;
	dir.z = currentPosition.z + vec.z * t;
}


void ViewPointNode::clearAnimList(void)
//----------------------------------------------------------------------------------------------
{
	delete this->animList;
	this->animList = new AnimationList();
}


//----------------------------------------------------------------------------------------------
void ViewPointNode::copyAnimList(AnimationList* list)
//----------------------------------------------------------------------------------------------
{
	this->clearAnimList();

	// If the provided list is empty, there is nothing to copy
	if (list->retNodeCount() == 0) {
		return;
	}

	// Iterate through the list and create copies of the nodes
	AnimationNode* help = list->retListHead();

	while (help != NULL) {
		AnimationNodeCamera* tempC = static_cast<AnimationNodeCamera*> (help);

		AnimationNodeCamera* newNodeCamera = new AnimationNodeCamera();

		newNodeCamera->setAnimTime(tempC->retAnimTime());
		newNodeCamera->setCameraPosition(tempC->retCameraPosition());
		newNodeCamera->setCameraOrientation(tempC->retCameraOrientation());
		newNodeCamera->setTension(tempC->retTension());
		newNodeCamera->setContinuity(tempC->retContinuity());
		newNodeCamera->setBias(tempC->retBias());

		// Insert the node to the list
		animList->insertNode(newNodeCamera);

		help = help->retAnimNext();
	}
}

//----------------------------------------------------------------------------------------------
vec3 ViewPointNode::retCurrentPosition(AnimationInfo* animInfo) const
//----------------------------------------------------------------------------------------------
{
	// if the camera is not animated, then just return its static position
	if (!m_bHasAnimated) {
		return position;
	}


	// In case there is only 1 animation checkpoint, return it as a static opeing rate of the camera
	if (animList->retNodeCount() == 1) {
		AnimationNodeCamera* tempAnimCamera = static_cast<AnimationNodeCamera*> (animList->retListHead());
		return tempAnimCamera->retCameraPosition();
	}


	// If the animation has not started yet, return the intial position
	if (animInfo->retCurrentFrame() < animList->retListHead()->retAnimTime()) {
		AnimationNodeCamera* tempAnimCamera = static_cast<AnimationNodeCamera*> (animList->retListHead());
		return tempAnimCamera->retCameraPosition();
	}


	// If the animation is over, return the final position
	if (animInfo->retCurrentFrame() >= animList->retListLast()->retAnimTime()) {
		AnimationNodeCamera* tempAnimCamera = static_cast<AnimationNodeCamera*> (animList->retListLast());
		return tempAnimCamera->retCameraPosition();
	}

	// Else calculate the exact camera position, based on the current time and the animation properties

	// 1) First find the two surrounding animation checkpoints
	AnimationNode* help = animList->retListHead();

	while (animInfo->retCurrentFrame() > help->retAnimTime()) {
		help = help->retAnimNext();
	}

	// Check if we are at the very begining of the animation
	if (help == animList->retListHead()) {
		AnimationNodeCamera* tempCCurr = static_cast<AnimationNodeCamera*> (help);
		return tempCCurr->retCameraPosition();
	}

	AnimationNodeCamera* tempCPrev = static_cast<AnimationNodeCamera*> (help->retAnimPrev());
	AnimationNodeCamera* tempCCurr = static_cast<AnimationNodeCamera*> (help);

	// 2) Calculate the time fration between the identified neighboring checkpoints
	float frameRel = (animInfo->retCurrentFrame() - tempCPrev->retAnimTime()) /
		(float)(tempCCurr->retAnimTime() - tempCPrev->retAnimTime());

	// 3) Based on the current interpolation mode, calculate the opening rate based on the time fraction 

	vec3 camPos;
	// Linear interpolation
	if (animList->retInterpMode() == 0) {

		camPos.x = linearInterp(tempCPrev->retCameraPosition().x, tempCCurr->retCameraPosition().x, frameRel);
		camPos.y = linearInterp(tempCPrev->retCameraPosition().y, tempCCurr->retCameraPosition().y, frameRel);
		camPos.z = linearInterp(tempCPrev->retCameraPosition().z, tempCCurr->retCameraPosition().z, frameRel);

	}
	// Cosine interpolation
	else if (animList->retInterpMode() == 1) {

		camPos.x = cosineInterp(tempCPrev->retCameraPosition().x, tempCCurr->retCameraPosition().x, frameRel);
		camPos.y = cosineInterp(tempCPrev->retCameraPosition().y, tempCCurr->retCameraPosition().y, frameRel);
		camPos.z = cosineInterp(tempCPrev->retCameraPosition().z, tempCCurr->retCameraPosition().z, frameRel);

	}
	// Cubic interpolation
	else if (animList->retInterpMode() == 2) {
		float v1 = tempCPrev->retCameraPosition().x;
		float v2 = tempCCurr->retCameraPosition().x;
		float v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraPosition().x;
		}

		float v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraPosition().x;
		}

		camPos.x = cubicInterp(v0, v1, v2, v3, frameRel);

		v1 = tempCPrev->retCameraPosition().y;
		v2 = tempCCurr->retCameraPosition().y;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraPosition().y;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraPosition().y;
		}

		camPos.y = cubicInterp(v0, v1, v2, v3, frameRel);

		v1 = tempCPrev->retCameraPosition().z;
		v2 = tempCCurr->retCameraPosition().z;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraPosition().z;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraPosition().z;
		}

		camPos.z = cubicInterp(v0, v1, v2, v3, frameRel);

	}
	// KB Spline
	else {
		float t = tempCPrev->retTension();
		float c = tempCPrev->retContinuity();
		float b = tempCPrev->retBias();

		float v1 = tempCPrev->retCameraPosition().x;
		float v2 = tempCCurr->retCameraPosition().x;
		float v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraPosition().x;
		}

		float v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraPosition().x;
		}

		camPos.x = KBSplineInterp(v0, v1, v2, v3, frameRel, t, c, b);

		v1 = tempCPrev->retCameraPosition().y;
		v2 = tempCCurr->retCameraPosition().y;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraPosition().y;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraPosition().y;
		}

		camPos.y = KBSplineInterp(v0, v1, v2, v3, frameRel, t, c, b);

		v1 = tempCPrev->retCameraPosition().z;
		v2 = tempCCurr->retCameraPosition().z;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraPosition().z;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraPosition().z;
		}

		camPos.z = KBSplineInterp(v0, v1, v2, v3, frameRel, t, c, b);

	}

	return camPos;
}


//----------------------------------------------------------------------------------------------
vec4 ViewPointNode::retCurrentOrientation(AnimationInfo* animInfo) const
//----------------------------------------------------------------------------------------------
{
	// if the camera is not animated, then just return its static orientation
	if (!m_bHasAnimated) {
		return orientation;
	}

	// In case there is only 1 animation checkpoint, return it as a static orientation of the camera
	if (animList->retNodeCount() == 1) {
		AnimationNodeCamera* tempAnimCamera = static_cast<AnimationNodeCamera*> (animList->retListHead());
		return tempAnimCamera->retCameraOrientation();
	}

	// If the animation has not started yet, return the intial orientation
	if (animInfo->retCurrentFrame() < animList->retListHead()->retAnimTime()) {
		AnimationNodeCamera* tempAnimCamera = static_cast<AnimationNodeCamera*> (animList->retListHead());
		return tempAnimCamera->retCameraOrientation();
	}

	// If the animation is over, return the final orientation
	if (animInfo->retCurrentFrame() >= animList->retListLast()->retAnimTime()) {
		AnimationNodeCamera* tempAnimCamera = static_cast<AnimationNodeCamera*> (animList->retListLast());
		return tempAnimCamera->retCameraOrientation();
	}

	// Else calculate the exact camera orientation, based on the current time and the animation properties

	// 1) First find the two surrounding animation checkpoints
	AnimationNode* help = animList->retListHead();

	while (animInfo->retCurrentFrame() > help->retAnimTime()) {
		help = help->retAnimNext();
	}

	// Check if we are at the very begining of the animation
	if (help == animList->retListHead()) {
		AnimationNodeCamera* tempCCurr = static_cast<AnimationNodeCamera*> (help);
		return tempCCurr->retCameraOrientation();
	}

	AnimationNodeCamera* tempCPrev = static_cast<AnimationNodeCamera*> (help->retAnimPrev());
	AnimationNodeCamera* tempCCurr = static_cast<AnimationNodeCamera*> (help);

	// 2) Calculate the time fration between the identified neighboring checkpoints
	float frameRel = (animInfo->retCurrentFrame() - tempCPrev->retAnimTime()) /
		(float)(tempCCurr->retAnimTime() - tempCPrev->retAnimTime());


	// 3) Based on the current interpolation mode, calculate the opening rate based on the time fraction 

	vec4 camOrient;

	// Linear interpolation
	if (animList->retInterpMode() == 0) {

		camOrient.x = linearInterp(tempCPrev->retCameraOrientation().x, tempCCurr->retCameraOrientation().x, frameRel);
		camOrient.y = linearInterp(tempCPrev->retCameraOrientation().y, tempCCurr->retCameraOrientation().y, frameRel);
		camOrient.z = linearInterp(tempCPrev->retCameraOrientation().z, tempCCurr->retCameraOrientation().z, frameRel);
		camOrient.par = linearInterp(tempCPrev->retCameraOrientation().par, tempCCurr->retCameraOrientation().par, frameRel);
	}
	// Cosine interpolation
	else if (animList->retInterpMode() == 1) {

		camOrient.x = cosineInterp(tempCPrev->retCameraOrientation().x, tempCCurr->retCameraOrientation().x, frameRel);
		camOrient.y = cosineInterp(tempCPrev->retCameraOrientation().y, tempCCurr->retCameraOrientation().y, frameRel);
		camOrient.z = cosineInterp(tempCPrev->retCameraOrientation().z, tempCCurr->retCameraOrientation().z, frameRel);
		camOrient.par = cosineInterp(tempCPrev->retCameraOrientation().par, tempCCurr->retCameraOrientation().par, frameRel);

	}
	// Cubic interpolation
	else if (animList->retInterpMode() == 2) {
		float v1 = tempCPrev->retCameraOrientation().x;
		float v2 = tempCCurr->retCameraOrientation().x;
		float v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraOrientation().x;
		}

		float v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraOrientation().x;
		}

		camOrient.x = cubicInterp(v0, v1, v2, v3, frameRel);

		v1 = tempCPrev->retCameraOrientation().y;
		v2 = tempCCurr->retCameraOrientation().y;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraOrientation().y;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraOrientation().y;
		}

		camOrient.y = cubicInterp(v0, v1, v2, v3, frameRel);

		v1 = tempCPrev->retCameraOrientation().z;
		v2 = tempCCurr->retCameraOrientation().z;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraOrientation().z;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraOrientation().z;
		}

		camOrient.z = cubicInterp(v0, v1, v2, v3, frameRel);

		v1 = tempCPrev->retCameraOrientation().par;
		v2 = tempCCurr->retCameraOrientation().par;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraOrientation().par;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraOrientation().par;
		}

		camOrient.par = cubicInterp(v0, v1, v2, v3, frameRel);

	}
	// KB Spline interpolation
	else {
		float t = tempCPrev->retTension();
		float c = tempCPrev->retContinuity();
		float b = tempCPrev->retBias();

		float v1 = tempCPrev->retCameraOrientation().x;
		float v2 = tempCCurr->retCameraOrientation().x;
		float v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraOrientation().x;
		}

		float v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraOrientation().x;
		}

		camOrient.x = KBSplineInterp(v0, v1, v2, v3, frameRel, t, c, b);

		v1 = tempCPrev->retCameraOrientation().y;
		v2 = tempCCurr->retCameraOrientation().y;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraOrientation().y;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraOrientation().y;
		}

		camOrient.y = KBSplineInterp(v0, v1, v2, v3, frameRel, t, c, b);

		v1 = tempCPrev->retCameraOrientation().z;
		v2 = tempCCurr->retCameraOrientation().z;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraOrientation().z;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraOrientation().z;
		}

		camOrient.z = KBSplineInterp(v0, v1, v2, v3, frameRel, t, c, b);

		v1 = tempCPrev->retCameraOrientation().par;
		v2 = tempCCurr->retCameraOrientation().par;
		v0 = v1;

		if (tempCPrev->retAnimPrev() != NULL) {
			AnimationNodeCamera* tempCPrevPrev = static_cast<AnimationNodeCamera*> (tempCPrev->retAnimPrev());
			v0 = tempCPrevPrev->retCameraOrientation().par;
		}

		v3 = v2;

		if (tempCCurr->retAnimNext() != NULL) {
			AnimationNodeCamera* tempCCurrCurr = static_cast<AnimationNodeCamera*> (tempCCurr->retAnimNext());
			v3 = tempCCurrCurr->retCameraOrientation().par;
		}

		camOrient.par = KBSplineInterp(v0, v1, v2, v3, frameRel, t, c, b);

	}


	return camOrient;
}



