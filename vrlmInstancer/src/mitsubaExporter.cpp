#include "mitsubaExporter.h"

#include"geometryModify.h"
#include "constants.h"
#include "dataStructs.h"

#include <filesystem>
#include <sstream>
#include <stack>

#define RAD_TO_DEG(angle)   ((angle)*57.29577951308f)

MitsubaExporter::MitsubaExporter(AnimationInfo* animInfo) : animInfo(animInfo), camera(nullptr)
{
	matFile = nullptr;
}

void MitsubaExporter::exportScene(std::vector<Scene*> scenes, ViewPointNode* camera, std::string sceneFileName,
	std::string outputFolder, std::string integrator, int width, int height, int samples,
	bool createNewGeometry, MaterialsFile* matFile, AnimationInfo* animInfo)
{
	this->imageWidth = width;
	this->imageHeight = height;
	this->nSamples = samples;
	this->animInfo = animInfo;
	this->scenes = scenes;
	this->outputFolder = outputFolder;
	this->camera = camera;
	this->sceneFileName = sceneFileName;
	this->integrator = integrator;
	this->matFile = matFile;
	this->createNewGeometry = createNewGeometry;

	std::filesystem::create_directory(this->outputFolder);

	// export all geometries into individual .obj files
	if (createNewGeometry)
	{
		writeAllGeometriesToObjFiles();
	}

	bool hasAnimatedCamera = false;
	bool hasAnimatedGeometry = false;
	for (Scene* scene : scenes)
	{
		scene->initShapeNodeTransformMatricies();
		if (scene->hasAnimatedCamera)
			hasAnimatedCamera = true;
		if (scene->isAnimated)
			hasAnimatedGeometry = true;

	}
	if (!hasAnimatedCamera && !hasAnimatedGeometry)
	{
		exportStatic();
	}
	else
	{
		exportDynamic();
	}
	
}

void MitsubaExporter::exportStatic()
{
	int depth = 0;
	out.open(outputFolder + "/" + sceneFileName + ".xml");
	out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
	writeElementBeg("scene", { "version", "3.5.2" }, depth);
	writeIntegrator(depth + 1);
	writeSensor(depth + 1);
	writeAllNamedMaterials(depth + 1);

	for (auto scene : scenes)
	{
		scene->initShapeNodeTransformMatricies();
		initCurrentGeometryFilename(scene->name, false);
		writeAllLights(scene, depth);
		writeElement("include", { "filename", currentGeometryFileName + ".xml" }, depth + 1);
		writeGeometryXML(scene, depth);
		std::cout << "Exported scene: " << scene->name << std::endl;
	}

	writeElementEnd("scene", depth);
	out.close();
}


void MitsubaExporter::exportDynamic() 
{
	int depth = 0;
	for (Scene* scene : scenes)
	{
		if (scene->isAnimated) continue;
		initCurrentGeometryFilename(scene->name, false);
		writeGeometryXML(scene, depth);
	}

	int numberFiles = (int)(floor(animInfo->retAnimLength() * animInfo->retAnimFps()) + 1);
	for (int i = 0; i < numberFiles; i++)
	{
		std::string indexStr = getStringWithLeadingZeros(i);
		out.open(outputFolder + "/" + sceneFileName + indexStr + ".xml");
		out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
		writeElementBeg("scene", { "version", "3.5.2" }, depth);
		writeIntegrator(depth + 1);
		writeSensor(depth + 1);
		writeAllNamedMaterials(depth + 1);

		for (auto scene : scenes)
		{
			if (scene->isAnimated)
			{
				initCurrentGeometryFilename(scene->name, true, i);
				writeGeometryXML(scene, depth);
			}
			else
				initCurrentGeometryFilename(scene->name, false);


			writeElement("include", { "filename", currentGeometryFileName + ".xml" }, depth + 1);
		}

		writeElementEnd("scene", depth);
		out.close();
		std::cout << "Exported frame " << i << std::endl;

		animInfo->incCurrentFrame();
		animInfo->incCurrentTime();
	}
}


std::string MitsubaExporter::getStringWithLeadingZeros(int number) {
	std::stringstream ss;
	ss << std::setw(3) << std::setfill('0') << number;
	std::string s = ss.str();
	// Ensure we only have up to three leading zeros
	if (s.length() > 3) {
		return s.substr(s.length() - 3);
	}
	return s;
}

void MitsubaExporter::initCurrentGeometryFilename(const std::string& sceneName, bool animated, int animationTimeIndex)
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
		currentGeometryFileName = geometryFileName + fileNumber;
	}
	else
		currentGeometryFileName = geometryFileName;

	currentGeometryFolderPath = geometryFileName;
}


void MitsubaExporter::writeElement(std::string elementName, std::vector<std::string> attributes, int depth)
{
	std::string leadingSpaces = getLeadingSpaces(depth);
	out << leadingSpaces << "<" << elementName << " ";
	for (int i = 0; i < attributes.size(); i+=2)
	{
		out << attributes[i] << "=\"" << attributes[i + 1] << "\" ";
	}
	out << "/>" << std::endl;
}

void MitsubaExporter::writeElementBeg(std::string elementName, std::vector<std::string> attributes, int depth)
{
	std::string leadingSpaces = getLeadingSpaces(depth);
	out << leadingSpaces << "<" << elementName << " ";
	for (int i = 0; i < attributes.size(); i += 2)
	{
		out << attributes[i] << "=\"" << attributes[i + 1] << "\" ";
	}
	out << ">" << std::endl;
}

