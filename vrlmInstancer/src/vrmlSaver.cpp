#include "vrmlSaver.h"
#include <iomanip>
#include <ctime> 

VrmlSaver::VrmlSaver(std::vector<BaseNode*>* AllNodes, std::vector<BaseNode*>* RootNodes, 
	std::vector<ShapeNode*>* ShapeNodes, std::vector<Geometry*>* geometries, std::vector<LightNode*>* lights) {
	this->AllNodes = AllNodes;
	this->RootNodes = RootNodes;
	this->ShapeNodes = ShapeNodes;
	this->geometries = geometries;
	this->lights = lights;
}


void VrmlSaver::saveLoadedToVrml(const char* outputFileName) {
	out.open(outputFileName);
	writeHeader();
	writeNodesfromRoot();
	out.close();
}

void VrmlSaver::writeHeader() {
	out << "#VRML V2.0 utf8" << std::endl;
	out << std::endl;
	out << "# Produced by VrmlInstancer program written by Josef Bacík as a part of his master thesis" << std::endl;

	out << "# Time and date " << "TODO " << std::endl;
	out << std::endl;
}

std::string VrmlSaver::getLeadingSpaces(int numberOfSpaces) {
	std::string leadingSpaces;
	for (int i = 0; i < numberOfSpaces; i++) { leadingSpaces += " "; }
	return leadingSpaces;
}

void VrmlSaver::writeNodesfromRoot() {
	for (size_t i = 0; i < RootNodes->size(); i++)
	{
		if (RootNodes->at(i)->type == Transform)
			writeTransformNode(static_cast<TransformNode*>(RootNodes->at(i)));
		else if (RootNodes->at(i)->type == Shape)
			writeShapeNode(static_cast<ShapeNode*>(RootNodes->at(i)));
		else if (RootNodes->at(i)->type == Light)
			writeLightNode(static_cast<LightNode*>(RootNodes->at(i)));
		else if (RootNodes->at(i)->type == ViewPoint)
			writeViewPointNode(static_cast<ViewPointNode*>(RootNodes->at(i)));

	}
}

void VrmlSaver::writeChildren(TransformNode* node) {
	for (size_t i = 0; i < node->children.size(); i++)
	{
		if (node->children[i]->type == Transform)
			writeTransformNode(static_cast<TransformNode*>(node->children[i]));
		else if (node->children[i]->type == Shape)
			writeShapeNode(static_cast<ShapeNode*>(node->children[i]));
	}
}



void VrmlSaver::writeTransformNode(TransformNode* node) {
	std::string leadingSpaces = getLeadingSpaces(4 * node->nodeDepth);
	out << leadingSpaces << "DEF " << node->name << " Transform {" << std::endl;
	if (node->hasTranslation())
		out << leadingSpaces << "  translation " << node->translation.x << " " << node->translation.y << " " << node->translation.z << std::endl;
	if (node->hasRotation())
		out << leadingSpaces << "  rotation " << node->rotation[0] << " " << node->rotation[1] << " " << node->rotation[2] << " " << node->rotation[3] << std::endl;
	if (node->hasScale())
		out << leadingSpaces << "  scale " << node->scale.x << " " << node->scale.y << " " << node->scale.z << std::endl;
	if (node->hasScaleOrientation())
		out << leadingSpaces << "  scaleOrientation " << node->scaleOrientation[0] << " " << node->scaleOrientation[1] << " " << node->scaleOrientation[2] << " " << node->scaleOrientation[2] << std::endl;
	if (node->children.size() > 0) {
		out << leadingSpaces << "  children [" << std::endl;
		writeChildren(node);
		out << leadingSpaces << "  ]" << std::endl;
	}
	out << leadingSpaces << "}" << std::endl;
}

void VrmlSaver::writeApperance(ShapeNode* node) {
	std::string leadingSpaces = getLeadingSpaces((4 * node->nodeDepth) + 2);

	out << leadingSpaces << "appearance Appearance {" << std::endl;
	writeMaterial(node);
	if(node->textureFilePath != "")
		writeTexture(node);
	out << leadingSpaces << "}" << std::endl;
}

