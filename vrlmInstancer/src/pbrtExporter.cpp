#include "pbrtExporter.h"
#include "constants.h"
#include "geometryModify.h"
#include "animation.h"

#include <unordered_map>
#include <sstream>
#include <string>
#include <iomanip>


#define RAD_TO_DEG(angle)   ((angle)*57.29577951308f)

//#define USE_FOR_CHECKERBOARD_RENDER

PbrtExporter::PbrtExporter(AnimationInfo* animInfo) : animInfo(animInfo)
{

}


void PbrtExporter::exportScene(std::vector<Scene*> scenes, ViewPointNode* camera, std::string outputFolder, std::string headerFileName, std::string outputImageFormat, bool createNewGeometry, MaterialsFile* matFile)
{
	this->scenes = scenes;
	this->outputFolder = outputFolder;
	this->headerFileName = headerFileName;
	this->outputImageFormat = outputImageFormat;
	this->camera = camera;
	this->createNewGeometry = createNewGeometry;
	bool hasAnimatedCamera = false;
	bool hasAnimatedGeometry = false;
	for (Scene* scene : scenes)
	{
		if (scene->hasAnimatedCamera)
			hasAnimatedCamera = true;
		if (scene->isAnimated)
			hasAnimatedGeometry = true;

	}
	writeAllMaterials(matFile);

	if (!hasAnimatedCamera && !hasAnimatedGeometry)
	{
		exportStatic();
	}
	else
	{
		exportDynamic();
	}
}

void PbrtExporter::exportStatic()
{
	out.open(this->outputFolder + "/" + this->headerFileName + ".pbrt");
	std::string renderImageFileName = headerFileName + "." + outputImageFormat;
	writeSceneWideOptions(renderImageFileName);
	out << " WorldBegin" << std::endl;
	out << "Include \"" << this->headerFileName << "_Mats.pbrt" << "\"" << std::endl;

	for (auto scene : scenes)
	{
		initCurrentGeometryFilename(scene->name, false);
		out << "Include \"" << currentGeometryFileName << "\"" << std::endl;
		writeGeometry(scene);
	}
	out.close();
}

void PbrtExporter::exportDynamic()
{
	for (Scene* scene : scenes)
	{
		if (scene->isAnimated) continue;
		initCurrentGeometryFilename(scene->name, false);
		writeGeometry(scene);
	}

	int numberFiles = (int)(floor(animInfo->retAnimLength() * animInfo->retAnimFps()) + 1);
	for (int i = 0; i < numberFiles; i++)
	{
		std::string indexStr = getStringWithLeadingZeros(i);
		out.open(this->outputFolder + "/" + this->headerFileName + indexStr + ".pbrt");
		std::string renderImageFileName = headerFileName + indexStr + "." + outputImageFormat;
		writeSceneWideOptions(renderImageFileName);
		out << " WorldBegin" << std::endl;
		out << "Include \"" << this->headerFileName << "_Mats.pbrt" << "\"" << std::endl;

		for (Scene* scene : scenes)
		{
			if (scene->isAnimated)
			{
				initCurrentGeometryFilename(scene->name, true, i);
				writeGeometry(scene);
			}
			else
				initCurrentGeometryFilename(scene->name, false);

			out << "Include \"" << currentGeometryFileName << "\"" << std::endl;
		}
		out.close();
		animInfo->incCurrentFrame();
		animInfo->incCurrentTime();
		std::cout << "Exported Frame: " << i << std::endl;
	}

}



void PbrtExporter::writeSceneWideOptions(std::string renderImageFileName) {
	writeIntegrator();
	writeCamera();
	writeSampler();
	writeFilter();
	writeFilm(renderImageFileName);
}


void PbrtExporter::writeCamera() {
	if (camera == nullptr)
	{

		AABB retAABB = scenes[0]->getSceneAABB();

		vec3 cameraPosition(-20000, 12000, 20000);
		cameraPosition.normalize();
		float length = retAABB.getDiagonal().len();

		int customCameraZoom = 1.4;		// adjust the camera distance manually, to include the entire object in the render
		length = length * customCameraZoom;
		cameraPosition = cameraPosition * length;

		cameraPosition = cameraPosition + (retAABB.getArithmeticCenter() - vec3());

		//std::cout << retAABB.min << "   " << retAABB.max << std::endl;
		//std::cout << cameraPosition << "   " << retAABB.getArithmeticCenter() << std::endl;

		vec3 lookUp(0.0f, 1.0f, 0.0f);
		lookUp.normalize();

		out << "LookAt " << cameraPosition << std::endl;
		out << "       " << retAABB.getArithmeticCenter() << std::endl;
		out << "       " << lookUp << std::endl;
		out << " Camera \"perspective\" \"float fov\" [ " << 40 << " ]" << std::endl;
		out << std::endl;
		return;
	}


	vec3 pos, lookAt, lookUp;
	camera->computeLookAt(animInfo, pos, lookAt, lookUp);
	out << "LookAt " << pos << std::endl;
	out << "       " << lookAt << std::endl;
	out << "       " << lookUp << std::endl;
	out << " Camera \"perspective\" \"float fov\" [ " << RAD_TO_DEG(camera->fieldOfView) << " ]" << std::endl;
	out << std::endl;
	
}

void PbrtExporter::writeSampler(){
	out << "Sampler \"halton\" \"integer pixelsamples\" " << numberOfSamples << std::endl;
}

void PbrtExporter::writeIntegrator() {
	out << "Integrator \"volpath\" \"integer maxdepth\" [ " << maxDepth << " ]" << std::endl;
	out << std::endl;
}

void PbrtExporter::writeFilter() {
	out << " #Filter used for antialiasing" << std::endl;
	out << " PixelFilter \"mitchell\"" << std::endl;
	out << std::endl;
}
void PbrtExporter::writeFilm(std::string renderImageFileName) {
	out << " #Output file specification" << std::endl;
	out << " Film \"rgb\" \"string filename\" \"" << renderImageFileName << "\"" << std::endl;
	out << "      \"integer xresolution\" [" << xResolution << "]" << std::endl;
	out << "      \"integer yresolution\" [" << yResolution << "]" << std::endl;
	out << std::endl;
}

void PbrtExporter::writeFloor() {
	//out << "AttributeBegin" << std::endl;
	//out << "Material \"diffuse\" \"rgb reflectance\" [ 0.8 0.8 0.8 ]" << std::endl;
	//out << "Shape \"trianglemesh\"" << std::endl;
	////out << "\"point2 uv\" [0 0 1 0 1 1 0 1 ]" << std::endl;
	////out << "\"normal N\" [ 1.2361e-7 -1 2.4837e-9 1.2361e-7 -1 2.4837e-9 1.2361e-7 -1 2.4837e-9 1.2361e-7 -1 2.4837e-9 ]" << std::endl;
	//out << "\"point3 P\" [-1000 0 -1000 -1000 0 1000 1000 0 1000 1000 0 -1000 ]" << std::endl;
	//out << "\"integer indices\" [ 0 1 2 0 2 3 ]" << std::endl;
	//out << "AttributeEnd" << std::endl;
}

