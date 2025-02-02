#include "pbrtExporter.h"
#include <unordered_map>

#define RAD_TO_DEG(angle)   ((angle)*57.29577951308f)

PbrtExporter::PbrtExporter(){
}

void PbrtExporter::exportScene(std::vector<Scene*> scenes, ViewPointNode* camera, std::string headerFileName, std::string renderImageFileName)
{
	out.open(headerFileName + ".pbrt");
	writeSceneWideOptions(camera, renderImageFileName);
	out << " WorldBegin" << std::endl;
	writeTexture();
	for (auto scene : scenes)
	{
		writeAllLightSourcesOfAScene(scene);
		writeGeometry(scene);
	}
	out.close();

}

void PbrtExporter::writeSceneWideOptions(const ViewPointNode* camera, std::string renderImageFileName) {
	writeIntegrator();
	writeCamera(camera);
	writeSampler();
	writeFilter();
	writeFilm(renderImageFileName);
}


void PbrtExporter::writeCamera(const ViewPointNode* camera) {
	vec3 pos, lookAt, lookUp;
	camera->computeLookAt(pos, lookAt, lookUp);
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
	out << " PixelFilter \"michell\"" << std::endl;
	out << std::endl;
}
void PbrtExporter::writeFilm(std::string renderImageFileName) {
	out << " #Output file specification" << std::endl;
	out << " Film \"image\" \"string filename\" " << renderImageFileName << std::endl;
	out << "      \"integer xresolution\" [" << xResolution << "]" << std::endl;
	out << "      \"integer yresolution\" [" << yResolution << "]" << std::endl;
	out << std::endl;
}

void PbrtExporter::writeLightSource(LightNode* lightNode){

	out << " AttributeBegin" << std::endl;

	switch (lightNode->lightType)
	{
	case LightNode::LightType::SPOTLIGHT:
		out << " LightSource \"spot\"" << std::endl;
		out << "    \"point3 from\" [" << lightNode->location << " ]" << std::endl;
		out << "    \"point3 to\" [" << lightNode->location + lightNode->direction << " ]" << std::endl;
		out << "    \"rgb I\" [" << lightNode->color * lightNode->intensity << " ]" << std::endl;
		out << "    \"float coneangle\" [" << 60 << " ]" << std::endl;
		out << "    \"float conedeltaangle\" [" << 80 << " ]" << std::endl;
		break;
	case LightNode::LightType::GONIOLIGHT:
		out << "    Translate " << lightNode->location << std::endl;
		out << " LightSource \"goniometric\"" << std::endl;
		out << "    \"string filename\" [ \"" << lightNode->url << "\" ]" << std::endl;
		out << "    \"rgb I\" [" << lightNode->color * lightNode->intensity << " ]" << std::endl;

		break;
	}
	out << " AttributeEnd" << std::endl;
	out << std::endl;
}

void PbrtExporter::writeAllLightSourcesOfAScene(Scene* scene) {
	for (auto light : scene->lights)
	{
		writeLightSource(light);
	}
}

void PbrtExporter::writeTexture() {
	out << "AttributeBegin" << std::endl;
	out << "Texture \"checkerBoard\" \"spectrum\" \"imagemap\" \"string filename\" [ \"checkerboard-pattern.jpg\" ]" << std::endl;
	out << "AttributeEnd" << std::endl;

}
void PbrtExporter::writeGeometry(Scene* scene){
	std::string geometryFileName = scene->name;
	size_t a = geometryFileName.find_last_of("/");
	if (a != std::string::npos)
	{
		geometryFileName = geometryFileName.substr(a+1);
	}
	if (geometryFileName.find(".wrl") != std::string::npos || geometryFileName.find(".WRL") != std::string::npos)
	{
		geometryFileName = geometryFileName.substr(0, geometryFileName.length() - 4);
	}

	currentGeometryFileName = geometryFileName + ".pbrt";

	out << "Include \"" << currentGeometryFileName << "\"";

	outGeometry.open(currentGeometryFileName);

	writeObjectInstances(scene);

	writeSceneHierarchy(scene);
	outGeometry.close();
}

void PbrtExporter::writeObjectInstances(Scene* scene) {
	for (auto geometry : scene->geometries)
	{
		
		outGeometry << " ObjectBegin \"" << geometry->name << "_" << currentGeometryFileName << "\"" << std::endl;

		bool hasTextureCoords = geometry->textureCoords.size() > 0;
		// Pridruzim odpovidajici material
		// Vytvorim ukazatel na geometrii uzlu
		if (hasTextureCoords) {
			writeMaterialWithTexture(&geometry->parent->material);
			writeTriangleMeshWithTexture(geometry);
		}
		else {
			writeMaterial(&geometry->parent->material);
			writeTriangleMesh(geometry);
		}
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
	outGeometry << " AttributeBegin" << std::endl;

	if (node->hasTranslation())
	{
		outGeometry << "   Translate " << node->translation << std::endl;
	}
	if (node->hasRotation()) 
	{
		vec3 axis(node->rotation[0], node->rotation[1], node->rotation[2]);
		outGeometry << "   Rotate " << RAD_TO_DEG(node->rotation[3]) << " " << axis << std::endl;
	}
	if (node->hasScaleOrientation()) {

		vec3 axis(node->scaleOrientation[0], node->scaleOrientation[1], node->scaleOrientation[2]);
		outGeometry << "   Rotate " << RAD_TO_DEG(node->scaleOrientation[3]) << " " << axis << std::endl;
		outGeometry << "   Scale " << node->scale << std::endl;
		outGeometry << "   Rotate ";
		float angle = RAD_TO_DEG(node->scaleOrientation[3]);
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
			writeShapeNode(static_cast<ShapeNode*>(child));
		}
	}
}
void PbrtExporter::writeShapeNode(ShapeNode* node)
{
	outGeometry << "    ObjectInstance \"" << node->geometry->name << "_" << currentGeometryFileName << "\"" << std::endl;
}

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

	if (geometry->coords.size() != geometry->textureCoords.size())
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