void MitsubaExporter::writeElementEnd(std::string elementName, int depth)
{
	std::string leadingSpaces = getLeadingSpaces(depth);
	out << leadingSpaces << "</" << elementName << ">" << std::endl;
}

void MitsubaExporter::writeElementScene(std::string elementName, std::vector<std::string> attributes, int depth)
{
	std::string leadingSpaces = getLeadingSpaces(depth);
	outCurrentScene << leadingSpaces << "<" << elementName << " ";
	for (int i = 0; i < attributes.size(); i+=2)
	{
		outCurrentScene << attributes[i] << "=\"" << attributes[i + 1] << "\" ";
	}
	outCurrentScene << "/>" << std::endl;
}

void MitsubaExporter::writeElementBegScene(std::string elementName, std::vector<std::string> attributes, int depth)
{
	std::string leadingSpaces = getLeadingSpaces(depth);
	outCurrentScene << leadingSpaces << "<" << elementName << " ";
	for (int i = 0; i < attributes.size(); i += 2)
	{
		outCurrentScene << attributes[i] << "=\"" << attributes[i + 1] << "\" ";
	}
	outCurrentScene << ">" << std::endl;
}

void MitsubaExporter::writeElementEndScene(std::string elementName, int depth)
{
	std::string leadingSpaces = getLeadingSpaces(depth);
	outCurrentScene << leadingSpaces << "</" << elementName << ">" << std::endl;
}

std::string MitsubaExporter::getLeadingSpaces(int depth)
{
	std::string ret = "";
	for (int i = 0; i < depth; i++)
	{
		ret += "    ";
	}
	return ret;
}

void MitsubaExporter::writeGeometryToObj(Geometry* geometry, std::string filePath)
{
	std::ofstream out_geom(filePath + "/" + geometry->name + ".obj");
	// how hard can it be to write a .obj exporter? (:
	for (const vec3& coord : geometry->coords)
	{
		out_geom << "v " << coord << std::endl;
	}
	for (const vec2& texCoord : geometry->textureCoords)
	{
		out_geom << "vt " << texCoord << std::endl;
	}
	for (vec3& normalCoord : geometry->normals)
	{
		if (normalCoord.len() < 0.01) normalCoord.y = 1;
		out_geom << "vn " << normalCoord << std::endl;
	}
	if (geometry->facesNormalIndex.size() > 0) {
		if (geometry->facesTextureIndex.size() > 0)
		{
			for (size_t i = 0; i < geometry->facesPointsIndex.size(); i++)
			{
				out_geom << "f "
					<< geometry->facesPointsIndex[i].x + 1 << "/" << geometry->facesTextureIndex[i].x + 1 << "/" << geometry->facesNormalIndex[i].x + 1 << " "
					<< geometry->facesPointsIndex[i].y + 1 << "/" << geometry->facesTextureIndex[i].y + 1 << "/" << geometry->facesNormalIndex[i].y + 1 << " "
					<< geometry->facesPointsIndex[i].z + 1 << "/" << geometry->facesTextureIndex[i].z + 1 << "/" << geometry->facesNormalIndex[i].z + 1 << " "
					<< std::endl;
			}
		}
		else
		{
			for (size_t i = 0; i < geometry->facesPointsIndex.size(); i++)
			{
				out_geom << "f "
					<< geometry->facesPointsIndex[i].x + 1 << "//" << geometry->facesNormalIndex[i].x + 1 << " "
					<< geometry->facesPointsIndex[i].y + 1 << "//" << geometry->facesNormalIndex[i].y + 1 << " "
					<< geometry->facesPointsIndex[i].z + 1 << "//" << geometry->facesNormalIndex[i].z + 1 << " "
					<< std::endl;
			}
		}
	}
	else if (geometry->facesTextureIndex.size() > 0)
	{
		for (size_t i = 0; i < geometry->facesPointsIndex.size(); i++)
		{
			out_geom << "f "
				<< geometry->facesPointsIndex[i].x + 1 << "/" << geometry->facesTextureIndex[i].x + 1 << " "
				<< geometry->facesPointsIndex[i].y + 1 << "/" << geometry->facesTextureIndex[i].y + 1 << " "
				<< geometry->facesPointsIndex[i].z + 1 << "/" << geometry->facesTextureIndex[i].z + 1 << " "
				<< std::endl;
		}
	}
	else
	{
		for (size_t i = 0; i < geometry->facesPointsIndex.size(); i++)
		{
			out_geom << "f "
				<< geometry->facesPointsIndex[i].x + 1 << " "
				<< geometry->facesPointsIndex[i].y + 1 << " "
				<< geometry->facesPointsIndex[i].z + 1 << " "
				<< std::endl;
		}
	}

	out_geom.close();
}

void MitsubaExporter::writeAllGeometriesToObjFiles()
{
	for (auto scene : scenes)
	{
		initCurrentGeometryFilename(scene->name);
		std::filesystem::create_directory(this->outputFolder + "/" + currentGeometryFolderPath);
		for (auto geometry : scene->geometries)
		{
			writeGeometryToObj(geometry, this->outputFolder + "/" + currentGeometryFolderPath);
		}
		std::cout << "Exported geometries of scene: " << scene->name << std::endl;
	}
}


void MitsubaExporter::writeIntegrator(int depth)
{

	writeElementBeg("integrator", { "type", integrator }, depth);
		//writeElement("integer", { "name", "max_depth", "value", std::to_string(pathTracingMaxDepth) }, depth + 1);
	writeElementEnd("integrator", depth);
	
}

