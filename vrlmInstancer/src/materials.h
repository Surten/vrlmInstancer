/**
@file materials.h
Definice fci zpracovavajicich soubor s materialy

@author Copyright (C) 2008 Jiri Drahokoupil
*/


#ifndef __MATERIALS_H__
#define __MATERIALS_H__

#include "dataStructs.h"
#include "matData.h"
#include "scene.h"
#include <string>
#include <sstream>


class MaterialsFile
{
public:
	MaterialsFile();
	~MaterialsFile();

	/// <summary>
	/// Loads materials with a predefined format to be used during export to PBRT / Mitsuba
	/// </summary>
	bool LoadMaterials(std::string filename);

	/// <summary>
	/// Returns the material with the found by the vrmlDiffuse color
	/// </summary>
	Mat* retMaterial(vec3 vrmlDiffuse);

	/// <summary>
	/// Traverses the scene objects and appends any materials, that are not already in the materials list, as UndefinedMaterialXX
	/// </summary>
	void AddSceneMaterials(Scene* scene);

	/// <summary>
	/// Adds a BTF counterpart of a material to the list, and sets the original material bool hasBTF to true
	/// </summary>
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

