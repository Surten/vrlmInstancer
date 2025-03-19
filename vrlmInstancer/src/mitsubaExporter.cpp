#include "mitsubaExporter.h"



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

std::string& MitsubaExporter::getLeadingSpaces(int depth)
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
	// how hard can it be to write a .obj exporter? (:
}

void MitsubaExporter::writeAllGeometriesToObjFiles()
{
	for (auto scene : scenes)
	{
		for (auto geometry : scene->geometries)
		{
			writeGeometryToObj(geometry, scene->name);
		}
	}
}


void MitsubaExporter::exportScene(std::vector<Scene*> scenes, ViewPointNode * camera, std::string sceneFileName)
{
	// figure out door animations...
	this->scenes = scenes;
	out.open(sceneFileName + ".xml");

	// create the scene xml with hierarchy
	for (auto scene : scenes)
	{
		for (auto shapeNode : scene->ShapeNodes)
		{

		}
	}
	out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
	int depth = 0;
	writeElementBeg("scene", { "version", "0.5.0" }, depth);
	writeIntegrator(depth + 1);
	writeSensor(depth + 1);

	// export all geometries into individual .obj files
	writeAllGeometriesToObjFiles();




	writeElementEnd("scene", depth);
	out.close();
}


void MitsubaExporter::writeIntegrator(int depth)
{
	writeElementBeg("integrator", { "type", "path" }, depth);
		writeElement("integer", { "name", "maxDepth", "value", std::to_string(pathTracingMaxDepth) }, depth + 1);
		writeElement("integer", { "name", "strictNormals", "value", "true" }, depth + 1);
	writeElementEnd("integrator", depth);
	
}

void MitsubaExporter::writeSensor(int depth)
{
	writeElementBeg("sensor", { "type", "perspective" }, depth);
		writeElement("float", { "name", "fov", "value", std::to_string(cameraFOV) }, depth + 1);
		writeElementBeg("transform", { "name", "toWorld" }, depth + 1);
			writeElement("matrix", { "value", "1 0 0 0  0 1 0 0  0 0 1 0  0 0 0 1" }, depth + 2);
		writeElementEnd("transform", depth + 1);
		writeSampler(depth + 1);
		writeFilm(depth + 1);
	writeElementEnd("sensor", depth);
}

void MitsubaExporter::writeSampler(int depth)
{
	writeElementBeg("sampler", { "type", "sobol" }, depth);
		writeElement("integer", { "name", "sampleCount", "value", std::to_string(nSamples) }, depth + 1);
	writeElementEnd("sampler", depth);
}

void MitsubaExporter::writeFilm(int depth)
{
	writeElementBeg("film", { "type", "ldrfilm" }, depth);
		writeElement("integer", { "name", "width", "value", std::to_string(imageWidth) }, depth + 1);
		writeElement("integer", { "name", "height", "value", std::to_string(imageHeight) }, depth + 1);
		writeElement("string", { "name", "fileFormat", "value", "png"}, depth + 1);
		writeElement("string", { "name", "pixelFormat", "value", "rgb"}, depth + 1);
		writeElement("float", { "name", "gamma", "value", "2.2"}, depth + 1);
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
		writeElement("string", { "name", "filename", "value", shapeNode->geometry->name }, depth + 1);
		writeTransform(shapeNode, depth + 1);
	writeElementEnd("shape", depth);
}

void MitsubaExporter::writeTransform(ShapeNode* shapeNode, int depth)
{
	writeElementBeg("transform", { "name", "toWorld" }, depth);
		writeElement("matrix", { "value", shapeNode->transformFromRootMatrix->GetAsString()}, depth + 1);
	writeElementEnd("transform", depth);
}

void MitsubaExporter::writeBsdfNamed(Material* material, int depth)
{
	writeElementBeg("bsdf", { "name", "toWorld" }, depth);
		writeBsdf(material, depth + 1);
	writeElementEnd("bsdf", depth);
}

void MitsubaExporter::writeBsdf(Material* material, int depth)
{
	writeElementBeg("bsdf", { "type", "twosided", "id", "TODO"}, depth);
		writeElement("rgb", { "name", "reflectance", "value", material->diffuseColor.toString()}, depth + 1);
	writeElementEnd("bsdf", depth);
}

void MitsubaExporter::writeLight(LightNode* lightNode, int depth)
{
	if (lightNode->lightType == LightNode::LightType::SPOTLIGHT)
	{
		writeElementBeg("emitter", { "type", "spot" }, depth);
			writeElementBeg("transform", { "name", "toWorld" }, depth + 1);
				writeElement("lookat", { "origin", lightNode->location.toString(), "target", (lightNode->location + lightNode->direction).toString()}, depth + 2);
			writeElementEnd("transform", depth + 1);
			writeElement("beamWidth", { "type", "float", "value", std::to_string(lightNode->beamWidth) }, depth + 1);
			writeElement("cutoffAngle", { "type", "float", "value", std::to_string(lightNode->cutOffAngle) }, depth + 1);
		writeElementEnd("bsdf", depth);
	}
	else
	{
		std::cout << "unsupported light" << std::endl;
	}
}