void MitsubaExporter::writeSensor(int depth)
{
	vec3 pos, lookAt, lookUp;
	float fov;
	if (camera == nullptr)
	{

		AABB retAABB = scenes[0]->getSceneAABB();

		vec3 cameraPosition(-20000, 12000, 20000);
		cameraPosition.normalize();
		float length = retAABB.getDiagonal().len();

		float customCameraZoom = 1.4;
		length = length * customCameraZoom;
		cameraPosition = cameraPosition * length;

		cameraPosition = cameraPosition + (retAABB.getArithmeticCenter() - vec3());

		//std::cout << retAABB.min << "   " << retAABB.max << std::endl;
		//std::cout << cameraPosition << "   " << retAABB.getArithmeticCenter() << std::endl;

		vec3 Up(0.0f, 1.0f, 0.0f);
		Up.normalize();

		pos = cameraPosition;
		lookAt = retAABB.getArithmeticCenter();
		lookUp = Up;
		fov = 50.f;
	}
	else
	{
		camera->computeLookAt(animInfo, pos, lookAt, lookUp);
		fov = RAD_TO_DEG(camera->fieldOfView);
		fov = 1.3 * fov;
	}
	writeElementBeg("sensor", { "type", "perspective", "id", "camera"}, depth);
		writeElement("float", { "name", "fov", "value", std::to_string(fov) }, depth + 1);
		writeElementBeg("transform", { "name", "to_world" }, depth + 1);
			writeElement("lookat", { "origin", pos.toString(), "target", lookAt.toString(), "up", lookUp.toString() }, depth + 2);
		writeElementEnd("transform", depth + 1);
		writeSampler(depth + 1);
		writeFilm(depth + 1);
	writeElementEnd("sensor", depth);
}

void MitsubaExporter::writeSampler(int depth)
{
	writeElementBeg("sampler", { "type", "independent" }, depth);
		writeElement("integer", { "name", "sample_count", "value", std::to_string(nSamples) }, depth + 1);
	writeElementEnd("sampler", depth);
}

void MitsubaExporter::writeFilm(int depth)
{
	writeElementBeg("film", { "type", "hdrfilm" }, depth);
		writeElement("integer", { "name", "width", "value", std::to_string(imageWidth) }, depth + 1);
		writeElement("integer", { "name", "height", "value", std::to_string(imageHeight) }, depth + 1);
		writeElement("string", { "name", "file_format", "value", "openexr"}, depth + 1);
		writeElement("string", { "name", "pixel_format", "value", "rgb"}, depth + 1);
		writeElement("rfilter", { "type", "gaussian"}, depth + 1);
	writeElementEnd("film", depth);
}

void MitsubaExporter::writeGeometryXML(Scene* scene, int depth)
{
	if (createNewGeometry)
	{
		outCurrentScene.open(this->outputFolder + "/" + currentGeometryFileName + ".xml");

		outCurrentScene << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
		writeElementBegScene("scene", { "version", "3.5.2" }, depth);
		writeAllShapeGroups(scene, depth);
		writeAllShapeReferences(scene, depth);
		writeElementEndScene("scene", depth);

		outCurrentScene.close();
	}
}


void MitsubaExporter::writeAllShapeGroups(Scene* scene, int depth)
{
	for (auto shapeNode : scene->ShapeNodes)
	{
		if (!shapeNode->geometry) continue;
		if (shapeNode->usesOtherGeometry) continue;
		writeShapeGroup(shapeNode, currentGeometryFolderPath, depth + 1);
	}
}
void MitsubaExporter::writeAllShapeReferences(Scene* scene, int depth)
{
	for (auto shapeNode : scene->ShapeNodes)
	{
		if (!shapeNode->geometry) continue;

		writeShapeReference(shapeNode, currentGeometryFolderPath, depth + 1);
	}
}

void MitsubaExporter::writeShape(ShapeNode* shapeNode, std::string filepath, int depth)
{
	writeElementBegScene("shape", { "type", "obj" }, depth);
		writeElementScene("boolean", { "name", "face_normals", "value", "true"}, depth + 1);
		writeElementScene("string", { "name", "filename", "value", filepath + "/" + shapeNode->geometry->name + ".obj"}, depth + 1);
		writeBsdfReference(shapeNode->exportMaterial, shapeNode->geometry->textureCoords.size() > 0, depth + 1);
	writeElementEndScene("shape", depth);
}
void MitsubaExporter::writeShapeGroup(ShapeNode* shapeNode, std::string filepath, int depth)
{
	//if (shapeNode->geometry->name[0] == '_') shapeNode->geometry->name[0] = 'a';
	writeElementBegScene("shape", { "type", "shapegroup", "id", filepath + "_" + shapeNode->geometry->name}, depth);
		writeShape(shapeNode, filepath, depth + 1);
	writeElementEndScene("shape", depth);
}
void MitsubaExporter::writeShapeReference(ShapeNode* shapeNode, std::string filepath, int depth)
{
	writeElementBegScene("shape", { "type", "instance" }, depth);
		writeTransform(shapeNode, depth + 1);
		writeElementScene("ref", { "id", filepath + "_" + shapeNode->geometry->name }, depth + 1);
	writeElementEndScene("shape", depth);
}