void PbrtExporter::writeLightSource(LightNode* lightNode) {

	outGeometry << " AttributeBegin" << std::endl;

	switch (lightNode->lightType)
	{
	case LightNode::LightType::SPOTLIGHT:
		outGeometry << " LightSource \"spot\"" << std::endl;
		outGeometry << "    \"point3 from\" [" << lightNode->location << " ]" << std::endl;
		outGeometry << "    \"point3 to\" [" << lightNode->location + lightNode->direction << " ]" << std::endl;
		outGeometry << "    \"rgb I\" [" << lightNode->color * lightNode->intensity << " ]" << std::endl;
		outGeometry << "    \"float coneangle\" [" << 60 << " ]" << std::endl;
		outGeometry << "    \"float conedeltaangle\" [" << 80 << " ]" << std::endl;
		break;
	case LightNode::LightType::GONIOLIGHT:
		outGeometry << "    Translate " << lightNode->location << std::endl;
		outGeometry << " LightSource \"goniometric\"" << std::endl;
		if (lightNode->url.find(".ies") != std::string::npos)
		{
			lightNode->url = lightNode->url.substr(0, lightNode->url.length() - 4);
			lightNode->url = lightNode->url + ".exr";
		}
		outGeometry << "    \"string filename\" [ \"" << lightNode->url << "\" ]" << std::endl;
		outGeometry << "    \"rgb I\" [" << lightNode->color * lightNode->intensity << " ]" << std::endl;

		break;
	case LightNode::LightType::ENVIROMENTAL_LIGHT:
		//out << " Rotate -90 1 0 0" << std::endl;
		outGeometry << " LightSource \"infinite\"" << std::endl;
		//out << "    \"string filename\" [ \"" << lightNode->url << "\" ]" << std::endl;
		break;
	}
	outGeometry << " AttributeEnd" << std::endl;
	outGeometry << std::endl;
}

void PbrtExporter::writeAllLightSourcesOfAScene(Scene* scene) {
	for (auto light : scene->lights)
	{
		writeLightSource(light);
	}
}
#ifdef USE_FOR_CHECKERBOARD_RENDER
void PbrtExporter::writeTexture() {
	out << "AttributeBegin" << std::endl;
	out << "Texture \"checkerBoard\" \"spectrum\" \"imagemap\" \"string filename\" [ \"checkerboard-pattern.jpg\" ]" << std::endl;
	out << "AttributeEnd" << std::endl;
}
#endif


std::string PbrtExporter::getStringWithLeadingZeros(int number) {
	std::stringstream ss;
	ss << std::setw(3) << std::setfill('0') << number;
	std::string s = ss.str();
	// Ensure we only have up to three leading zeros
	if (s.length() > 3) {
		return s.substr(s.length() - 3);
	}
	return s;
}

void PbrtExporter::initCurrentGeometryFilename(const std::string & sceneName, bool animated, int animationTimeIndex)
{
	std::string geometryFileName = sceneName;
	size_t ind = sceneName.rfind('/');
	if (ind != std::string::npos) geometryFileName = sceneName.substr(ind + 1);
	if (geometryFileName.find(".wrl") != std::string::npos || geometryFileName.find(".WRL") != std::string::npos)
	{
		geometryFileName = geometryFileName.substr(0, geometryFileName.length() - 4);
	}
	if (animated)
	{
		//int numberFiles = (int)(floor(animInfo->retAnimLength() * animInfo->retAnimFps()) + 1);
		std::string fileNumber = getStringWithLeadingZeros(animationTimeIndex);
		currentGeometryFileName = geometryFileName + fileNumber + ".pbrt";
	}
	else
		currentGeometryFileName = geometryFileName + ".pbrt";
}

void PbrtExporter::writeGeometry(Scene* scene){

	if (createNewGeometry)
	{
		geometryAppendString = scene->name.substr(4);
		outGeometry.open(this->outputFolder + "/" + currentGeometryFileName);
		writeAllLightSourcesOfAScene(scene);
		writeObjectInstances(scene);
		writeSceneHierarchy(scene);
		outGeometry.close();
	}
}

void PbrtExporter::writeObjectInstances(Scene* scene) {
	for (auto geometry : scene->geometries)
	{
		outGeometry << " ObjectBegin \"" << geometry->name << "_" << geometryAppendString << "\"" << std::endl;

		if (geometry->textureCoords.size() > 0)
		{
			writeTriangleMeshWithTexture(geometry);
		}
		else
		{
			writeTriangleMesh(geometry);
		}
#ifdef USE_FOR_CHECKERBOARD_RENDER
		if (geometry->textureCoords.size() > 0) {
			writeMaterialWithTexture(geometry->parent->material);
		}
		else {
			writeMaterial(geometry->parent->material);
		}

#endif

		outGeometry << " ObjectEnd " << std::endl;
		outGeometry << std::endl;
	}
}

void PbrtExporter::writeSceneHierarchy(Scene* scene) {
	for (auto rootNode : scene->RootNodes)
	{
		if (rootNode->type == NodeTypes::Transform)
		{
			writeTransformNode(static_cast<TransformNode*>(rootNode));
		}
	}
}

void PbrtExporter::writeTransformNode(TransformNode* node)
{
	TransformNode* tT = static_cast<TransformNode*>(node);

	// decide based on the code name of the node and proceed with a specific object
	if (CheckNodeNameCode(tT->name.c_str(), CODE_DOOR_MOVE)) {
		pbrtTransformDoor(tT);
	}
	else if (CheckNodeNameCode(tT->name.c_str(), CODE_DOOR_HANDLES)) {
		pbrtTransformDoorHandle(tT);
	}
	else if (CheckNodeNameCode(tT->name.c_str(), CODE_WINDOW_MOVE)) {
		pbrtTransformWindow(tT);
	}
	else if (CheckNodeNameCode(tT->name.c_str(), CODE_WINDOW_HANDLES)) {
		pbrtTransformWindowHandle(tT);
	}
	else if (CheckNodeNameCode(tT->name.c_str(), CODE_SHUTTER)) {
		pbrtTransformWindowShutter(tT);
	}
	else {
		writeBaseTransformNode(tT);
	}
}

