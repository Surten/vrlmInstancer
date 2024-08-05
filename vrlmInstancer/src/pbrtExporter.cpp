#include "pbrtExporter.h"


PbrtExporter::PbrtExporter() : scene(nullptr){

}

void PbrtExporter::exportScene(Scene* scene)
{
	this->scene = scene;
	out.open(scene->name + ".pbrt");
}

void PbrtExporter::writeSceneWideOptions() {

}

void PbrtExporter::writeCamera(const ViewPointNode & camera) {
	out << "LookAt " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
	//out << "       " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
	//out << "       " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
	
}

void PbrtExporter::writeSampler(){
	out << "Sampler \"halton\" \"integer pixelsamples\" 128" << std::endl;
}

void PbrtExporter::writeIntegrator() {
	out << "Integrator \"volpath\"" << std::endl;
}

void PbrtExporter::writeFilm() {
	int xRes = 400, yRes = 400;
	out << " #Output file specification" << std::endl;
	out << " Film \"image\" \"string filename\" " << "pbrtFileOut.pbrt" << std::endl;
	out << "      \"integer xresolution\" [" << xRes << "]" << std::endl;
	out << "      \"integer yresolution\" [" << yRes << "]" << std::endl;
	out << std::endl;
}

void PbrtExporter::writeLightSource(const LightNode& camera){

}

void PbrtExporter::writeAtribute(){

}

//----------------------------------------------------------------------------------------------
void PbrtExporter::computeLookAt(vec3& loc, vec3& dir, vec3& up)
//----------------------------------------------------------------------------------------------
{
	//loc = this->retCurrentPosition();

	//// Get the current orientation of camera
	//float currentOrient = this->retCurrentOrientation();

	//vec3 vec(currentOrient.x, currentOrient.y, currentOrient.z);

	//// Normalizuju
	//vec = vec.normalize();

	//// Matice identity
	//Matrix mat;
	//// Vytvorim rotacni matici kolem obecneho vektoru definovaneho pomoci 'dir' a uhlu v 'orientation'
	//mat = mat.mRotate(vec, currentOrient.par);

	//// Urcim smer pohledu kamery
	//vec = mat * vec3(0.0, 0.0, -1.0);
	//// Urcim up-vector kamery
	//up = mat * vec3(0.0, 1.0, 0.0);

	//// Zbyva urcit bod, na ktery bude kamera centrovana. Pokud znam smerovy vektor
	//// pohledu kamery, staci vzit libovolne cislo a dosadit za parametr 't' v parametricke
	//// rci primky.
	//const float t = 10.0f;

	//// Get the current position of the camera
	//vec3 currentPosition = this->retCurrentPosition();
	//dir.x = currentPosition.x + vec.x * t;
	//dir.y = currentPosition.y + vec.y * t;
	//dir.z = currentPosition.z + vec.z * t;
}

