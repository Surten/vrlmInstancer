/**
@file materials.h
Deklarace fci zpracovavajicich soubor s materialy

@author Copyright (C) 2008 Jiri Drahokoupil
*/

#include "materials.h"
#include "dataStructs.h"
#include "matData.h"
#include "scene.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


MaterialsFile::MaterialsFile()
{

}

MaterialsFile::~MaterialsFile()
{
	for (size_t i = 0; i < materials.size(); i++)
	{
		delete materials[i];
	}
}

//----------------------------------------------------------------------------------------------
bool MaterialsFile::LoadMaterials(std::string fileName)
//----------------------------------------------------------------------------------------------
{
	// Otestuju korektnost vstupu
	if (!InitMatFile(fileName))
		return false;

	// Citac radek(kvuli snazsi identifikaci chyby v .mat souboru)
	int rowCounter = 0;

	// Vytvorim vstupni proud
	std::ifstream matFile(fileName.c_str());
	std::string matName = "";
	std::string matType = "";
	vec3 vrmlDiffuse;

	// Zacnu zpracovavat .mat soubor
	while (!matFile.eof())
	{
		std::string line = "";
		std::getline(matFile, line);
		std::stringstream linestream(line);

		linestream >> matName >> vrmlDiffuse.x >> vrmlDiffuse.y >> vrmlDiffuse.z >> matType;

		matType = matType.substr(4);

		Mat* mat = new Mat();
		mat->name = matName;
		mat->vrmlDiffuse = vrmlDiffuse;

		if (matType == "coateddiffuse")
		{
			LoadMaterialCoatedDiffuse(linestream, mat);
			SaveMaterial(mat);
		}
		else if (matType == "diffuse")
		{
			LoadMaterialDiffuse(linestream, mat);
			SaveMaterial(mat);
		}
		else if (matType == "dielectric")
		{
			LoadMaterialDielectric(linestream, mat);
			SaveMaterial(mat);
		}
		else if (matType == "conductor")
		{
			LoadMaterialConductor(linestream, mat);
			SaveMaterial(mat);
		}
		else if (matType == "diffusetransmission")
		{
			LoadMaterialDiffuseTransmissive(linestream, mat);
			SaveMaterial(mat);
		}
		else if (matType == "conductorreflectance")
		{
			LoadMaterialConductorReflectance(linestream, mat);
			SaveMaterial(mat);
		}
		else if (matType == "coatedconductor")
		{
			LoadMaterialConductorReflectance(linestream, mat);
			SaveMaterial(mat);
		}
		else
		{
			std::cout << "Unsuported material: " << matType << std::endl;
		}
	}
}


Mat* MaterialsFile::retMaterial(vec3 vrmlDiffuse)
{
	for (int i = 0; i < materials.size(); i++) {
		vec3 difference = materials[i]->vrmlDiffuse - vrmlDiffuse;
		if (difference.len() < 0.00005) {
			return materials[i];
		}
	}
	return nullptr;
}


bool MaterialsFile::InitMatFile(std::string fileName)
{
	std::ifstream matFile(fileName.c_str());
	if (matFile.is_open()){	
		return true;
	}

	return false;
}

bool MaterialsFile::SaveMaterial(Mat* material)
{	
	for(int i = 0; i < materials.size(); i++){
		vec3 difference = materials[i]->vrmlDiffuse - material->vrmlDiffuse;
		if(difference.len() < 0.00005) {
			return false;
		}
	}
	materials.push_back(material);
	return true;
}



void MaterialsFile::AddSceneMaterials(Scene* scene)
{
	int numOfGeometriesWithoutTexCoords = 0;

	for (ShapeNode* shapeNode : scene->ShapeNodes)
	{
		Mat* existing_material = retMaterial(shapeNode->material->diffuseColor);
		if (existing_material == nullptr)
		{
			Mat* mat = new Mat();
			mat->name = "UnregognizedMaterial" + std::to_string(unregognizedMaterialCount);
			mat->vrmlDiffuse = shapeNode->material->diffuseColor;
			mat->Kd = shapeNode->material->diffuseColor;
			mat->materialType = MaterialType::UNDEFINED;

			shapeNode->exportMaterial = mat;
			materials.push_back(mat);

			unregognizedMaterialCount++;
		}
		else
		{
			shapeNode->exportMaterial = existing_material;
		}

		if (shapeNode->exportMaterial->hasBTF)
		{
			if (shapeNode->geometry && shapeNode->geometry->textureCoords.size() == 0)
			{
				//std::cout << "No Texture coordinates to use BTF: " << shapeNode->exportMaterial->name << " - " << scene->name << " - " << shapeNode->geometry->name << std::endl;
				numOfGeometriesWithoutTexCoords++;
			}
		}
	}
	std::cout << "Cant use BTF for " << numOfGeometriesWithoutTexCoords << " / " << scene->ShapeNodes.size() << " shape nodes, no texture coordinates... in scene: " << scene->name << std::endl;
}

