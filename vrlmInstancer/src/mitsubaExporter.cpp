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

void MitsubaExporter::writeElementBegin(std::string elementName, std::vector<std::string> attributes, int depth)
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

void MitsubaExporter::exportScene(std::vector<Scene*> scenes, ViewPointNode * camera, std::string sceneFileName)
{
	this->scenes = scenes;
	out.open(sceneFileName + ".xml");

	out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
	//depth = 0;
	writeElementBegin("scene", { "version", "0.5.0" }, 0);
	writeIntegrator(1);
	writeSensor(1);

	// export all geometries into individual .obj files
	


	// figure out door animations...
	// create the scene xml with hierarchy

	writeElementEnd("scene", 0);
	out.close();
}

void MitsubaExporter::writeIntegrator(int depth)
{
	writeElementBegin("integrator", { "type", "path" }, depth);
	
	writeElement("integer", { "name", "maxDepth", "value", std::to_string(pathTracingMaxDepth) }, depth + 1);
	
	writeElement("integer", { "name", "strictNormals", "value", "true" }, depth + 1);

	writeElementEnd("integrator", depth);
	
}

void MitsubaExporter::writeSensor(int depth)
{
	writeElementBegin("sensor", { "type", "path" }, depth);

	writeElement("float", { "name", "fov", "value", std::to_string(cameraFOV) }, depth + 1);

	//camera
	writeElementBegin("transform", { "name", "toWorld" }, depth + 1);
		writeElement("matrix", { "value", "1 0 0 0  0 1 0 0  0 0 1 0  0 0 0 1" }, depth + 2);
	writeElementEnd("transform", depth + 1);

	writeSampler(depth + 1);

	writeFilm(depth + 1);
	writeElementEnd("sensor", depth);
}

void MitsubaExporter::writeSampler(int depth)
{
	writeElementBegin("sampler", { "type", "sobol" }, depth);
		writeElement("integer", { "name", "sampleCount", "value", std::to_string(nSamples) }, depth + 1);
	writeElementEnd("sampler", depth);
}

void MitsubaExporter::writeFilm(int depth)
{
	writeElementBegin("film", { "type", "ldrfilm" }, depth);
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
	char buffer[100];
	sprintf_s(buffer, "%.6f %.6f %.6f", mat->diffuseColor.x, mat->diffuseColor.y, mat->diffuseColor.z);
	writeElementBegin("bsdf", { "type", "diffuse" }, depth);
		writeElement("rgb", { "name", "reflectance", "value", buffer }, depth + 1);
	writeElementEnd("bsdf", depth);
}

void MitsubaExporter::writeShape(ShapeNode* shapeNode, std::string filename, int depth)
{
	writeElementBegin("shape", { "type", "obj" }, depth);
		writeElement("string", { "name", "filename", "value", shapeNode->geometry->name }, depth + 1);
	writeElementEnd("shape", depth);
}

void MitsubaExporter::writeTransform(ShapeNode* shapeNode, int depth)
{
	writeElementBegin("transform", { "name", "toWorld" }, depth);
		

	writeElementEnd("shape", depth);
}