void VrmlSaver::writeMaterial(ShapeNode* node) {
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth + 1)));

	out << leadingSpaces << "material Material {" << std::endl;
	out << leadingSpaces << "  diffuseColor " << node->material.diffuseColor[0] << " " << node->material.diffuseColor[1] << " " << node->material.diffuseColor[2] << std::endl;
	out << leadingSpaces << "  ambientIntensity " << node->material.ambientIntensity << std::endl;
	out << leadingSpaces << "  specularColor " << node->material.specularColor[0] << " " << node->material.specularColor[1] << " " << node->material.specularColor[2] << std::endl;
	out << leadingSpaces << "  shininess " << node->material.shininess << std::endl;
	out << leadingSpaces << "  transparency " << node->material.transparency << std::endl;
	out << leadingSpaces << "}" << std::endl;
}

void VrmlSaver::writeTexture(ShapeNode* node) {
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth + 1)));
	
	out << leadingSpaces << "texture " << node->textureType << " {" << std::endl;
	out << leadingSpaces << "  url \"" << node->textureFilePath << "\"" << std::endl;
	out << leadingSpaces << "}" << std::endl;
}

void VrmlSaver::writeGeometryDEF(ShapeNode* node) {
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth)) + 2);
	out << leadingSpaces << "geometry DEF " << node->geometry->name << " IndexedFaceSet {" << std::endl;
	out << leadingSpaces << "  ccw " << ((node->geometry->ccw == true) ? "TRUE" : "FALSE") << std::endl;
	out << leadingSpaces << "  solid " << ((node->geometry->solid == true) ? "TRUE" : "FALSE") << std::endl;
	//out << leadingSpaces << "  normalPerVertex " << ((node->geometry.normalPerVertex == true) ? "TRUE" : "FALSE") << std::endl;
	//out << leadingSpaces << "  creaseAngle " << node->geometry.creaseAngle << std::endl;
	writeGeometryCoords(node);
	if(node->geometry->textureCoords.size() > 0)
		writeGeometryTexCoords(node);
	writeGeometryIndices(node);
	if(node->geometry->facesTextureIndex.size() > 0)
		writeGeometryTextureIndices(node);
	out << leadingSpaces << "}" << std::endl;

}

void VrmlSaver::writeGeometryCoords(ShapeNode* node) 
{
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth + 1)));
	out << leadingSpaces << "coord DEF " << node->geometry->name + "-COORD Coordinate { point [" << std::endl;

	int i = 0, j = 0;
	for (i = 0; i < ((int)node->geometry->coords.size() / numOfPointsPerLine); i++)
	{
		out << leadingSpaces << "  ";
		for (j = 0; j < numOfPointsPerLine; j++)
		{
			int a = (i * numOfPointsPerLine) + j;
			out << node->geometry->coords[a].x << " " << node->geometry->coords[a].y << " " << node->geometry->coords[a].z;
			if (a + 1 == node->geometry->coords.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}
		}
		out << std::endl;
	}
	out << leadingSpaces << "  ";
	for (j = 0; j < numOfPointsPerLine; j++)
	{
		int a = (i * numOfPointsPerLine) + j;
		if (a < node->geometry->coords.size()) {
			out << node->geometry->coords[a].x << " " << node->geometry->coords[a].y << " " << node->geometry->coords[a].z;
			if (a + 1 == node->geometry->coords.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}
		}

	}
	out << std::endl;

	out << leadingSpaces << "}" << std::endl;
}

void VrmlSaver::writeGeometryTexCoords(ShapeNode* node)
{
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth + 1)));
	out << leadingSpaces << "texCoord DEF " << node->geometry->name + "-TEXCOORD TextureCoordinate { point [" << std::endl;

	int i = 0, j = 0;
	for (i = 0; i < ((int)node->geometry->textureCoords.size() / numOfPointsPerLine); i++)
	{
		out << leadingSpaces << "  ";
		for (j = 0; j < numOfPointsPerLine; j++)
		{
			int a = (i * numOfPointsPerLine) + j;
			out << node->geometry->textureCoords[a].x << " " << node->geometry->textureCoords[a].y;
			if (a + 1 == node->geometry->textureCoords.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}
		}
		out << std::endl;
	}
	out << leadingSpaces << "  ";
	for (j = 0; j < numOfPointsPerLine; j++)
	{
		int a = (i * numOfPointsPerLine) + j;
		if (a < node->geometry->textureCoords.size()) {
			out << node->geometry->textureCoords[a].x << " " << node->geometry->textureCoords[a].y;
			if (a + 1 == node->geometry->textureCoords.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}
		}
	}
	out << std::endl;

	out << leadingSpaces << "}" << std::endl;
}