void MaterialsFile::replaceByBTF(std::string matName, std::string btfFileName)
{
	for (Mat* material : materials)
	{
		if (material->name == matName)
		{
			material->hasBTF = true;
			Mat* btfMat = new Mat();
			btfMat->name = material->name + "BTF";
			btfMat->materialType = MaterialType::BTF_MATERIAL;
			btfMat->btfFileName = btfFileName;
			materials.push_back(btfMat);
		}
	}
}

void MaterialsFile::LoadMaterialDiffuse(std::stringstream& matFile, Mat * mat)
{
	std::string parameterName;
	std::string symbol;

	while (!matFile.eof()) {
		matFile >> parameterName;
		if (matFile.eof()) break;

		if (parameterName == "reflectance")
		{
			matFile >> symbol >> mat->Kd.x >> mat->Kd.y >> mat->Kd.z >> symbol;
		}
	}
	mat->materialType = MaterialType::DIFFUSE;
}

void MaterialsFile::LoadMaterialCoatedDiffuse(std::stringstream& matFile, Mat *mat)
{
	std::string parameterName;
	std::string symbol;

	while(!matFile.eof()) {
		matFile >> parameterName;
		if (matFile.eof()) break;
		if (parameterName == "roughness")
		{
			matFile >> symbol >> mat->roughness >> symbol;
		}
		else if (parameterName == "reflectance")
		{
			matFile >> symbol >> mat->Kd.x >> mat->Kd.y >> mat->Kd.z >> symbol;
		}
	}
	mat->materialType = MaterialType::COATED_DIFFUSE;
}

void MaterialsFile::LoadMaterialDielectric(std::stringstream& matFile, Mat *mat)
{
	std::string parameterName;
	std::string symbol;

	while(!matFile.eof()) {
		matFile >> parameterName;
		if (matFile.eof()) break;
		if (parameterName == "eta")
		{
			matFile >> symbol >> mat->eta_str >> symbol;
		}
	}
	mat->materialType = MaterialType::DIELECTRIC;
}

void MaterialsFile::LoadMaterialDiffuseTransmissive(std::stringstream& matFile, Mat* mat)
{
	std::string parameterName;
	std::string symbol;

	while (!matFile.eof()) {
		matFile >> parameterName;
		if (matFile.eof()) break;
		if (parameterName == "reflectance")
		{
			matFile >> symbol >> mat->reflect.x >> mat->reflect.y >> mat->reflect.z >> symbol;
		}
		else if (parameterName == "transmittance")
		{
			matFile >> symbol >> mat->transmit.x >> mat->transmit.y >> mat->transmit.z >> symbol;
		}
	}
	mat->materialType = MaterialType::DIFFUSE_TRANSMISSIVE;
}

void MaterialsFile::LoadMaterialConductor(std::stringstream& matFile, Mat* mat)
{
	std::string parameterName;
	std::string symbol;

	while (!matFile.eof()) {
		matFile >> parameterName;
		if (matFile.eof()) break;
		if (parameterName == "eta")
		{
			matFile >> symbol >> mat->eta_str >> symbol;
		}
		else if (parameterName == "k")
		{
			matFile >> symbol >> mat->k_str >> symbol;
		}
		else if (parameterName == "roughness")
		{
			matFile >> symbol >> mat->roughness >> symbol;
		}
	}
	mat->materialType = MaterialType::CONDUCTOR;
}

void MaterialsFile::LoadMaterialConductorReflectance(std::stringstream& matFile, Mat* mat)
{
	std::string parameterName;
	std::string symbol;

	while (!matFile.eof()) {
		matFile >> parameterName;
		if (matFile.eof()) break;

		else if (parameterName == "roughness")
		{
			matFile >> symbol >> mat->roughness >> symbol;
		}
		else if (parameterName == "reflectance")
		{
			matFile >> symbol >> mat->reflect.x >> mat->reflect.y >> mat->reflect.z >> symbol;
		}
	}
	mat->materialType = MaterialType::CONDUCTOR_REFLECTANCE;

}
void MaterialsFile::LoadMaterialCoatedConductor(std::stringstream& matFile, Mat* mat)
{
	std::string parameterName;
	std::string symbol;

	while (!matFile.eof()) {
		matFile >> parameterName;
		if (matFile.eof()) break;
		if (parameterName == "eta")
		{
			matFile >> symbol >> mat->eta_str >> symbol;
		}
		else if (parameterName == "k")
		{
			matFile >> symbol >> mat->k_str >> symbol;
		}
		else if (parameterName == "roughness")
		{
			matFile >> symbol >> mat->roughness >> symbol;
		}
	}
	mat->materialType = MaterialType::COATED_CONDUCTOR;
}