void PbrtExporter::writeBaseTransformNode(TransformNode* node)
{
	outGeometry << " AttributeBegin" << std::endl;

	if (node->hasTranslation())
	{
		outGeometry << "   Translate " << node->translation << std::endl;
	}
	if (node->hasRotation()) 
	{
		vec3 axis(node->rotation.x, node->rotation.y, node->rotation.z);
		outGeometry << "   Rotate " << RAD_TO_DEG(node->rotation.par) << " " << axis << std::endl;
	}
	if (node->hasScaleOrientation()) {

		vec3 axis(node->scaleOrientation.x, node->scaleOrientation.y, node->scaleOrientation.z);
		outGeometry << "   Rotate " << RAD_TO_DEG(node->scaleOrientation.par) << " " << axis << std::endl;
		outGeometry << "   Scale " << node->scale << std::endl;
		outGeometry << "   Rotate ";
		float angle = RAD_TO_DEG(node->scaleOrientation.par);
		if (angle >= 0)
			outGeometry << angle << " " << axis << std::endl;
		else
			outGeometry << -angle << " " << axis << std::endl;
	}
	else 
	{
		if (node->hasScale())
		{
			outGeometry << "   Scale " << node->scale << std::endl;
		}
	}

	writeNodeChildren(node);
	outGeometry << " AttributeEnd" << std::endl;
	outGeometry << std::endl;

}

void PbrtExporter::writeNodeChildren(TransformNode* node)
{
	for (auto child : node->children)
	{
		if (child->type == NodeTypes::Transform)
		{
			writeTransformNode(static_cast<TransformNode*>(child));
		}
		else if (child->type == NodeTypes::Shape)
		{
			if (static_cast<ShapeNode*>(child)->geometry != nullptr)
				writeShapeNode(static_cast<ShapeNode*>(child));
		}
	}
}
void PbrtExporter::writeShapeNode(ShapeNode* node)
{
	outGeometry << " AttributeBegin" << std::endl;
	writeMaterialReference(node->exportMaterial);
	outGeometry << "    ObjectInstance \"" << node->geometry->name << "_" << geometryAppendString << "\"" << std::endl;
	outGeometry << " AttributeEnd" << std::endl;

}




struct pair_hash {
	size_t operator() (const std::pair<int, int>& p) const {
		size_t hash1 = std::hash<int>{}(p.first);
		size_t hash2 = std::hash<int>{}(p.second);
		return hash1
			^ (hash2 + 0x9e3779b9 + (hash1 << 6)
				+ (hash1 >> 2));
	}
};

void PbrtExporter::writeTriangleMesh(Geometry* geometry) {
	outGeometry << "   Shape \"trianglemesh\" " << std::endl;

	outGeometry << "   \"point3 P\" [" << std::endl;
	int i = 0;
	for (auto point : geometry->coords)
	{
		outGeometry << point;
		if (i++ % 6 == 0)
			outGeometry << std::endl;
	}
	outGeometry << std::endl << " ]" << std::endl;

	outGeometry << "   \"integer indices\" [" << std::endl;
	i = 0;
	for (auto point : geometry->facesPointsIndex)
	{
		outGeometry << point << " ";
		if (i++ % 6 == 0)
			outGeometry << std::endl;
	}
	outGeometry << std::endl << " ]" << std::endl;


}

void PbrtExporter::convertTextureCoordsIntoPBRTFormat(Geometry* geometry, std::vector<vec3>& newCoords,
	std::vector<vec2>& newTexCoords, std::vector<vec3i>& newCoordIndex)
{
	std::unordered_map<std::pair<int, int>, int, pair_hash> map;

	if (geometry->facesPointsIndex.size() != geometry->facesTextureIndex.size())
		std::cout << "How did I even get here, the sizes of coordinate indicies and texture coordinate indicies do not match, which should not happen in VRML" << std::endl;

	for (int i = 0; i < geometry->facesPointsIndex.size(); i++)
	{
		vec3i coordIndices = geometry->facesPointsIndex[i];
		vec3i texIndices = geometry->facesTextureIndex[i];

		vec3i newIndices(0, 0, 0);

		vec3 point1 = geometry->coords[coordIndices.x];
		vec3 point2 = geometry->coords[coordIndices.y];
		vec3 point3 = geometry->coords[coordIndices.z];

		vec2 texPoint1 = geometry->textureCoords[texIndices.x];
		vec2 texPoint2 = geometry->textureCoords[texIndices.y];
		vec2 texPoint3 = geometry->textureCoords[texIndices.z];


		// x
		std::pair<int, int> help = std::make_pair(coordIndices.x, texIndices.x);
		if (map.find(help) != map.end())
		{
			newIndices.x = map.at(help);
		}
		else
		{
			int index = newCoords.size();
			newCoords.push_back(point1);
			newTexCoords.push_back(texPoint1);

			newIndices.x = index;
			map.insert(std::make_pair(help, index));
		}

		// y
		help = std::make_pair(coordIndices.y, texIndices.y);
		if (map.find(help) != map.end())
		{
			newIndices.y = map.at(help);
		}
		else
		{
			int index = newCoords.size();
			newCoords.push_back(point2);
			newTexCoords.push_back(texPoint2);

			newIndices.y = index;
			map.insert(std::make_pair(help, index));
		}

		// z
		help = std::make_pair(coordIndices.z, texIndices.z);
		if (map.find(help) != map.end())
		{
			newIndices.z = map.at(help);
		}
		else
		{
			int index = newCoords.size();
			newCoords.push_back(point3);
			newTexCoords.push_back(texPoint3);

			newIndices.z = index;
			map.insert(std::make_pair(help, index));
		}

		newCoordIndex.push_back(newIndices);
	}
}

void PbrtExporter::writeTriangleMeshWithTexture(Geometry * geometry) {
	if (!geometry) return;

	std::vector<vec3> newCoords;
	std::vector<vec2> newTexCoords;
	std::vector<vec3i> newCoordIndex;
	convertTextureCoordsIntoPBRTFormat(geometry, newCoords, newTexCoords, newCoordIndex);

	outGeometry << "   Shape \"trianglemesh\" " << std::endl;

	// Coordinates
	outGeometry << "   \"point3 P\" [" << std::endl;
	int i = 0;
	for (auto point : newCoords)
	{
		outGeometry << point;
		if (i++ % 6 == 0)
			outGeometry << std::endl;
	}
	outGeometry << std::endl << " ]" << std::endl;

	// Indices 
	outGeometry << "   \"integer indices\" [" << std::endl;
	i = 0;
	for (auto point : newCoordIndex)
	{
		outGeometry << point << " ";
		if (i++ % 6 == 0)
			outGeometry << std::endl;
	}
	outGeometry << std::endl << " ]" << std::endl;

	// Texture coordinates
	outGeometry << "   \"point2 uv\" [" << std::endl;
	i = 0;
	for (auto point : newTexCoords)
	{
		outGeometry << point << " ";
		if (i++ % 6 == 0)
			outGeometry << std::endl;
	}
	outGeometry << std::endl << " ]" << std::endl;
	
}