void VrmlSaver::writeGeometryIndices(ShapeNode* node)
{
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth + 1)));
	out << leadingSpaces << "coordIndex [" << std::endl;

	int i = 0, j = 0;
	for (i = 0; i < ((int)node->geometry->facesPointsIndex.size() / numOfIndicesPerLine); i++)
	{
		out << leadingSpaces << "  ";
		for (j = 0; j < numOfIndicesPerLine; j++)
		{
			int a = (i * numOfIndicesPerLine) + j;
			out << node->geometry->facesPointsIndex[a].x << ", " << node->geometry->facesPointsIndex[a].y << ", " << node->geometry->facesPointsIndex[a].z << ", -1, ";

		}
		out << std::endl;
	}
	out << leadingSpaces << "  ";
	for (j = 0; j < numOfIndicesPerLine; j++)
	{
		int a = (i * numOfIndicesPerLine) + j;
		if (a < node->geometry->facesPointsIndex.size())
			out << node->geometry->facesPointsIndex[a].x << ", " << node->geometry->facesPointsIndex[a].y << ", " << node->geometry->facesPointsIndex[a].z << ", -1, ";

	}
	out << std::endl;

	out << leadingSpaces << "]" << std::endl;
}

void VrmlSaver::writeGeometryTextureIndices(ShapeNode* node)
{
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth + 1)));
	out << leadingSpaces << "texCoordIndex [" << std::endl;

	int i = 0, j = 0;
	for (i = 0; i < ((int)node->geometry->facesTextureIndex.size() / numOfIndicesPerLine); i++)
	{
		out << leadingSpaces << "  ";
		for (j = 0; j < numOfIndicesPerLine; j++)
		{
			int a = (i * numOfIndicesPerLine) + j;
			out << node->geometry->facesTextureIndex[a].x << ", " << node->geometry->facesTextureIndex[a].y << ", " << node->geometry->facesTextureIndex[a].z << ", -1, ";

		}
		out << std::endl;
	}
	out << leadingSpaces << "  ";
	for (j = 0; j < numOfIndicesPerLine; j++)
	{
		int a = (i * numOfIndicesPerLine) + j;
		if (a < node->geometry->facesTextureIndex.size())
			out << node->geometry->facesTextureIndex[a].x << ", " << node->geometry->facesTextureIndex[a].y << ", " << node->geometry->facesTextureIndex[a].z << ", -1, ";

	}
	out << std::endl;

	out << leadingSpaces << "]" << std::endl;
}

void VrmlSaver::writeGeometryUSE(ShapeNode* node) {
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth)) + 2);
	out << leadingSpaces << "geometry USE " << node->geometry->name <<  std::endl;
}

void VrmlSaver::writeShapeNode(ShapeNode* node) {
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth)));
	out << leadingSpaces << "Shape {" << std::endl;
	writeApperance(node);
	if (node->geometry != nullptr) {
		if (!node->usesOtherGeometry) {
			writeGeometryDEF(node);
		}
		else {
			writeGeometryUSE(node);
		}
	}
	out << leadingSpaces << "}" << std::endl;
}

void VrmlSaver::writeLightNode(LightNode* node) {
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth)));
	out << "DEF " << node->name << " SpotLight {" << std::endl;
	out << "  intensity " << node->intensity << std::endl;
	out << "  color " << node->color.x << " " << node->color.y << " " << node->color.z << std::endl;
	out << "  location " << node->location.x << " " << node->location.y << " " << node->location.z << std::endl;
	out << "  direction " << node->direction.x << " " << node->direction.y << " " << node->direction.z << std::endl;
	out << "  cutOffAngle " << node->cutOffAngle << std::endl;
	out << "  beamWidth " << node->beamWidth << std::endl;
	out << "  on " << (node->on ? "TRUE" : "FALSE") << std::endl;
	out << "  radius " << node->radius << std::endl;
	out << "}" << std::endl;

}

void VrmlSaver::writeViewPointNode(ViewPointNode* viewPointNode) {
	std::string leadingSpaces = getLeadingSpaces((4 * (viewPointNode->nodeDepth)));
	out << "DEF " << viewPointNode->name << " Viewpoint {" << std::endl;
	out << "  position " << viewPointNode->position.x << " " << viewPointNode->position.y << " " << viewPointNode->position.z << std::endl;
	out << "  orientation " << viewPointNode->orientation[0] << " " << viewPointNode->orientation[1] << " " << viewPointNode->orientation[2] << " " << viewPointNode->orientation[3] << std::endl;
	out << "  fieldOfView " << viewPointNode->fieldOfView << std::endl;
	out << "  description " << viewPointNode->description << std::endl;
	out << "}" << std::endl;

}
