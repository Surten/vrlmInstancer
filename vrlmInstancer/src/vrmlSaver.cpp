#include "vrmlSaver.h"
#include <iomanip>
#include <ctime> 

VrmlSaver::VrmlSaver() {
	scene = nullptr;
}


void VrmlSaver::saveSceneToVrml(const char* outputFileName, Scene* scene) {
	this->scene = scene;
	out.open(outputFileName);
	writeHeader();
	writeNodesfromRoot();
	out.close();
}

void VrmlSaver::writeHeader() {
	out << "#VRML V2.0 utf8" << std::endl;
	out << std::endl;
	out << "# Produced by VrmlInstancer program written by Josef Bac�k as a part of his master thesis" << std::endl;

	out << "# Time and date " << "TODO " << std::endl;
	out << std::endl;
}

std::string VrmlSaver::getLeadingSpaces(int numberOfSpaces) {
	std::string leadingSpaces;
	for (int i = 0; i < numberOfSpaces; i++) { leadingSpaces += " "; }
	return leadingSpaces;
}

void VrmlSaver::writeNodesfromRoot() {
	
	// we cycle through root nodes, expecting only Transform nodes to have children

	for (size_t i = 0; i < scene->RootNodes.size(); i++)
	{
		if (scene->RootNodes.at(i)->type == NodeTypes::Transform)
			writeTransformNode(static_cast<TransformNode*>(scene->RootNodes.at(i)));
		else if (scene->RootNodes.at(i)->type == NodeTypes::Shape)
			writeShapeNode(static_cast<ShapeNode*>(scene->RootNodes.at(i)));
		else if (scene->RootNodes.at(i)->type == NodeTypes::Light)
			writeLightNode(static_cast<LightNode*>(scene->RootNodes.at(i)));
		else if (scene->RootNodes.at(i)->type == NodeTypes::ViewPoint)
			writeViewPointNode(static_cast<ViewPointNode*>(scene->RootNodes.at(i)));

	}
}

void VrmlSaver::writeChildren(TransformNode* node) {
	// we write children of the given node, expecting only Transform nodes to have children
	for (size_t i = 0; i < node->children.size(); i++)
	{
		if (node->children[i]->type == NodeTypes::Transform)
			writeTransformNode(static_cast<TransformNode*>(node->children[i]));
		else if (node->children[i]->type == NodeTypes::Shape)
			writeShapeNode(static_cast<ShapeNode*>(node->children[i]));
	}
}



void VrmlSaver::writeTransformNode(TransformNode* node) {
	std::string leadingSpaces = getLeadingSpaces(4 * node->nodeDepth);
	out << leadingSpaces;
	if (node->name != "")
		out << "DEF " << node->name << " ";
	out << "Transform {" << std::endl;
	if (node->hasTranslation())
		out << leadingSpaces << "  translation " << node->translation.x << " " << node->translation.y << " " << node->translation.z << std::endl;
	if (node->hasRotation())
		out << leadingSpaces << "  rotation " << node->rotation.x << " " << node->rotation.y << " " << node->rotation.z << " " << node->rotation.par << std::endl;
	if (node->hasScale())
		out << leadingSpaces << "  scale " << node->scale.x << " " << node->scale.y << " " << node->scale.z << std::endl;
	if (node->hasScaleOrientation())
		out << leadingSpaces << "  scaleOrientation " << node->scaleOrientation.x << " " << node->scaleOrientation.y << " " << node->scaleOrientation.z << " " << node->scaleOrientation.par << std::endl;
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
	out << leadingSpaces << "  diffuseColor " << node->material->diffuseColor << std::endl;
	out << leadingSpaces << "  ambientIntensity " << node->material->ambientIntensity << std::endl;
	out << leadingSpaces << "  specularColor " << node->material->specularColor << std::endl;
	out << leadingSpaces << "  shininess " << node->material->shininess << std::endl;
	out << leadingSpaces << "  transparency " << node->material->transparency << std::endl;
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
	out << leadingSpaces << "  normalPerVertex " << ((node->geometry->normalPerVertex == true) ? "TRUE" : "FALSE") << std::endl;
	//out << leadingSpaces << "  creaseAngle " << node->geometry.creaseAngle << std::endl;
	writeGeometryCoords(node);

	if(node->geometry->normals.size() > 0)
		writeGeometryNormalCoords(node);
	if(node->geometry->textureCoords.size() > 0)
		writeGeometryTexCoords(node);

	writeGeometryIndices(node);

	if(node->geometry->facesTextureIndex.size() > 0)
		writeGeometryTextureIndices(node);
	if (node->geometry->facesNormalIndex.size() > 0)
		writeGeometryNormalIndices(node);
	out << leadingSpaces << "}" << std::endl;

}