void PbrtExporter::writeAllMaterials(MaterialsFile* matFile) 
{
	outMaterials.open(outputFolder + "/" + headerFileName + "_Mats.pbrt");
	for (Mat* material : matFile->materials)
	{
		writeMaterialNamed(material);
	}
}

#ifdef USE_FOR_CHECKERBOARD_RENDER
void PbrtExporter::writeMaterial(Material* material) {
	outGeometry << "   Material \"diffuse\" " << std::endl;
	outGeometry << "      \"rgb reflectance\" [ " << material->diffuseColor << " ]";
	outGeometry << std::endl;
}
void PbrtExporter::writeMaterialWithTexture(Material* material) {
	outGeometry << "   Material \"diffuse\" " << std::endl;
	outGeometry << "      \"texture reflectance\" [ " << "\"checkerBoard\" ]";
	outGeometry << std::endl;
}

#endif

void PbrtExporter::writeMaterialReference(Mat* material)
{
	outGeometry << "    NamedMaterial \"" + material->name +"\"" << std::endl;
}


void PbrtExporter::writeMaterialNamed(Mat* material)
{

	if (material->materialType == MaterialType::DIFFUSE)
	{
		writeMaterialDiffuse(material);
	}
	else if (material->materialType == MaterialType::COATED_DIFFUSE)
	{
		writeMaterialCoatedDiffuse(material);
	}
	else if (material->materialType == MaterialType::DIFFUSE_TRANSMISSIVE)
	{
		writeMaterialDiffuseTransmission(material);
	}
	else if (material->materialType == MaterialType::DIELECTRIC)
	{
		writeMaterialDielectric(material);
	}
	else if (material->materialType == MaterialType::CONDUCTOR)
	{
		writeMaterialConductor(material);
	}
	else if (material->materialType == MaterialType::CONDUCTOR_REFLECTANCE)
	{
		writeMaterialConductorReflectance(material);
	}
	else if (material->materialType == MaterialType::COATED_CONDUCTOR)
	{
		writeMaterialCoatedConductor(material);
	}
	else if (material->materialType == MaterialType::BTF_MATERIAL)
	{
		writeMaterialBTF(material);
	}
	else
	{
		writeMaterialDiffuse(material);
	}
}




void PbrtExporter::writeMaterialDiffuse(Mat* material)
{
	outMaterials << "MakeNamedMaterial \"" << material->name << "\"" << std::endl;
	outMaterials << "    \"string type\" \"diffuse\"" << std::endl;
	outMaterials << "    \"rgb reflectance\" [ " << material->Kd << " ]" << std::endl;
}
void PbrtExporter::writeMaterialCoatedDiffuse(Mat* material)
{
	outMaterials << "MakeNamedMaterial \"" << material->name << "\"" << std::endl;
	outMaterials << "    \"string type\" \"coateddiffuse\"" << std::endl;
	outMaterials << "    \"rgb reflectance\" [ " << material->Kd << " ]" << std::endl;
	outMaterials << "    \"float roughness\" [ " << material->roughness << " ]" << std::endl;
}
void PbrtExporter::writeMaterialDiffuseTransmission(Mat* material)
{
	outMaterials << "MakeNamedMaterial \"" << material->name << "\"" << std::endl;
	outMaterials << "    \"string type\" \"diffusetransmission\" " << std::endl;
	outMaterials << "    \"rgb reflectance\" [ " << material->reflect << " ]" << std::endl;
	outMaterials << "    \"rgb transmittance\" [ " << material->transmit << " ]" << std::endl;
}
void PbrtExporter::writeMaterialDielectric(Mat* material)
{
	outMaterials << "MakeNamedMaterial \"" << material->name << "\"" << std::endl;
	outMaterials << "    \"string type\" \"dielectric\" " << std::endl;
	outMaterials << "    \"spectrum eta\" [ " << material->eta_str << " ]" << std::endl;
}
void PbrtExporter::writeMaterialConductor(Mat* material)
{
	outMaterials << "MakeNamedMaterial \"" << material->name << "\"" << std::endl;
	outMaterials << "    \"string type\" \"conductor\" " << std::endl;
	outMaterials << "    \"spectrum eta\" [ " << material->eta_str << " ]" << std::endl;
	outMaterials << "    \"spectrum k\" [ " << material->k_str << " ]" << std::endl;
	outMaterials << "    \"float roughness\" " << material->roughness << std::endl;
}
void PbrtExporter::writeMaterialConductorReflectance(Mat* material)
{
	outMaterials << "MakeNamedMaterial \"" << material->name << "\"" << std::endl;
	outMaterials << "    \"string type\" \"conductor\" " << std::endl;
	outMaterials << "    \"rgb roughness\" [ " << material->reflect << " ]" << std::endl;
	outMaterials << "    \"float roughness\" " << material->roughness << std::endl;
}

void PbrtExporter::writeMaterialCoatedConductor(Mat* material)
{
	outMaterials << "MakeNamedMaterial \"" << material->name << "\"" << std::endl;
	outMaterials << "    \"string type\" \"coatedconductor\" " << std::endl;
	outMaterials << "    \"spectrum conductor.eta\" [ " << material->eta_str << " ]" << std::endl;
	outMaterials << "    \"spectrum conductor.k\" [ " << material->k_str << " ]" << std::endl;
	outMaterials << "    \"float conductor.roughness\" " << material->roughness << std::endl;
}

void PbrtExporter::writeMaterialBTF(Mat* material)
{
	outMaterials << "MakeNamedMaterial \"" << material->name << "\"" << std::endl;
	outMaterials << "    \"string type\" \"custom\" " << std::endl;
	outMaterials << "    \"string filename\" [ " << material->btfFileName << " ]" << std::endl;
}








