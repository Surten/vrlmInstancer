#include "mitsubaExporter.h"

#include <filesystem>

#define RAD_TO_DEG(angle)   ((angle)*57.29577951308f)

MitsubaExporter::MitsubaExporter()
{
	matFile = nullptr;
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
	for (const auto& coord : geometry->coords)
	{
		out_geom << "v " << coord << std::endl;
	}
	for (const auto& texCoord : geometry->textureCoords)
	{
		out_geom << "vt " << texCoord << std::endl;
	}
	for (const auto& texCoord : geometry->normals)
	{
		out_geom << "vn " << texCoord << std::endl;
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
		std::string sceneGeometryFolder = scene->name;
		size_t ind = scene->name.rfind('/');
		if (ind != std::string::npos) sceneGeometryFolder = scene->name.substr(ind+1);
		sceneGeometryFolder = this->outputFolder + "/" + sceneGeometryFolder.substr(0, sceneGeometryFolder.size()-4);
		std::filesystem::create_directory(sceneGeometryFolder);
		for (auto geometry : scene->geometries)
		{
			writeGeometryToObj(geometry, sceneGeometryFolder);
		}
		std::cout << "Exported geometries of scene: " << scene->name << std::endl;
	}
}


void MitsubaExporter::exportScene(std::vector<Scene*> scenes, ViewPointNode * camera, std::string sceneFileName, std::string outputFolder, bool createNewGeometry, MaterialsFile* matFile)
{
	// figure out door animations...
	this->scenes = scenes;
	this->outputFolder = outputFolder;
	this->matFile = matFile;
	int depth = 0;

	out.open(this->outputFolder + "/" + sceneFileName + ".xml");

	// export all geometries into individual .obj files
	if (createNewGeometry && false)
	{
		writeAllGeometriesToObjFiles();
	}


	out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
	writeElementBeg("scene", { "version", "3.5.2" }, depth);
	writeIntegrator(depth + 1);
	writeSensor(camera, depth + 1);


	// create the scene xml with hierarchy
	for (auto scene : scenes)
	{
		scene->initShapeNodeTransformMatricies();
		std::string sceneGeometryFolder = scene->name;
		size_t ind = scene->name.rfind('/');
		if (ind != std::string::npos) sceneGeometryFolder = scene->name.substr(ind + 1);
		sceneGeometryFolder = sceneGeometryFolder.substr(0, sceneGeometryFolder.size() - 4);

		writeElement("include", { "filename", sceneGeometryFolder + ".xml" }, depth + 1);

		if (createNewGeometry)
		{
			outCurrentScene.open(this->outputFolder + "/" + sceneGeometryFolder + ".xml");
			outCurrentScene << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
			writeElementBegScene("scene", { "version", "3.5.2" }, depth);

			for (auto lightNode : scene->lights)
			{
				writeLight(lightNode, depth + 1);
			}
			for (auto shapeNode : scene->ShapeNodes)
			{
				if (!shapeNode->geometry) continue;
				if (shapeNode->usesOtherGeometry) continue;
				writeShapeGroup(shapeNode, sceneGeometryFolder, depth + 1);
			}
			for (auto shapeNode : scene->ShapeNodes)
			{
				if (!shapeNode->geometry) continue;

				writeShapeReference(shapeNode, sceneGeometryFolder, depth + 1);
			}

			writeElementEndScene("scene", depth);
			outCurrentScene.close();
		}

		std::cout << "Exported scene: " << scene->name << std::endl;
	}


	writeElementEnd("scene", depth);
	out.close();
}


void MitsubaExporter::writeIntegrator(int depth)
{
	writeElementBeg("integrator", { "type", "path" }, depth);
		writeElement("integer", { "name", "max_depth", "value", std::to_string(pathTracingMaxDepth) }, depth + 1);
	writeElementEnd("integrator", depth);
	
}