void VrmlSaver::writeGeometryCoords(ShapeNode* node) 
{
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth + 1)));
	out << leadingSpaces << "coord ";
	if (node->geometry->name != "")
		out << "DEF " << node->geometry->name + "-COORD ";
	out << "Coordinate { point [" << std::endl;

	int i = 0, j = 0;
	for (i = 0; i < ((int)node->geometry->coords.size() / numOfPointsPerLine); i++)
	{
		out << leadingSpaces << "  ";
		for (j = 0; j < numOfPointsPerLine; j++)
		{
			int a = (i * numOfPointsPerLine) + j;
			out << node->geometry->coords[a].x << " " << node->geometry->coords[a].y << " " << node->geometry->coords[a].z;

			// very dumb solve to how to write ']' instead of ',' after the last element
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

			// very dumb solve to how to write ']' instead of ',' after the last element
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

void VrmlSaver::writeGeometryNormalCoords(ShapeNode* node) 
{
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth + 1)));
	out << leadingSpaces << "normal ";
	out << "Normal { vector [" << std::endl;

	int i = 0, j = 0;
	for (i = 0; i < ((int)node->geometry->normals.size() / numOfPointsPerLine); i++)
	{
		out << leadingSpaces << "  ";
		for (j = 0; j < numOfPointsPerLine; j++)
		{
			int a = (i * numOfPointsPerLine) + j;
			out << node->geometry->normals[a].x << " " << node->geometry->normals[a].y << " " << node->geometry->normals[a].z;

			// very dumb solve to how to write ']' instead of ',' after the last element
			if (a + 1 == node->geometry->normals.size()) {
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
		if (a < node->geometry->normals.size()) {
			out << node->geometry->normals[a].x << " " << node->geometry->normals[a].y << " " << node->geometry->normals[a].z;

			// very dumb solve to how to write ']' instead of ',' after the last element
			if (a + 1 == node->geometry->normals.size()) {
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

			// very dumb solve to how to write ']' instead of ',' after the last element
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

			// very dumb solve to how to write ']' instead of ',' after the last element
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
			out << node->geometry->facesPointsIndex[a].x << ", " << node->geometry->facesPointsIndex[a].y << ", " << node->geometry->facesPointsIndex[a].z << ", -1";
			
			// very dumb solve to how to write ']' instead of ',' after the last element
			if (a + 1 == node->geometry->facesPointsIndex.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}

		}
		out << std::endl;
	}
	out << leadingSpaces << "  ";
	for (j = 0; j < numOfIndicesPerLine; j++)
	{
		int a = (i * numOfIndicesPerLine) + j;
		if (a < node->geometry->facesPointsIndex.size()) {
			out << node->geometry->facesPointsIndex[a].x << ", " << node->geometry->facesPointsIndex[a].y << ", " << node->geometry->facesPointsIndex[a].z << ", -1";
			
			// very dumb solve to how to write ']' instead of ',' after the last element
			if (a + 1 == node->geometry->facesPointsIndex.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}
		}
	}
	out << std::endl;
}

void VrmlSaver::writeGeometryNormalIndices(ShapeNode* node)
{
	std::string leadingSpaces = getLeadingSpaces((4 * (node->nodeDepth + 1)));
	out << leadingSpaces << "normalIndex [" << std::endl;

	int i = 0, j = 0;
	for (i = 0; i < ((int)node->geometry->facesNormalIndex.size() / numOfIndicesPerLine); i++)
	{
		out << leadingSpaces << "  ";
		for (j = 0; j < numOfIndicesPerLine; j++)
		{
			int a = (i * numOfIndicesPerLine) + j;
			out << node->geometry->facesNormalIndex[a].x << ", " << node->geometry->facesNormalIndex[a].y << ", " << node->geometry->facesNormalIndex[a].z << ", -1";
			
			// very dumb solve to how to write ']' instead of ',' after the last element
			if (a + 1 == node->geometry->facesNormalIndex.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}

		}
		out << std::endl;
	}
	out << leadingSpaces << "  ";
	for (j = 0; j < numOfIndicesPerLine; j++)
	{
		int a = (i * numOfIndicesPerLine) + j;
		if (a < node->geometry->facesNormalIndex.size()) {
			out << node->geometry->facesNormalIndex[a].x << ", " << node->geometry->facesNormalIndex[a].y << ", " << node->geometry->facesNormalIndex[a].z << ", -1";
			
			// very dumb solve to how to write ']' instead of ',' after the last element
			if (a + 1 == node->geometry->facesNormalIndex.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}
		}
	}
	out << std::endl;
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
			out << node->geometry->facesTextureIndex[a].x << ", " << node->geometry->facesTextureIndex[a].y << ", " << node->geometry->facesTextureIndex[a].z << ", -1";
			
			// very dumb solve to how to write ']' instead of ',' after the last element
			if (a + 1 == node->geometry->facesTextureIndex.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}
		}
		out << std::endl;
	}
	out << leadingSpaces << "  ";
	for (j = 0; j < numOfIndicesPerLine; j++)
	{
		int a = (i * numOfIndicesPerLine) + j;
		if (a < node->geometry->facesTextureIndex.size()) {
			out << node->geometry->facesTextureIndex[a].x << ", " << node->geometry->facesTextureIndex[a].y << ", " << node->geometry->facesTextureIndex[a].z << ", -1";
			
			// very dumb solve to how to write ']' instead of ',' after the last element
			if (a + 1 == node->geometry->facesTextureIndex.size()) {
				out << "] ";
			}
			else {
				out << ", ";
			}
		}
	}
	out << std::endl;
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
	switch (node->lightType)
	{
	case LightNode::LightType::SPOTLIGHT:
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
		break;

	case LightNode::LightType::GONIOLIGHT:
		out << "DEF " << node->name << " GonioLight {" << std::endl;
		out << "  url [ \"" << node->url << "\" ]" << std::endl;
		out << "  color " << node->color.x << " " << node->color.y << " " << node->color.z << std::endl;
		out << "  location " << node->location.x << " " << node->location.y << " " << node->location.z << std::endl;
		out << "  direction " << node->direction.x << " " << node->direction.y << " " << node->direction.z << std::endl;
		out << "  on " << (node->on ? "TRUE" : "FALSE") << std::endl;
		out << "  intensity " << node->intensity << std::endl;
		out << "}" << std::endl;
		break;
	}
}

void VrmlSaver::writeViewPointNode(ViewPointNode* viewPointNode) {
	std::string leadingSpaces = getLeadingSpaces((4 * (viewPointNode->nodeDepth)));
	if (viewPointNode->name != "")
		out << "DEF " << viewPointNode->name << " ";
	out << "Viewpoint {" << std::endl;
	out << "  position " << viewPointNode->position.x << " " << viewPointNode->position.y << " " << viewPointNode->position.z << std::endl;
	out << "  orientation " << viewPointNode->orientation[0] << " " << viewPointNode->orientation[1] << " " << viewPointNode->orientation[2] << " " << viewPointNode->orientation[3] << std::endl;
	out << "  fieldOfView " << viewPointNode->fieldOfView << std::endl;
	out << "  description " << '"' << viewPointNode->description << '"' << std::endl;
	out << "}" << std::endl;

}