//----------------------------------------------------------------------------------------------
void PbrtExporter::pbrtTransformDoor(TransformNode* tempT)
//----------------------------------------------------------------------------------------------
{
	// Find the parent, which stores the information on the joint axis
	TransformNode* tempPar = static_cast<TransformNode*> (tempT->parent);

	// Get the doorInfo object
	DoorInfo* tempDInfo = static_cast<DoorInfo*> (tempPar->retObjectInfo());

	outGeometry << " AttributeBegin" << std::endl;
	// If we defined 'Translation'
	if (tempT->hasTranslation()) {
		outGeometry << "   Translate " << tempT->translation << std::endl;
	}

	// Now we have do center translation to the door axis
	if (tempDInfo->retCurrentOpening(animInfo) > 0) {

		if ((tempDInfo->retObjectConstructTypeNumber() == 1) || (tempDInfo->retObjectConstructTypeNumber() == 2) ||
			(tempDInfo->retObjectConstructTypeNumber() == 10) || (tempDInfo->retObjectConstructTypeNumber() == 11) ||
			(tempDInfo->retObjectConstructTypeNumber() == 15) || (tempDInfo->retObjectConstructTypeNumber() == 16)) {

			vec3 vecAxis = tempDInfo->retAxis();
			outGeometry << "   Translate " << vecAxis.x << " " << vecAxis.y << " " << vecAxis.z << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 3) {
			outGeometry << "   Translate 0 0 " << CENTER_DOOR_NO_JOINTS << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 4) {
			outGeometry << "   Translate 0 0 " << -CENTER_DOOR_NO_JOINTS << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 6) {
			outGeometry << "   Translate 0 0 " << -CENTER_DOOR_NO_JOINTS_4 << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 7) {
			vec3 vecAxis = tempDInfo->retAxis();
			outGeometry << "   Translate " << vecAxis.x << " " << vecAxis.y << " " << vecAxis.z << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 8) {
			vec3 vecAxis = tempDInfo->retAxis();
			outGeometry << "   Translate 0 " << vecAxis.y << " " << vecAxis.z << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 9) {
			outGeometry << "   Translate 0 0 " << -CENTER_DOOR_NO_JOINTS_NARROW << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 12) {
			outGeometry << "   Translate " << -CENTER_DOOR_NO_JOINTS_12 << " 0 0 " << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 13) {
			outGeometry << "   Translate 0 0 " << -CENTER_DOOR_NO_JOINTS_12 << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 14) {
			outGeometry << "   Translate " << CENTER_DOOR_NO_JOINTS_12 << " 0 0 " << std::endl;
		}
	}

	// Rotate the door object 
	if (tempDInfo->retCurrentOpening(animInfo) > 0) {

		// Find the parent, which stores the information about the door object
		TransformNode* tempParT = static_cast<TransformNode*> (tempT->parent);

		DoorInfo* doorInfoParT = static_cast<DoorInfo*> (tempParT->retObjectInfo());

		//vec4 tempRot(tempT->retRotation().x, tempT->retRotation().y, tempT->retRotation().z, tempT->retRotation().par);
		vec4 tempRot(0, 1, 0, (float)(DEG_TO_RAD(90.0 * (doorInfoParT->retCurrentOpening(animInfo) / 100.0f))));

		if ((doorInfoParT->retObjectConstructTypeNumber() == 1) || (doorInfoParT->retObjectConstructTypeNumber() == 3) ||
			(doorInfoParT->retObjectConstructTypeNumber() == 7) || (doorInfoParT->retObjectConstructTypeNumber() == 11) ||
			(doorInfoParT->retObjectConstructTypeNumber() == 14) || (doorInfoParT->retObjectConstructTypeNumber() == 15) ||
			(doorInfoParT->retObjectConstructTypeNumber() == 16))
		{
			tempRot.par *= -1.0;
		}

		outGeometry << "  Rotate " << RAD_TO_DEG(tempRot.par) << " " << tempRot.x << " " << tempRot.y << " " << tempRot.z << std::endl;
	}

	// Pokud byl definovan uzel 'scaleOrientation', pak ho prevedu na rotaci 'Rotate'
	if (tempT->hasScaleOrientation()) {
		vec4 temp4D = tempT->scaleOrientation;
		vec3 temp3D(temp4D.x, temp4D.y, temp4D.z);
		float angleRad = temp4D.par;
		// Operaci 'scaleOrientation' prevedu na rotaci
		outGeometry << "   Rotate " << RAD_TO_DEG(angleRad) << " " << temp3D << std::endl;
		outGeometry << "   Scale " << tempT->scale << std::endl;
		// Inverzni operaci '-scaleOrientation' ziskam jako rotaci o opacny uhel
		outGeometry << "   Rotate ";
		float angle = RAD_TO_DEG(angleRad);
		if (angle >= 0)
			outGeometry << angle << " " << temp3D << std::endl;
		else
			outGeometry << -angle << " " << temp3D << std::endl;
	}
	else {
		// Pokud byl definovan samostatny uzel 'Scale'
		if (tempT->hasScale())
			outGeometry << "   Scale " << tempT->scale << std::endl;
	}

	// Now we have do the reversed center translation
	if (tempDInfo->retCurrentOpening(animInfo) > 0) {

		if ((tempDInfo->retObjectConstructTypeNumber() == 1) || (tempDInfo->retObjectConstructTypeNumber() == 2) ||
			(tempDInfo->retObjectConstructTypeNumber() == 10) || (tempDInfo->retObjectConstructTypeNumber() == 11) ||
			(tempDInfo->retObjectConstructTypeNumber() == 15) || (tempDInfo->retObjectConstructTypeNumber() == 16)) {

			vec3 vecAxis = tempDInfo->retAxis();
			outGeometry << "   Translate " << -vecAxis.x << " " << -vecAxis.y << " " << -vecAxis.z << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 3) {
			outGeometry << "   Translate 0 0 " << -CENTER_DOOR_NO_JOINTS << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 4) {
			outGeometry << "   Translate 0 0 " << CENTER_DOOR_NO_JOINTS << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 6) {
			outGeometry << "   Translate 0 0 " << CENTER_DOOR_NO_JOINTS_4 << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 7) {
			vec3 vecAxis = tempDInfo->retAxis();
			outGeometry << "   Translate " << -vecAxis.x << " " << -vecAxis.y << " " << -vecAxis.z << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 8) {
			vec3 vecAxis = tempDInfo->retAxis();
			outGeometry << "   Translate 0 " << -vecAxis.y << " " << -vecAxis.z << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 9) {
			outGeometry << "   Translate 0 0 " << CENTER_DOOR_NO_JOINTS_NARROW << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 12) {
			outGeometry << "   Translate " << CENTER_DOOR_NO_JOINTS_12 << " 0 0 " << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 13) {
			outGeometry << "   Translate 0 0 " << CENTER_DOOR_NO_JOINTS_12 << std::endl;
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 14) {
			outGeometry << "   Translate " << -CENTER_DOOR_NO_JOINTS_12 << " 0 0 " << std::endl;
		}
	}
	// Najdu uzly, ktere jsou potomci tohoto VRML uzlu 'Transform' --O
	writeNodeChildren(tempT); //--O

	outGeometry << " AttributeEnd" << std::endl << std::endl;

}