void MitsubaExporter::writeTransform(ShapeNode* shapeNode, int depth)
{
	std::stack<Transform> transforms;
	TransformNode* parent = static_cast<TransformNode*>(shapeNode->parent);
	while (parent != nullptr)
	{
		// decide based on the code name of the node and proceed with a specific object
		if (CheckNodeNameCode(parent->name.c_str(), CODE_DOOR_MOVE)) {
			mitsubaTransformDoor(parent, transforms);
		}
		else if (CheckNodeNameCode(parent->name.c_str(), CODE_DOOR_HANDLES)) {
			mitsubaTransformDoorHandle(parent, transforms);
		}
		else if (CheckNodeNameCode(parent->name.c_str(), CODE_WINDOW_MOVE)) {
			mitsubaTransformWindow(parent, transforms);
		}
		else if (CheckNodeNameCode(parent->name.c_str(), CODE_WINDOW_HANDLES)) {
			mitsubaTransformWindowHandle(parent, transforms);
		}
		else if (CheckNodeNameCode(parent->name.c_str(), CODE_SHUTTER)) {
			mitsubaTransformWindowShutter(parent, transforms);
		}
		else {
			mitsubaTransformNodeBase(parent, transforms);
		}
		parent = static_cast<TransformNode*>(parent->parent);
	}
	Matrix m;
	m.applyTransforms(transforms);
	//if (!(*shapeNode->transformFromRootMatrix == m))
	//{
	//	std::cout << "unequal matricies" << std::endl;
	//}
	//Matrix m1;
	//m1.applyTransforms(transforms);
	writeElementBegScene("transform", { "name", "to_world" }, depth);
	writeElementScene("matrix", { "value", m.GetAsString() }, depth + 1);
	writeElementEndScene("transform", depth);
}

void MitsubaExporter::writeAllLights(Scene* scene, int depth)
{
	for (auto lightNode : scene->lights)
	{
		writeLight(lightNode, depth + 1);
	}
}


void MitsubaExporter::writeLight(LightNode* lightNode, int depth)
{
	if (lightNode->on == false) return;

	if (false)
	{
		writeElementBeg("emitter", { "type", "point" }, depth);
		writeElement("rgb", { "name", "intensity", "value", lightNode->color.toString() }, depth + 1);
		writeElement("point", { "name", "position", "x", std::to_string(lightNode->location.x), "y", std::to_string(lightNode->location.y), "z", std::to_string(lightNode->location.z), }, depth + 1);
		writeElementEnd("emitter", depth);
	}
	else if (lightNode->lightType == LightNode::LightType::SPOTLIGHT)
	{
		float mult = 15;
		writeElementBeg("emitter", { "type", "spot" }, depth);
		writeElementBeg("transform", { "name", "to_world" }, depth + 1);
		writeElement("lookat", { "origin", lightNode->location.toString(), "target", (lightNode->location + lightNode->direction).toString() }, depth + 2);
		writeElementEnd("transform", depth + 1);
		writeElement("float", { "name", "intensity", "value", std::to_string((lightNode->color * lightNode->intensity).len() * mult) }, depth + 1);
		writeElement("float", { "name", "beam_width", "value", std::to_string(RAD_TO_DEG(lightNode->beamWidth)) }, depth + 1);
		writeElement("float", { "name", "cutoff_angle", "value", std::to_string(RAD_TO_DEG(lightNode->cutOffAngle) * 1.3) }, depth + 1);
		writeElementEnd("emitter", depth);
	}
	else if (lightNode->lightType == LightNode::LightType::GONIOLIGHT)
	{
		float mult = 15;
		writeElementBeg("emitter", { "type", "spot" }, depth);
		writeElementBeg("transform", { "name", "to_world" }, depth + 1);
		writeElement("lookat", { "origin", lightNode->location.toString(), "target", (lightNode->location + lightNode->direction).toString() }, depth + 2);
		writeElementEnd("transform", depth + 1);
		writeElement("float", { "name", "intensity", "value", std::to_string((lightNode->color * lightNode->intensity).len() * mult) }, depth + 1);
		writeElement("float", { "name", "beam_width", "value", std::to_string(RAD_TO_DEG(lightNode->beamWidth)) }, depth + 1);
		writeElement("float", { "name", "cutoff_angle", "value", std::to_string(RAD_TO_DEG(lightNode->cutOffAngle) * 1.3) }, depth + 1);
		writeElementEnd("emitter", depth);
	}
	else if (lightNode->lightType == LightNode::LightType::ENVIROMENTAL_LIGHT)
	{
		writeElementBeg("emitter", { "type", "envmap" }, depth);
		writeElement("string", { "name", "filename", "value", lightNode->url }, depth + 1);
		writeElementEnd("emitter", depth);
	}
}

void MitsubaExporter::writeBsdfReference(Mat* material, bool hasTextureCoordinates, int depth)
{
	if(material->hasBTF && hasTextureCoordinates)
		writeElementScene("ref", { "name", "bsdf", "id", material->name + "BTF"}, depth);
	else
		writeElementScene("ref", { "name", "bsdf", "id", material->name }, depth);
}


void MitsubaExporter::writeAllNamedMaterials(int depth)
{
	for (Mat* material : matFile->materials)
	{
		writeBsdfNamed(material, depth + 1);
	}
}

void MitsubaExporter::writeBsdfNamed(Mat* material, int depth)
{

	if (material->materialType == MaterialType::DIFFUSE)
	{
		writeMaterialDiffuse(material, depth);
	}
	else if (material->materialType == MaterialType::COATED_DIFFUSE)
	{
		writeMaterialCoatedDiffuse(material, depth);
	}
	else if (material->materialType == MaterialType::DIFFUSE_TRANSMISSIVE)
	{
		writeMaterialDiffuseTransmission(material, depth);
	}
	else if (material->materialType == MaterialType::DIELECTRIC)
	{
		writeMaterialDielectric(material, depth);
	}
	else if (material->materialType == MaterialType::CONDUCTOR)
	{
		writeMaterialConductor(material, depth);
	}
	else if (material->materialType == MaterialType::CONDUCTOR_REFLECTANCE)
	{
		writeMaterialConductorReflectance(material, depth);
	}
	else if (material->materialType == MaterialType::COATED_CONDUCTOR)
	{
		writeMaterialCoatedConductor(material, depth);
	}
	else if (material->materialType == MaterialType::BTF_MATERIAL)
	{
		writeMaterialBTF(material, depth);
	}
	else
	{
		writeMaterialDiffuse(material, depth);
	}


}


