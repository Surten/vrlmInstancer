#include "mitsubaExporter.h"

#include <filesystem>

#define RAD_TO_DEG(angle)   ((angle)*57.29577951308f)

MitsubaExporter::MitsubaExporter()
{

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
	if (geometry->facesTextureIndex.size() > 0)
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
		std::string sceneGeometryFolder = this->outputFolder + "/" + scene->name.substr(0, scene->name.size()-4);
		std::filesystem::create_directory(sceneGeometryFolder);
		for (auto geometry : scene->geometries)
		{
			writeGeometryToObj(geometry, sceneGeometryFolder);
		}
	}
}


void MitsubaExporter::exportScene(std::vector<Scene*> scenes, ViewPointNode * camera, std::string sceneFileName, std::string outputFolder)
{
	// figure out door animations...
	this->scenes = scenes;
	this->outputFolder = outputFolder;
	int depth = 0;

	out.open(this->outputFolder + "/" + sceneFileName + ".xml");

	// export all geometries into individual .obj files
	writeAllGeometriesToObjFiles();
	out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
	writeElementBeg("scene", { "version", "3.0.0" }, depth);
	writeIntegrator(depth + 1);
	writeSensor(camera, depth + 1);


	// create the scene xml with hierarchy
	for (auto scene : scenes)
	{
		scene->initShapeNodeTransformMatricies();
		for (auto lightNode : scene->lights)
		{
			writeLight(lightNode, depth + 1);
		}
		for (auto shapeNode : scene->ShapeNodes)
		{
			writeShape(shapeNode, scene->name.substr(0, scene->name.size() - 4) + "/" + shapeNode->geometry->name, depth + 1);
		}
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
	camera->computeLookAt(pos, lookAt, lookUp);

	writeElementBeg("sensor", { "type", "perspective" }, depth);
		writeElement("float", { "name", "fov", "value", std::to_string(RAD_TO_DEG(camera->fieldOfView)) }, depth + 1);
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
		writeElement("string", { "name", "pixel_format", "value", "rgba"}, depth + 1);
		writeElement("boolean", { "name", "banner", "value", "false"}, depth + 1);
		writeElement("rfilter", { "type", "tent"}, depth + 1);
	writeElementEnd("film", depth);
}

void MitsubaExporter::writeMaterial(Material* mat, int depth)
{
	writeElementBeg("bsdf", { "type", "diffuse" }, depth);
		writeElement("rgb", { "name", "reflectance", "value", mat->diffuseColor.toString() }, depth + 1);
	writeElementEnd("bsdf", depth);
}

void MitsubaExporter::writeShape(ShapeNode* shapeNode, std::string filename, int depth)
{
	writeElementBeg("shape", { "type", "obj" }, depth);
		writeElement("string", { "name", "filename", "value", filename + ".obj"}, depth + 1);
		writeTransform(shapeNode, depth + 1);
		writeBsdf(shapeNode->material, depth + 1);
	writeElementEnd("shape", depth);
}

void MitsubaExporter::writeTransform(ShapeNode* shapeNode, int depth)
{
	writeElementBeg("transform", { "name", "to_world" }, depth);
		writeElement("matrix", { "value", shapeNode->transformFromRootMatrix->GetAsString()}, depth + 1);
	writeElementEnd("transform", depth);
}

void MitsubaExporter::writeBsdfNamed(Material* material, int depth)
{
	writeElementBeg("bsdf", { "type", "twosided", "id", "TODO" }, depth);
		writeBsdf(material, depth + 1);
	writeElementEnd("bsdf", depth);
}

void MitsubaExporter::writeBsdf(Material* material, int depth)
{
	writeElementBeg("bsdf", { "type", "diffuse" }, depth);
		writeElement("rgb", { "name", "reflectance", "value", material->diffuseColor.toString()}, depth + 1);
	writeElementEnd("bsdf", depth);
}

void MitsubaExporter::writeLight(LightNode* lightNode, int depth)
{
	if (true)
	{
		writeElementBeg("emitter", { "type", "point" }, depth);
			writeElement("rgb", { "name", "intensity", "value", lightNode->color.toString()}, depth + 1);
			writeElement("point", { "name", "position", "x", std::to_string(lightNode->location.x), "y", std::to_string(lightNode->location.y), "z", std::to_string(lightNode->location.z), }, depth + 1);
		writeElementEnd("emitter", depth);
	}
	else if (lightNode->lightType == LightNode::LightType::SPOTLIGHT)
	{
		writeElementBeg("emitter", { "type", "spot" }, depth);
			writeElementBeg("transform", { "name", "to_world" }, depth + 1);
				writeElement("lookat", { "origin", lightNode->location.toString(), "target", (lightNode->location + lightNode->direction).toString()}, depth + 2);
			writeElementEnd("transform", depth + 1);
			writeElement("beam_width", { "type", "float", "value", std::to_string(lightNode->beamWidth) }, depth + 1);
			writeElement("cutoff_angle", { "type", "float", "value", std::to_string(lightNode->cutOffAngle) }, depth + 1);
		writeElementEnd("emitter", depth);
	}
	else if (lightNode->lightType == LightNode::LightType::GONIOLIGHT)
	{
		writeElementBeg("emitter", { "type", "spot" }, depth);
			writeElementBeg("transform", { "name", "to_world" }, depth + 1);
			writeElement("lookat", { "origin", lightNode->location.toString(), "target", (lightNode->location + lightNode->direction).toString() }, depth + 2);
			writeElementEnd("transform", depth + 1);
			//writeElement("intensity", { "type", "float", "value", std::to_string(lightNode->intensity)}, depth + 1);
			writeElement("beam_width", { "type", "float", "value", "80"}, depth + 1);
			writeElement("cutoff_angle", { "type", "float", "value", "40"}, depth + 1);
		writeElementEnd("emitter", depth);
	}
}