//----------------------------------------------------------------------------------------------
void PbrtExporter::pbrtTransformDoorHandle(TransformNode* tempT)
//----------------------------------------------------------------------------------------------
{
	outGeometry << " AttributeBegin" << std::endl;
	// Pokud byl definovan uzel 'Translation'
	if (tempT->hasTranslation()) {
		outGeometry << "   Translate " << tempT->translation << std::endl;
	}

	// Insert the translation for the Center operation
	// get the parent with the handle rotation rate (parent->parent)
	TransformNode* tempParT = static_cast<TransformNode*> (tempT->parent->parent);

	if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 1) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 3) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 7) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 8)) {
		outGeometry << "   Translate 0 0 " << -CENTER_DOOR_HANDLES << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 2) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 4) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 9) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 11) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 13)) {
		outGeometry << "   Translate 0 0 " << CENTER_DOOR_HANDLES << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 10) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 14) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 15)) {
		outGeometry << "   Translate " << -CENTER_DOOR_HANDLES << " 0 0 " << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 12)) {
		outGeometry << "   Translate " << CENTER_DOOR_HANDLES << " 0 0 " << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 16)) {
		outGeometry << "   Translate " << CENTER_DOOR_HANDLES3 << " 0 0 " << std::endl;
	}

	// Pokud byl definovan uzel 'Rotation'
	if (tempT->hasRotation()) {
		vec4 temp = tempT->rotation;
		vec3 tempV(temp.x, temp.y, temp.z);
		float angleRad = temp.par;
		outGeometry << "   Rotate " << RAD_TO_DEG(angleRad) << " " << tempV << std::endl;
	}

	// Get the doorInfo object
	DoorInfo* doorInfoT = static_cast<DoorInfo*> (tempParT->retObjectInfo());

	if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 1) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 3) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 7) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 8)) {

		outGeometry << "   Rotate " << RAD_TO_DEG(1.57 * (doorInfoT->retCurrentHandleRate(animInfo) / 100.0f)) << " 1 0 0" << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 2) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 4) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 9) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 11) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 13)) {

		outGeometry << "   Rotate " << RAD_TO_DEG(-1.57 * (doorInfoT->retCurrentHandleRate(animInfo) / 100.0f)) << " 1 0 0" << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 10) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 14) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 15)) {

		outGeometry << "   Rotate " << RAD_TO_DEG(-1.57 * (doorInfoT->retCurrentHandleRate(animInfo) / 100.0f)) << " 0 0 1" << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 12) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 16)) {

		outGeometry << "   Rotate " << RAD_TO_DEG(1.57 * (doorInfoT->retCurrentHandleRate(animInfo) / 100.0f)) << " 0 0 1" << std::endl;
	}

	// Pokud byl definovan uzel 'scaleOrientation', pak ho prevedu na rotaci 'Rotate'
	if (tempT->hasScaleOrientation()) {
		vec4 temp4D = tempT->scaleOrientation;
		vec3 temp3D(temp4D.x, temp4D.y, temp4D.z);
		float angleRad = temp4D.par;
		// Operaci 'scaleOrientation' prevedu na rotaci
		outGeometry << "   Rotate " << RAD_TO_DEG(angleRad) << " " << temp3D << std::endl;
		outGeometry << "   Scale " << tempT->scale << std::endl;
		// Inverzni operaci '-scaleOrientation' ziskam jako rotaci o opacny uhel
		outGeometry << "   Rotate ";
		float angle = RAD_TO_DEG(angleRad);
		if (angle >= 0)
			outGeometry << angle << " " << temp3D << std::endl;
		else
			outGeometry << -angle << " " << temp3D << std::endl;
	}
	else {
		// Pokud byl definovan samostatny uzel 'Scale'
		if (tempT->hasScale())
			outGeometry << "   Scale " << tempT->scale << std::endl;
	}

	// Perform the inverse translation for the center node	
	if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 1) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 3) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 7) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 8)) {
		outGeometry << "   Translate 0 0 " << CENTER_DOOR_HANDLES << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 2) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 4) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 9) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 11) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 13)) {
		outGeometry << "   Translate 0 0 " << -CENTER_DOOR_HANDLES << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 10) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 14) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 15)) {
		outGeometry << "   Translate " << CENTER_DOOR_HANDLES << " 0 0 " << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 12)) {
		outGeometry << "   Translate " << -CENTER_DOOR_HANDLES << " 0 0 " << std::endl;
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 16)) {
		outGeometry << "   Translate " << -CENTER_DOOR_HANDLES3 << " 0 0 " << std::endl;
	}

	// Najdu uzly, ktere jsou potomci tohoto VRML uzlu 'Transform' --O
	writeNodeChildren(tempT); //--O

	outGeometry << " AttributeEnd" << std::endl << std::endl;
}