void MitsubaExporter::writeMaterialDiffuse(Mat* material, int depth)
{
	writeElementBeg("bsdf", { "type", "twosided", "id", material->name }, depth);
	writeElementBeg("bsdf", { "type", "diffuse" }, depth + 1);
	writeElement("rgb", { "name", "reflectance", "value", material->Kd.toString() }, depth + 2);
	writeElementEnd("bsdf", depth + 1);
	writeElementEnd("bsdf", depth);
}
void MitsubaExporter::writeMaterialCoatedDiffuse(Mat* material, int depth)
{
	writeElementBeg("bsdf", { "type", "twosided", "id", material->name }, depth);
	writeElementBeg("bsdf", { "type", "roughplastic" }, depth + 1);
	writeElement("rgb", { "name", "diffuse_reflectance", "value", material->Kd.toString() }, depth + 2);
	writeElement("float", { "name", "alpha", "value", std::to_string(material->roughness) }, depth + 2);
	writeElementEnd("bsdf", depth + 1);
	writeElementEnd("bsdf", depth);
}
void MitsubaExporter::writeMaterialDiffuseTransmission(Mat* material, int depth)
{
	writeElementBeg("bsdf", { "type", "roughdielectric", "id", material->name }, depth);
	writeElement("float", { "name", "alpha", "value", "0.5" }, depth + 1);
	writeElementEnd("bsdf", depth);
}
void MitsubaExporter::writeMaterialDielectric(Mat* material, int depth)
{
	writeElementBeg("bsdf", { "type", "dielectric", "id", material->name }, depth);
	writeElement("float", { "name", "int_ior", "value", "1.5" }, depth + 1);
	writeElement("float", { "name", "ext_ior", "value", "1.0" }, depth + 1);
	writeElementEnd("bsdf", depth);
}
void MitsubaExporter::writeMaterialConductor(Mat* material, int depth)
{
	std::string metal = material->eta_str.substr(material->eta_str.find("-") + 1);
	metal = metal.substr(0, metal.find("-"));
	writeElementBeg("bsdf", { "type", "twosided", "id", material->name }, depth);
	writeElementBeg("bsdf", { "type", "roughconductor" }, depth + 1);
	writeElement("string", { "name", "material", "value", metal }, depth + 2);
	writeElement("float", { "name", "alpha", "value", std::to_string(std::min(material->roughness * 4, 1.f)) }, depth + 2);
	writeElementEnd("bsdf", depth + 1);
	writeElementEnd("bsdf", depth);

}
void MitsubaExporter::writeMaterialConductorReflectance(Mat* material, int depth)
{
	writeElementBeg("bsdf", { "type", "twosided", "id", material->name }, depth);
	writeElementBeg("bsdf", { "type", "plastic" }, depth + 1);
	writeElement("rgb", { "name", "diffuse_reflectance", "value", material->reflect.toString() }, depth + 2);
	writeElementEnd("bsdf", depth + 1);
	writeElementEnd("bsdf", depth);

}

void MitsubaExporter::writeMaterialCoatedConductor(Mat* material, int depth)
{
	std::string metal = material->eta_str.substr(material->eta_str.find("-") + 1);
	metal = metal.substr(0, metal.find("-"));
	writeElementBeg("bsdf", { "type", "twosided", "id", material->name }, depth);
	writeElementBeg("bsdf", { "type", "roughconductor" }, depth + 1);
	writeElement("string", { "name", "material", "value", metal }, depth + 2);
	writeElement("float", { "name", "alpha", "value", std::to_string(std::min(material->roughness * 4, 1.f)) }, depth + 2);
	writeElementEnd("bsdf", depth + 1);
	writeElementEnd("bsdf", depth);

}

void MitsubaExporter::writeMaterialBTF(Mat* material, int depth)
{
	writeElementBeg("bsdf", { "type", "twosided", "id", material->name }, depth);
	writeElementBeg("bsdf", { "type", "custombsdf" }, depth + 1);
	writeElement("string", { "name", "filename", "value", material->btfFileName }, depth + 2);
	writeElementEnd("bsdf", depth + 1);
	writeElementEnd("bsdf", depth);

}









void addTransformTranslate(TransformNode* transformNode, std::stack<Transform>& transforms)
{
	if (transformNode->hasTranslation()) {
		Transform t;
		t.createTranslate(transformNode->translation);
		transforms.push(t);
	}
}

void addTransformRotation(TransformNode* transformNode, std::stack<Transform>& transforms)
{
	if (transformNode->hasRotation()) {
		Transform t;
		t.createRotate(transformNode->rotation);
		transforms.push(t);
	}
}

void addTransformScaleOrientationorScale(TransformNode* transformNode, std::stack<Transform>& transforms) 
{
	// Pokud byl definovan uzel 'scaleOrientation', pak ho prevedu na rotaci 'Rotate'
	if (transformNode->hasScaleOrientation()) {
		vec4 temp4D = transformNode->scaleOrientation;
		vec3 temp3D(temp4D.x, temp4D.y, temp4D.z);
		float angleRad = temp4D.par;

		//if (angleRad < 0)
			angleRad = -angleRad;
		Transform t1, t2, t3;
		t1.createRotate(vec4(temp3D.x, temp3D.y, temp3D.z, angleRad));
		t2.createScale(transformNode->scale);
		t3.createRotate(temp4D);
		transforms.push(t1);
		transforms.push(t2);
		transforms.push(t3);
	}
	else {
		// Pokud byl definovan samostatny uzel 'Scale'
		if (transformNode->hasScale())
		{
			Transform t;
			t.createScale(transformNode->scale);
			transforms.push(t);
		}
	}
}




