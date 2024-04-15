#include "vrmlSaver.h"

VrmlSaver::VrmlSaver() {
	AllNodes = nullptr;
	RootNodes = nullptr;
}


void VrmlSaver::saveLoadedToVrml(const char* outputFileName) {
	out.open(outputFileName);
	writeHeader();
	writeNodesfromRoot();
}

void VrmlSaver::writeHeader() {
	out << "# VRML V2.0 utf8" << std::endl;
	out << std::endl;
	out << "# Produced by VrmlInstancer program written by Josef Bacík as a part of his master thesis" << std::endl;
	out << "# Time and date are not important yet, still TODO" << std::endl;
	out << std::endl;
}

void VrmlSaver::writeNodesfromRoot() {
	for (size_t i = 0; i < RootNodes->size(); i++)
	{
		if (RootNodes->at(i)->type == Transform)
			writeTransformNode(static_cast<TransformNode*>(RootNodes->at(i)));
		else if (RootNodes->at(i)->type == Shape)
			writeShapeNode(static_cast<ShapeNode*>(RootNodes->at(i)));
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
	std::string leadingSpaces = "";
	for (int i = 0; i < node->nodeDepth; i++){ leadingSpaces += "    "; }
	out << leadingSpaces << "DEF " << node->name << " Transform {" << std::endl;
	if (node->hasTranslation())
		out << leadingSpaces << "  translation " << node->translation[0] << " " << node->translation[1] << " " << node->translation[2] << std::endl;
	if (node->hasRotation())
		out << leadingSpaces << "  rotation " << node->rotation[0] << " " << node->rotation[1] << " " << node->rotation[2] << " " << node->rotation[3] << std::endl;
	if (node->hasScale())
		out << leadingSpaces << "  scale " << node->scale[0] << " " << node->scale[1] << " " << node->scale[2] << std::endl;
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
	std::string leadingSpaces = "";
	for (int i = 0; i < node->nodeDepth; i++) { leadingSpaces += "    "; }

	out << leadingSpaces << "  apperance Apperance {" << std::endl;
	writeMaterial(node);
	writeTexture(node);
	out << leadingSpaces << "  }" << std::endl;
}

void VrmlSaver::writeMaterial(ShapeNode* node) {
	std::string leadingSpaces = "";
	for (int i = 0; i < node->nodeDepth + 1; i++) { leadingSpaces += "    "; }

	out << leadingSpaces << "  material Material {" << std::endl;
	out << leadingSpaces << "    diffuseColor " << node->material.diffuseColor[0] << " " << node->material.diffuseColor[1] << " " << node->material.diffuseColor[2] << std::endl;
	out << leadingSpaces << "    ambientIntensity " << node->material.ambientIntensity << std::endl;
	out << leadingSpaces << "    diffuseColor " << node->material.specularColor[0] << " " << node->material.specularColor[1] << " " << node->material.specularColor[2] << std::endl;
	out << leadingSpaces << "    shininess " << node->material.shininess << std::endl;
	out << leadingSpaces << "    transparency " << node->material.transparency << std::endl;
	out << leadingSpaces << "  }" << std::endl;
}

void VrmlSaver::writeTexture(ShapeNode* node) {
	std::string leadingSpaces = "";
	for (int i = 0; i < node->nodeDepth + 1; i++) { leadingSpaces += "    "; }
	
	out << leadingSpaces << "texture " << node->textureType << " {" << std::endl;
	out << leadingSpaces << "  url \"" << node->textureFilePath << "\"" << std::endl;
	out << leadingSpaces << "}" << std::endl;
}

void VrmlSaver::writeGeometryDEF(ShapeNode* node) {
	std::string leadingSpaces = "  ";
	for (int i = 0; i < node->nodeDepth; i++) { leadingSpaces += "    "; }
	out << leadingSpaces << "geometry DEF " << node->geometry.name << " IndexedFaceSet {" << std::endl;
	out << leadingSpaces << "  ccw " << ((node->geometry.ccw == true) ? "TRUE" : "FALSE") << std::endl;
	out << leadingSpaces << "  solid " << ((node->geometry.solid == true) ? "TRUE" : "FALSE") << std::endl;
	//out << leadingSpaces << "  normalPerVertex " << ((node->geometry.normalPerVertex == true) ? "TRUE" : "FALSE") << std::endl;
	//out << leadingSpaces << "  creaseAngle " << node->geometry.creaseAngle << std::endl;


	out << "}" << std::endl;

}

void VrmlSaver::writeShapeNode(ShapeNode* node) {
	std::string leadingSpaces = "";
	for (int i = 0; i < node->nodeDepth; i++) { leadingSpaces += "    "; }
	out << leadingSpaces << "Shape {" << std::endl;
	writeApperance(node);
	writeGeometryDEF(node);
	out << leadingSpaces << "}" << std::endl;
}