//----------------------------------------------------------------------------------------------
void PbrtExporter::pbrtTransformWindow(TransformNode* tempT)
//----------------------------------------------------------------------------------------------
{

	// Get the windowInfo object
	TransformNode* tempPar = static_cast<TransformNode*> (tempT->parent);
	WindowInfo* tempWInfo = static_cast<WindowInfo*> (tempPar->retObjectInfo());

	outGeometry << " AttributeBegin" << std::endl;
	// Pokud byl definovan uzel 'Translation'
	if (tempT->hasTranslation()) {
		outGeometry << "   Translate " << tempT->translation << std::endl;
	}

	// Perform the Translation for the Center operation
	if (tempWInfo->retCurrentOpening(animInfo) > 0) {

		// Now test how the window should be opened, if vertically or horizontally
		if (tempWInfo->retOpenType() == 0) {
			vec3 vecAxis = tempWInfo->retAxis();
			if ((tempWInfo->retObjectConstructTypeNumber() < 5) || tempWInfo->retObjectConstructTypeNumber() == 7) {
				outGeometry << "   Translate " << vecAxis.x << " " << vecAxis.y << " " << vecAxis.z << std::endl;
			}
			else if (tempWInfo->retObjectConstructTypeNumber() == 5) {
				outGeometry << "   Translate " << vecAxis.x + CENTER_WINDOW_TYPE_56 << " " << vecAxis.y << " " << vecAxis.z << std::endl;
			}
			else if (tempWInfo->retObjectConstructTypeNumber() == 6) {
				outGeometry << "   Translate " << vecAxis.x - CENTER_WINDOW_TYPE_56 << " " << vecAxis.y << " " << vecAxis.z << std::endl;
			}
		}
		else {
			outGeometry << "   Translate 0 " << -CENTER_WINDOW_VERTICAL << " 0" << std::endl;
		}
	}


	// Pokud byl definovan uzel 'Rotation'
	if (tempT->hasRotation()) {
		vec4 temp = tempT->rotation;
		vec3 tempV(temp.x, temp.y, temp.z);
		float angleRad = temp.par;
		outGeometry << "   Rotate " << RAD_TO_DEG(angleRad) << " " << tempV << std::endl;
	}

	// Implement the rotation of the window
	// Find the parent, which stores the information about the window object
	TransformNode* tempParT = static_cast<TransformNode*> (tempT->parent);

	WindowInfo* windowInfoT = static_cast<WindowInfo*> (tempParT->retObjectInfo());

	// Check what type of rotation is supposed to be done
	float rotateRate = 0;

	if (windowInfoT->retCurrentOpenType(animInfo) == 0) {
		rotateRate = (float)(DEG_TO_RAD(90.0 * windowInfoT->retCurrentOpening(animInfo) / 100.0f));
		if ((windowInfoT->retObjectConstructTypeNumber() == 1) || (windowInfoT->retObjectConstructTypeNumber() == 3) ||
			(windowInfoT->retObjectConstructTypeNumber() == 5)) {
			vec4 tempRot(0, 1, 0, -rotateRate);

			outGeometry << "   Rotate " << RAD_TO_DEG(tempRot.par) << " " << tempRot.x << " " << tempRot.y << " " << tempRot.z << std::endl;
		}
		else {
			vec4 tempRot(0, 1, 0, rotateRate);

			outGeometry << "   Rotate " << RAD_TO_DEG(tempRot.par) << " " << tempRot.x << " " << tempRot.y << " " << tempRot.z << std::endl;
		}
	}
	else {
		rotateRate = (float)(DEG_TO_RAD(10.0 * windowInfoT->retCurrentOpening(animInfo) / 100.0f));
		if ((windowInfoT->retObjectConstructTypeNumber() == 1) || (windowInfoT->retObjectConstructTypeNumber() == 5)) {
			vec4 tempRot(1, 0, 0, -rotateRate);

			outGeometry << "   Rotate " << RAD_TO_DEG(tempRot.par) << " " << tempRot.x << " " << tempRot.y << " " << tempRot.z << std::endl;
		}
		else if ((windowInfoT->retObjectConstructTypeNumber() == 2) || (windowInfoT->retObjectConstructTypeNumber() == 6)) {
			vec4 tempRot(1, 0, 0, -rotateRate);

			outGeometry << "   Rotate " << RAD_TO_DEG(tempRot.par) << " " << tempRot.x << " " << tempRot.y << " " << tempRot.z << std::endl;
		}
		else if (windowInfoT->retObjectConstructTypeNumber() == 3) {
			vec4 tempRot(0, 0, 1, rotateRate);

			outGeometry << "   Rotate " << RAD_TO_DEG(tempRot.par) << " " << tempRot.x << " " << tempRot.y << " " << tempRot.z << std::endl;
		}
		else if (windowInfoT->retObjectConstructTypeNumber() == 4) {
			vec4 tempRot(0, 0, 1, rotateRate);

			outGeometry << "   Rotate " << RAD_TO_DEG(tempRot.par) << " " << tempRot.x << " " << tempRot.y << " " << tempRot.z << std::endl;
		}
		else if (windowInfoT->retObjectConstructTypeNumber() == 7) {
			vec4 tempRot(0, 0, 1, -rotateRate);

			outGeometry << "   Rotate " << RAD_TO_DEG(tempRot.par) << " " << tempRot.x << " " << tempRot.y << " " << tempRot.z << std::endl;
		}
	}

	// Pokud byl definovan uzel 'scaleOrientation', pak ho prevedu na rotaci 'Rotate'
	if (tempT->hasScaleOrientation()) {
		vec4 temp4D = tempT->scaleOrientation;
		vec3 temp3D(temp4D.x, temp4D.y, temp4D.z);
		float angleRad = temp4D.par;
		// Operaci 'scaleOrientation' prevedu na rotaci
		outGeometry << "   Rotate " << RAD_TO_DEG(angleRad) << " " << temp3D << std::endl;
		outGeometry << "   Scale " << tempT->scale << std::endl;
		// Inverzni operaci '-scaleOrientation' ziskam jako rotaci o opacny uhel
		outGeometry << "   Rotate ";
		float angle = RAD_TO_DEG(angleRad);
		if (angle >= 0)
			outGeometry << angle << " " << temp3D << std::endl;
		else
			outGeometry << -angle << " " << temp3D << std::endl;
	}
	else {
		// Pokud byl definovan samostatny uzel 'Scale'
		if (tempT->hasScale())
			outGeometry << "   Scale " << tempT->scale << std::endl;
	}

	// Perform the Inverse Translation for the Center operation
	if (tempWInfo->retCurrentOpening(animInfo) > 0) {

		// Now test how the window should be opened, if vertically or horizontally
		if (tempWInfo->retOpenType() == 0) {
			vec3 vecAxis = tempWInfo->retAxis();
			if ((tempWInfo->retObjectConstructTypeNumber() < 5) || tempWInfo->retObjectConstructTypeNumber() == 7) {
				outGeometry << "   Translate " << -vecAxis.x << " " << -vecAxis.y << " " << -vecAxis.z << std::endl;
			}
			else if (tempWInfo->retObjectConstructTypeNumber() == 5) {
				outGeometry << "   Translate " << -vecAxis.x - CENTER_WINDOW_TYPE_56 << " " << -vecAxis.y << " " << -vecAxis.z << std::endl;
			}
			else if (tempWInfo->retObjectConstructTypeNumber() == 6) {
				outGeometry << "   Translate " << -vecAxis.x + CENTER_WINDOW_TYPE_56 << " " << -vecAxis.y << " " << -vecAxis.z << std::endl;
			}
		}
		else {
			outGeometry << "   Translate 0 " << CENTER_WINDOW_VERTICAL << " 0" << std::endl;
		}
	}


	// Najdu uzly, ktere jsou potomci tohoto VRML uzlu 'Transform' --O
	writeNodeChildren(tempT); //--O

	outGeometry << " AttributeEnd" << std::endl << std::endl;
}