void MitsubaExporter::mitsubaTransformNodeBase(TransformNode* transformNode, std::stack<Transform>& transforms)
{
	addTransformScaleOrientationorScale(transformNode, transforms);
	addTransformRotation(transformNode, transforms);
	addTransformTranslate(transformNode, transforms);
}

void addTransformDoorRotation(TransformNode* transformNode, DoorInfo* tempDInfo, AnimationInfo* animInfo, std::stack<Transform>& transforms)
{
	// Rotate the door object 
	if (tempDInfo->retCurrentOpening(animInfo) > 0) {
		// Find the parent, which stores the information about the door object
		TransformNode* tempParT = static_cast<TransformNode*> (transformNode->parent);

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

		Transform t;
		t.createRotate(tempRot);
		transforms.push(t);
	}
}

void addTransformDoorCenterTranslation(TransformNode* transformNode, DoorInfo* tempDInfo, AnimationInfo* animInfo, std::stack<Transform>& transforms, bool isBackwards)
{
	if (tempDInfo->retCurrentOpening(animInfo) > 0) {
		vec3 vec;
		if ((tempDInfo->retObjectConstructTypeNumber() == 1) || (tempDInfo->retObjectConstructTypeNumber() == 2) ||
			(tempDInfo->retObjectConstructTypeNumber() == 7) ||
			(tempDInfo->retObjectConstructTypeNumber() == 10) || (tempDInfo->retObjectConstructTypeNumber() == 11) ||
			(tempDInfo->retObjectConstructTypeNumber() == 15) || (tempDInfo->retObjectConstructTypeNumber() == 16)) {

			vec = tempDInfo->retAxis();
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 3) {
			vec = vec3(0, 0, CENTER_DOOR_NO_JOINTS);
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 4) {
			vec = vec3(0, 0, -CENTER_DOOR_NO_JOINTS);
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 6) {
			vec = vec3(0, 0, -CENTER_DOOR_NO_JOINTS_4);
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 8) {
			vec3 vecAxis = tempDInfo->retAxis();
			vec = vec3(0, vecAxis.y, vecAxis.z);
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 9) {
			vec = vec3(0, 0, -CENTER_DOOR_NO_JOINTS_NARROW);
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 12) {
			vec = vec3(-CENTER_DOOR_NO_JOINTS_12, 0, 0);
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 13) {
			vec = vec3(0, 0, -CENTER_DOOR_NO_JOINTS_12);
		}
		else if (tempDInfo->retObjectConstructTypeNumber() == 14) {
			vec = vec3(CENTER_DOOR_NO_JOINTS_12, 0, 0);
		}
		if (isBackwards)
		{
			vec = vec3() - vec;
		}
		Transform t;
		t.createTranslate(vec);
		transforms.push(t);

	}
}


//----------------------------------------------------------------------------------------------
void MitsubaExporter::mitsubaTransformDoor(TransformNode* tempT, std::stack<Transform>& transforms)
//----------------------------------------------------------------------------------------------
{
	// Find the parent, which stores the information on the joint axis
	TransformNode* tempPar = static_cast<TransformNode*> (tempT->parent);
	// Get the doorInfo object
	DoorInfo* tempDInfo = static_cast<DoorInfo*> (tempPar->retObjectInfo());

	addTransformDoorCenterTranslation(tempT, tempDInfo, animInfo, transforms, true);
	addTransformScaleOrientationorScale(tempT, transforms);
	addTransformDoorRotation(tempT, tempDInfo, animInfo, transforms);
	addTransformDoorCenterTranslation(tempT, tempDInfo, animInfo, transforms, false);
	addTransformTranslate(tempT, transforms);


}

void addTransformDoorHandleCenterTranslation(TransformNode* transformNode, std::stack<Transform>& transforms, bool isBackwards)
{
	vec3 vec;
	TransformNode* tempParT = static_cast<TransformNode*> (transformNode->parent->parent);

	if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 1) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 3) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 7) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 8)) {
		vec = vec3(0, 0, -CENTER_DOOR_HANDLES);
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 2) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 4) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 9) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 11) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 13)) {
		vec = vec3(0, 0, CENTER_DOOR_HANDLES);
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 10) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 14) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 15)) {
		vec = vec3(-CENTER_DOOR_HANDLES, 0, 0);
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 12)) {
		vec = vec3(CENTER_DOOR_HANDLES, 0, 0);
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 16)) {
		vec = vec3(CENTER_DOOR_HANDLES3, 0, 0);
	}

	if (isBackwards)
	{
		vec = vec3() - vec;
	}
	Transform t;
	t.createTranslate(vec);
	transforms.push(t);
}

