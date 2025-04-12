/**
@file materials.h
Definice fci zpracovavajicich soubor s materialy

@author Copyright (C) 2008 Jiri Drahokoupil
*/


#ifndef __MATERIALS_H__
#define __MATERIALS_H__

#include <string>
#include "matData.h"
#include "scene.h"

class MaterialsFile
{
public:
	MaterialsFile();
	~MaterialsFile();

	bool LoadMaterials(std::string filename);
	Mat* retMaterial(vec3 vrmlDiffuse);

	void AddSceneMaterials(Scene* scene);

	void replaceByBTF(std::string matName, std::string btfFileName);


public:
	std::vector<Mat*> materials;

	std::vector<vec3> unrecognizedMaterials;


private:
	bool SaveMaterial(Mat* material);
	bool InitMatFile(std::string fileName);

	void LoadMaterialDiffuse(std::stringstream& matFile, Mat* mat);
	void LoadMaterialCoatedDiffuse(std::stringstream& matFile, Mat* mat);
	void LoadMaterialDielectric(std::stringstream& matFile, Mat* mat);
	void LoadMaterialDiffuseTransmissive(std::stringstream& matFile, Mat* mat);
	void LoadMaterialConductor(std::stringstream& matFile, Mat* mat);
	void LoadMaterialConductorReflectance(std::stringstream& matFile, Mat* mat);
	void LoadMaterialCoatedConductor(std::stringstream& matFile, Mat* mat);

private:
	int unregognizedMaterialCount = 0;

};



#endif //__MATERIALS_H__

