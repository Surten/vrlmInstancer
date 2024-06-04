#include "pbrtExporter.h"


PbrtExporter::PbrtExporter() : scene(nullptr){

}

void PbrtExporter::exportScene(Scene* scene)
{
	this->scene = scene;

}