void addTransformDoorHandleRotation(TransformNode* transformNode, AnimationInfo* animInfo, std::stack<Transform>& transforms)
{
	TransformNode* tempParT = static_cast<TransformNode*> (transformNode->parent->parent);
	// Get the doorInfo object
	DoorInfo* doorInfoT = static_cast<DoorInfo*> (tempParT->retObjectInfo());

	vec4 vec;
	if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 1) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 3) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 7) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 8))
	{
		vec = vec4(1, 0, 0, (1.57 * (doorInfoT->retCurrentHandleRate(animInfo) / 100.0f)));
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 2) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 4) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 9) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 11) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 13))
	{
		vec = vec4(1, 0, 0, (-1.57 * (doorInfoT->retCurrentHandleRate(animInfo) / 100.0f)));
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 10) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 14) ||
		(tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 15))
	{
		vec = vec4(0, 0, 1, (-1.57 * (doorInfoT->retCurrentHandleRate(animInfo) / 100.0f)));
	}
	else if ((tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 12) || (tempParT->retObjectInfo()->retObjectConstructTypeNumber() == 16)) 
	{
		vec = vec4(0, 0, 1, (1.57 * (doorInfoT->retCurrentHandleRate(animInfo) / 100.0f)));
	}
	Transform t;
	t.createRotate(vec);
	transforms.push(t);
}


//----------------------------------------------------------------------------------------------
void MitsubaExporter::mitsubaTransformDoorHandle(TransformNode* tempT, std::stack<Transform>& transforms)
//----------------------------------------------------------------------------------------------
{
	addTransformDoorHandleCenterTranslation(tempT, transforms, true);
	addTransformScaleOrientationorScale(tempT, transforms);
	addTransformDoorHandleRotation(tempT, animInfo, transforms);
	addTransformRotation(tempT, transforms);
	addTransformDoorHandleCenterTranslation(tempT, transforms, false);
	addTransformTranslate(tempT, transforms);
}


void addTransformWindowTranslation(WindowInfo* tempWInfo,  AnimationInfo* animInfo, std::stack<Transform>& transforms, bool isBackwards)
{
	if (tempWInfo->retCurrentOpening(animInfo) > 0) {

		vec3 vec;
		// Now test how the window should be opened, if vertically or horizontally
		if (tempWInfo->retOpenType() == 0) {
			vec3 vecAxis = tempWInfo->retAxis();
			if ((tempWInfo->retObjectConstructTypeNumber() < 5) || tempWInfo->retObjectConstructTypeNumber() == 7) {
				vec = vecAxis;
			}
			else if (tempWInfo->retObjectConstructTypeNumber() == 5) {
				vec = vec3(vecAxis.x + CENTER_WINDOW_TYPE_56, vecAxis.y, vecAxis.z);
			}
			else if (tempWInfo->retObjectConstructTypeNumber() == 6) {
				vec = vec3(vecAxis.x - CENTER_WINDOW_TYPE_56, vecAxis.y, vecAxis.z);
			}
		}
		else {
			vec = vec3(0, -CENTER_WINDOW_VERTICAL, 0);
		}

		if (isBackwards)
		{
			vec = vec3() - vec;
		}
		Transform t;
		t.createTranslate(vec);
		transforms.push(t);
	}
}

void addTransformWindowRotation(WindowInfo* tempWInfo, AnimationInfo* animInfo, std::stack<Transform>& transforms)
{
	vec4 vec;
	// Check what type of rotation is supposed to be done
	float rotateRate = 0;

	if (tempWInfo->retCurrentOpenType(animInfo) == 0) {
		rotateRate = (float)(DEG_TO_RAD(90.0 * tempWInfo->retCurrentOpening(animInfo) / 100.0f));
		if ((tempWInfo->retObjectConstructTypeNumber() == 1) || (tempWInfo->retObjectConstructTypeNumber() == 3) ||
			(tempWInfo->retObjectConstructTypeNumber() == 5)) {
			vec = vec4(0, 1, 0, -rotateRate);
		}
		else {
			vec = vec4(0, 1, 0, rotateRate);
		}
	}
	else {
		rotateRate = (float)(DEG_TO_RAD(10.0 * tempWInfo->retCurrentOpening(animInfo) / 100.0f));
		if ((tempWInfo->retObjectConstructTypeNumber() == 1) || (tempWInfo->retObjectConstructTypeNumber() == 5)) {
			vec = vec4(1, 0, 0, -rotateRate);
		}
		else if ((tempWInfo->retObjectConstructTypeNumber() == 2) || (tempWInfo->retObjectConstructTypeNumber() == 6)) {
			vec = vec4(1, 0, 0, -rotateRate);
		}
		else if (tempWInfo->retObjectConstructTypeNumber() == 3) {
			vec = vec4(0, 0, 1, rotateRate);
		}
		else if (tempWInfo->retObjectConstructTypeNumber() == 4) {
			vec = vec4(0, 0, 1, rotateRate);
		}
		else if (tempWInfo->retObjectConstructTypeNumber() == 7) {
			vec = vec4(0, 0, 1, -rotateRate);
		}
	}
	Transform t;
	t.createRotate(vec);
	transforms.push(t);
}

//----------------------------------------------------------------------------------------------
void MitsubaExporter::mitsubaTransformWindow(TransformNode* tempT, std::stack<Transform>& transforms)
//----------------------------------------------------------------------------------------------
{

	// Get the windowInfo object
	TransformNode* tempPar = static_cast<TransformNode*> (tempT->parent);
	WindowInfo* tempWInfo = static_cast<WindowInfo*> (tempPar->retObjectInfo());

	addTransformWindowTranslation(tempWInfo, animInfo, transforms, true);
	addTransformScaleOrientationorScale(tempT, transforms);
	addTransformWindowRotation(tempWInfo, animInfo, transforms);
	addTransformRotation(tempT, transforms);
	addTransformWindowTranslation(tempWInfo, animInfo, transforms, false);
	addTransformTranslate(tempT, transforms);
}