void MitsubaExporter::writeSensor(ViewPointNode* camera, int depth)
{
	vec3 pos, lookAt, lookUp;
	float fov;
	if (camera == nullptr)
	{


		//for (auto node : scenes[0]->RootNodes)
		//{
		//	if (node->type == NodeTypes::Transform)
		//	{
		//		static_cast<TransformNode*>(node)->translation.setVector(0, 0, 0);
		//	}
		//}

		AABB retAABB = scenes[0]->getSceneAABB();

		vec3 cameraPosition(-20000, 12000, 20000);
		cameraPosition.normalize();
		float length = retAABB.getDiagonal().len();

		float customCameraZoom = 1.4;
		length = length * customCameraZoom;
		cameraPosition = cameraPosition * length;

		cameraPosition = cameraPosition + (retAABB.getArithmeticCenter() - vec3());

		std::cout << retAABB.min << "   " << retAABB.max << std::endl;
		std::cout << cameraPosition << "   " << retAABB.getArithmeticCenter() << std::endl;

		vec3 Up(0.0f, 1.0f, 0.0f);
		Up.normalize();

		pos = cameraPosition;
		lookAt = retAABB.getArithmeticCenter();
		lookUp = Up;
		fov = 50.f;
	}
	else
	{
		camera->computeLookAt(pos, lookAt, lookUp);
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

void MitsubaExporter::writeMaterial(Material* mat, int depth)
{
	vec3 diffTest(1.f, 1.f, 1.f);
	writeElementBegScene("bsdf", { "type", "diffuse" }, depth);
		writeElementScene("rgb", { "name", "reflectance", "value", diffTest.toString() }, depth + 1);
	writeElementEndScene("bsdf", depth);
}

void MitsubaExporter::writeShape(ShapeNode* shapeNode, std::string filepath, int depth)
{
	writeElementBegScene("shape", { "type", "obj" }, depth);
		writeElementScene("string", { "name", "filename", "value", filepath + "/" + shapeNode->geometry->name + ".obj"}, depth + 1);
		//writeBsdf(shapeNode->material, depth + 1);
		writeMaterial(shapeNode->material, depth + 1);
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
	writeElementBegScene("transform", { "name", "to_world" }, depth);
		writeElementScene("matrix", { "value", shapeNode->transformFromRootMatrix->GetAsString()}, depth + 1);
	writeElementEndScene("transform", depth);
}

void MitsubaExporter::writeBsdfNamed(Material* material, int depth)
{
	writeElementBegScene("bsdf", { "type", "twosided", "id", "TODO" }, depth);
		writeBsdf(material, depth + 1);
	writeElementEndScene("bsdf", depth);
}

void MitsubaExporter::writeBsdf(Material* material, int depth)
{
	Mat* mat = this->matFile->retMaterial(material->diffuseColor);
	if (mat == nullptr) {
		writeElementBegScene("bsdf", { "type", "diffuse" }, depth);
			writeElementScene("rgb", { "name", "reflectance", "value", material->diffuseColor.toString() }, depth + 1);
		writeElementEndScene("bsdf", depth);
		return;
	}



	if (mat->materialType == MaterialType::DIFFUSE)
	{
		writeElementBegScene("bsdf", { "type", "diffuse" }, depth);
			writeElementScene("rgb", { "name", "reflectance", "value", mat->Kd.toString()}, depth + 1);
		writeElementEndScene("bsdf", depth);
	}
	else if(mat->materialType == MaterialType::COATED_DIFFUSE)
	{
		writeElementBegScene("bsdf", { "type", "roughplastic" }, depth);
			writeElementScene("rgb", { "name", "diffuse_reflectance", "value", mat->Kd.toString()}, depth + 1);
			writeElementScene("float", { "name", "alpha", "value", std::to_string(mat->roughness) }, depth + 1);
		writeElementEndScene("bsdf", depth);
	}
	else if(mat->materialType == MaterialType::DIFFUSE_TRANSMISSIVE)
	{
		writeElementBegScene("bsdf", { "type", "roughdielectric" }, depth);
			writeElementScene("float", { "name", "alpha", "value", "0.5"}, depth + 1);
		writeElementEndScene("bsdf", depth);

	}
	else if(mat->materialType == MaterialType::DIELECTRIC)
	{
		writeElementBegScene("bsdf", { "type", "dielectric" }, depth);
		writeElementScene("float", { "name", "int_ior", "value", "1.1"}, depth + 1);
		writeElementScene("float", { "name", "ext_ior", "value", "1.0"}, depth + 1);
		writeElementEndScene("bsdf", depth);
	}
	else if(mat->materialType == MaterialType::CONDUCTOR)
	{
		std::string metal = mat->eta_str.substr(mat->eta_str.find("-") + 1);
		metal = metal.substr(0, metal.find("-"));
		writeElementBegScene("bsdf", { "type", "roughconductor" }, depth);
		writeElementScene("string", { "name", "material", "value", metal }, depth + 1);
		writeElementScene("float", { "name", "alpha", "value", std::to_string(std::min(mat->roughness*4, 1.f)) }, depth + 1);
		writeElementEndScene("bsdf", depth);
	}
	else if(mat->materialType == MaterialType::CONDUCTOR_REFLECTANCE)
	{
		writeElementBegScene("bsdf", { "type", "plastic" }, depth);
		writeElementScene("rgb", { "name", "diffuse_reflectance", "value", mat->reflect.toString() }, depth + 1);
		writeElementEndScene("bsdf", depth);
	}
	else if(mat->materialType == MaterialType::COATED_CONDUCTOR)
	{
		std::string metal = mat->eta_str.substr(mat->eta_str.find("-") + 1);
		metal = metal.substr(0, metal.find("-"));
		writeElementBegScene("bsdf", { "type", "roughconductor" }, depth);
		writeElementScene("string", { "name", "material", "value", metal }, depth + 1);
		writeElementScene("float", { "name", "alpha", "value", std::to_string(std::min(mat->roughness * 4, 1.f)) }, depth + 1);
		writeElementEndScene("bsdf", depth);
	}
	else
	{
		writeElementBegScene("bsdf", { "type", "diffuse" }, depth);
			writeElementScene("rgb", { "name", "reflectance", "value", material->diffuseColor.toString()}, depth + 1);
		writeElementEndScene("bsdf", depth);
	}


}

void MitsubaExporter::writeLight(LightNode* lightNode, int depth)
{
	if (false)
	{
		writeElementBegScene("emitter", { "type", "point" }, depth);
			writeElementScene("rgb", { "name", "intensity", "value", lightNode->color.toString()}, depth + 1);
			writeElementScene("point", { "name", "position", "x", std::to_string(lightNode->location.x), "y", std::to_string(lightNode->location.y), "z", std::to_string(lightNode->location.z), }, depth + 1);
		writeElementEndScene("emitter", depth);
	}
	else if (lightNode->lightType == LightNode::LightType::SPOTLIGHT)
	{
		float mult = 106;
		writeElementBegScene("emitter", { "type", "spot" }, depth);
			writeElementBegScene("transform", { "name", "to_world" }, depth + 1);
				writeElementScene("lookat", { "origin", lightNode->location.toString(), "target", (lightNode->location + lightNode->direction).toString()}, depth + 2);
			writeElementEndScene("transform", depth + 1);
			writeElementScene("float", { "name", "intensity", "value", std::to_string((lightNode->color * lightNode->intensity).len()* mult) }, depth + 1);
			writeElementScene("float", { "name", "beam_width", "value", std::to_string(RAD_TO_DEG(lightNode->beamWidth)) }, depth + 1);
			writeElementScene("float", { "name", "cutoff_angle", "value", std::to_string(RAD_TO_DEG(lightNode->cutOffAngle)*2) }, depth + 1);
		writeElementEndScene("emitter", depth);
	}
	else if (lightNode->lightType == LightNode::LightType::GONIOLIGHT)
	{
		writeElementBegScene("emitter", { "type", "spot" }, depth);
			writeElementBegScene("transform", { "name", "to_world" }, depth + 1);
			writeElementScene("lookat", { "origin", lightNode->location.toString(), "target", (lightNode->location + lightNode->direction).toString() }, depth + 2);
			writeElementEndScene("transform", depth + 1);
			writeElementScene("float", { "name", "intensity", "value", std::to_string(lightNode->intensity)}, depth + 1);
			writeElementScene("float", { "name", "beam_width", "value", "100"}, depth + 1);
			writeElementScene("float", { "name", "cutoff_angle", "value", "50"}, depth + 1);
		writeElementEndScene("emitter", depth);
	}
	else if (lightNode->lightType == LightNode::LightType::ENVIROMENTAL_LIGHT)
	{
		writeElementBegScene("emitter", { "type", "envmap" }, depth);
			writeElementScene("string", { "name", "filename", "value", lightNode->url }, depth + 1);
		writeElementEndScene("emitter", depth);
	}
}