//----------------------------------------------------------------------------------------------
void PbrtExporter::pbrtTransformWindowHandle(TransformNode* tempT)
//----------------------------------------------------------------------------------------------
{
	outGeometry << " AttributeBegin" << std::endl;
	// Pokud byl definovan uzel 'Translation'
	if (tempT->hasTranslation()) {
		outGeometry << "   Translate " << tempT->translation << std::endl;
	}

	// Perform the translation of the Center operation
	// get the parent with the handle rotation rate (parent->parent)
	TransformNode* tempParT = static_cast<TransformNode*> (tempT->parent->parent);

	outGeometry << "   Translate 0 " << CENTER_WINDOW_HANDLES << " 0" << std::endl;

	// Rotate the window handle accordingly
	WindowInfo* windowInfoT = static_cast<WindowInfo*> (tempParT->retObjectInfo());

	if ((windowInfoT->retObjectConstructTypeNumber() == 1) || (windowInfoT->retObjectConstructTypeNumber() == 5)) {
		outGeometry << "   Rotate " << RAD_TO_DEG(3.14 * (windowInfoT->retCurrentHandleRate(animInfo) / 100.0f)) << " 0 0 1" << std::endl;
	}
	else if ((windowInfoT->retObjectConstructTypeNumber() == 2) || (windowInfoT->retObjectConstructTypeNumber() == 6)) {
		outGeometry << "   Rotate " << RAD_TO_DEG(-3.14 * (windowInfoT->retCurrentHandleRate(animInfo) / 100.0f)) << " 0 0 1" << std::endl;
	}
	else if ((windowInfoT->retObjectConstructTypeNumber() == 3) || windowInfoT->retObjectConstructTypeNumber() == 7) {
		outGeometry << "   Rotate " << RAD_TO_DEG(3.14 * (windowInfoT->retCurrentHandleRate(animInfo) / 100.0f)) << " 1 0 0" << std::endl;
	}
	else if (windowInfoT->retObjectConstructTypeNumber() == 4) {
		outGeometry << "   Rotate " << RAD_TO_DEG(-3.14 * (windowInfoT->retCurrentHandleRate(animInfo) / 100.0f)) << " 1 0 0" << std::endl;
	}

	// Pokud byl definovan uzel 'scaleOrientation', pak ho prevedu na rotaci 'Rotate'
	if (tempT->hasScaleOrientation()) {
		vec4 temp4D = tempT->scaleOrientation;
		vec3 temp3D(temp4D.x, temp4D.y, temp4D.z);
		float angleRad = temp4D.par;
		// Operaci 'scaleOrientation' prevedu na rotaci
		outGeometry << "   Rotate " << RAD_TO_DEG(angleRad) << " " << temp3D << std::endl;
		outGeometry << "   Scale " << tempT->scale << std::endl;
		// Inverzni operaci '-scaleOrientation' ziskam jako rotaci o opacny uhel
		outGeometry << "   Rotate ";
		float angle = RAD_TO_DEG(angleRad);
		if (angle >= 0)
			outGeometry << angle << " " << temp3D << std::endl;
		else
			outGeometry << -angle << " " << temp3D << std::endl;
	}
	else {
		// Pokud byl definovan samostatny uzel 'Scale'
		if (tempT->hasScale())
			outGeometry << "   Scale " << tempT->scale << std::endl;
	}

	// Perform the inverse translation of the Center operation	

	outGeometry << "   Translate 0 " << -CENTER_WINDOW_HANDLES << " 0" << std::endl;

	// Najdu uzly, ktere jsou potomci tohoto VRML uzlu 'Transform' --O
	writeNodeChildren(tempT); //--O

	outGeometry << " AttributeEnd" << std::endl << std::endl;
}


//----------------------------------------------------------------------------------------------
void PbrtExporter::pbrtTransformWindowShutter(TransformNode* tempT)
//----------------------------------------------------------------------------------------------
{
	// Check if this shuttrer object is being animated, in that case, set the correct rotation rate
	if (tempT->retObjectInfo()->retHasAnimated()) {

		ShutterInfo* tempSInfo = static_cast<ShutterInfo*> (tempT->retObjectInfo());

		setShutterRoll(tempT, (int)(tempSInfo->retCurrentRotation(animInfo)));
	}

	outGeometry << " AttributeBegin" << std::endl;

	// Perform the Translation operation
	vec3 trans = tempT->translation;

	// Compute the y-axis translation offset based on the difference between the shutter and the top and the amount of shutter opening
	// Compute it based on the type of the shutter
	float yOff = 0.0;

	ShutterInfo* tempSInfo = static_cast<ShutterInfo*> (tempT->retObjectInfo());

	if (tempSInfo->retObjectConstructType() == _TYPE_CONSTRUCT_1) {
		yOff = tempSInfo->retTopOffset() - abs(tempSInfo->retTopOffset() - trans.y) *
			(tempSInfo->retCurrentOpening(animInfo) / 100.0f);
	}
	else if (tempSInfo->retObjectConstructType() == _TYPE_CONSTRUCT_2) {
		float topOff = (float)(tempSInfo->retTopOffset() + SHIFT_SHUTTER_TYPE_2);
		yOff = topOff - abs(topOff - trans.y) * (tempSInfo->retCurrentOpening(animInfo) / 100.0f);
	}
	else if (tempSInfo->retObjectConstructType() == _TYPE_CONSTRUCT_3) {
		float topOff = SHIFT_SHUTTER_TYPE_3;
		yOff = (float)(trans.y + topOff * (1.0 - tempSInfo->retCurrentOpening(animInfo) / 100.0f));
	}
	else if (tempSInfo->retObjectConstructType() == _TYPE_CONSTRUCT_4) {
		float topOff = SHIFT_SHUTTER_TYPE_4;
		yOff = (float)(trans.y + topOff * (1.0 - tempSInfo->retCurrentOpening(animInfo) / 100.0f));
	}

	outGeometry << "   Translate " << trans.x << " " << yOff << " " << trans.z << std::endl;

	// Pokud byl definovan uzel 'Rotation'
	if (tempT->hasRotation()) {
		vec4 temp = tempT->rotation;
		vec3 tempV(temp.x, temp.y, temp.z);
		float angleRad = temp.par;
		outGeometry << "   Rotate " << RAD_TO_DEG(angleRad) << " " << tempV << std::endl;
	}


	// Perform the scaling operation
	if (!tempT->hasScaleOrientation()) {
		ShutterInfo* tempSInfo = static_cast<ShutterInfo*> (tempT->retObjectInfo());

		outGeometry << "   Scale 1 " << tempSInfo->retCurrentOpening(animInfo) / 100.0f << " 1 " << std::endl;
	}
	else {
		vec3 scaleVec = tempT->scale;

		// If the y scale was not used, just plug in the shutter opening rate
		if (scaleVec.y == 1) {
			ShutterInfo* tempSInfo = static_cast<ShutterInfo*> (tempT->retObjectInfo());

			outGeometry << "   Scale " << scaleVec.x << " " << tempSInfo->retCurrentOpening(animInfo) / 100.0f << " " <<
				scaleVec.z << std::endl;
		}
		// Else we have to use the previous y scale and calculate the correct scaling value
		else {
			ShutterInfo* tempSInfo = static_cast<ShutterInfo*> (tempT->retObjectInfo());

			float scaleY = (float)(1.0 + (tempSInfo->retCurrentOpening(animInfo) / 100.0f - 1.0f) + (scaleVec.y - 1.0f));

			outGeometry << "   Scale " << scaleVec.x << " " << scaleY << " " << scaleVec.z << std::endl;
		}
	}

	// Najdu uzly, ktere jsou potomci tohoto VRML uzlu 'Transform' --O
	writeNodeChildren(tempT); //--O

	outGeometry << " AttributeEnd" << std::endl << std::endl;
}