void addTransformWindowHandleRotation(WindowInfo* tempWInfo, AnimationInfo* animInfo, std::stack<Transform>& transforms)
{
	// Rotate the window handle accordingly
	vec4 vec;
	if ((tempWInfo->retObjectConstructTypeNumber() == 1) || (tempWInfo->retObjectConstructTypeNumber() == 5)) {
		vec = vec4(0, 0, 1, 3.14 * (tempWInfo->retCurrentHandleRate(animInfo) / 100.0f));
	}
	else if ((tempWInfo->retObjectConstructTypeNumber() == 2) || (tempWInfo->retObjectConstructTypeNumber() == 6)) {
		vec = vec4(0, 0, 1, -3.14 * (tempWInfo->retCurrentHandleRate(animInfo) / 100.0f));
	}
	else if ((tempWInfo->retObjectConstructTypeNumber() == 3) || tempWInfo->retObjectConstructTypeNumber() == 7) {
		vec = vec4(1, 0, 0, 3.14 * (tempWInfo->retCurrentHandleRate(animInfo) / 100.0f));
	}
	else if (tempWInfo->retObjectConstructTypeNumber() == 4) {
		vec = vec4(1, 0, 0, -3.14 * (tempWInfo->retCurrentHandleRate(animInfo) / 100.0f));
	}
	Transform t;
	t.createRotate(vec);
	transforms.push(t);
}

//----------------------------------------------------------------------------------------------
void MitsubaExporter::mitsubaTransformWindowHandle(TransformNode* tempT, std::stack<Transform>& transforms)
//----------------------------------------------------------------------------------------------
{

	// Perform the translation of the Center operation
	// get the parent with the handle rotation rate (parent->parent)
	TransformNode* tempParT = static_cast<TransformNode*> (tempT->parent->parent);
	WindowInfo* windowInfoT = static_cast<WindowInfo*> (tempParT->retObjectInfo());

	Transform t2;
	t2.createTranslate(vec3(0, -CENTER_WINDOW_HANDLES, 0));
	transforms.push(t2);

	addTransformScaleOrientationorScale(tempT, transforms);
	addTransformWindowHandleRotation(windowInfoT, animInfo, transforms);

	Transform t1;
	t1.createTranslate(vec3(0, CENTER_WINDOW_HANDLES, 0));
	transforms.push(t1);

	addTransformTranslate(tempT, transforms);
}


//----------------------------------------------------------------------------------------------
void MitsubaExporter::mitsubaTransformWindowShutter(TransformNode* tempT, std::stack<Transform>& transforms)
//----------------------------------------------------------------------------------------------
{
	// Check if this shuttrer object is being animated, in that case, set the correct rotation rate
	if (tempT->retObjectInfo()->retHasAnimated()) {

		ShutterInfo* tempSInfo = static_cast<ShutterInfo*> (tempT->retObjectInfo());

		setShutterRoll(tempT, (int)(tempSInfo->retCurrentRotation(animInfo)));
	}


	// Perform the Translation operation
	vec3 trans = tempT->translation;

	// Compute the y-axis translation offset based on the difference between the shutter and the top and the amount of shutter opening
	// Compute it based on the type of the shutter
	float yOff = 0.0;

	ShutterInfo* tempSInfo = static_cast<ShutterInfo*> (tempT->retObjectInfo());

	if (tempSInfo->retObjectConstructType() == _TYPE_CONSTRUCT_1) {
		yOff = tempSInfo->retTopOffset() - abs(tempSInfo->retTopOffset() - trans.y) *
			(tempSInfo->retCurrentOpening(animInfo));
	}
	else if (tempSInfo->retObjectConstructType() == _TYPE_CONSTRUCT_2) {
		float topOff = (float)(tempSInfo->retTopOffset() + SHIFT_SHUTTER_TYPE_2);
		yOff = topOff - abs(topOff - trans.y) * (tempSInfo->retCurrentOpening(animInfo));
	}
	else if (tempSInfo->retObjectConstructType() == _TYPE_CONSTRUCT_3) {
		float topOff = SHIFT_SHUTTER_TYPE_3;
		yOff = (float)(trans.y + topOff * (1.0 - tempSInfo->retCurrentOpening(animInfo)));
	}
	else if (tempSInfo->retObjectConstructType() == _TYPE_CONSTRUCT_4) {
		float topOff = SHIFT_SHUTTER_TYPE_4;
		yOff = (float)(trans.y + topOff * (1.0 - tempSInfo->retCurrentOpening(animInfo)));
	}

	


	vec3 scaleVec;
	// Perform the scaling operation
	if (!tempT->hasScaleOrientation()) {
		scaleVec = vec3(1, tempSInfo->retCurrentOpening(animInfo) / 100.0f, 1);
	}
	else {
		vec3 scaleVec = tempT->scale;

		// If the y scale was not used, just plug in the shutter opening rate
		if (scaleVec.y == 1) {
			scaleVec = vec3(scaleVec.x, tempSInfo->retCurrentOpening(animInfo) / 100.0f, scaleVec.z);
		}
		// Else we have to use the previous y scale and calculate the correct scaling value
		else {
			float scaleY = (float)(1.0 + (tempSInfo->retCurrentOpening(animInfo) / 100.0f - 1.0f) + (scaleVec.y - 1.0f));

			scaleVec = vec3(scaleVec.x, scaleY, scaleVec.z);
		}
	}

	Transform t2;
	t2.createScale(vec3(trans.x, yOff, trans.z));
	transforms.push(t2);

	addTransformRotation(tempT, transforms);

	Transform t1;
	t1.createTranslate(vec3(trans.x, yOff, trans.z));
	transforms.push(t1);
}