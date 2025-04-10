/**
@file materials.h
Deklarace fci zpracovavajicich soubor s materialy

@author Copyright (C) 2008 Jiri Drahokoupil
*/

#include "materials.h"
#include "dataStructs.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


MaterialsFile::MaterialsFile()
{

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
	float fdX = vrmlDiffuse.x;
	float fdY = vrmlDiffuse.y;
	float fdZ = vrmlDiffuse.z;

	int mtcount = materials.size();
	//std::cout << "Matcount = " << mtcount << endl;
	for (int i = 0; i < (int)mtcount; i++) {
		Mat* mat = materials[i];
		float ndX = mat->vrmlDiffuse.x;
		float ndY = mat->vrmlDiffuse.y;
		float ndZ = mat->vrmlDiffuse.z;
		// Povolim nepatrnou toleranci, abych dosahl rovnosti materialu
		if ((fdX >= ndX - 0.00005 && fdX <= ndX + 0.00005)
			&& (fdY >= ndY - 0.00005 && fdY <= ndY + 0.00005)
			&& (fdZ >= ndZ - 0.00005 && fdZ <= ndZ + 0.00005))
			return mat;
	}
	if (unrecognizedMaterials.size() == 0)
		unrecognizedMaterials.push_back(vrmlDiffuse);
	else {
		bool matInTable = false;
		for (int i = 0; i < unrecognizedMaterials.size(); i++) {
			if (unrecognizedMaterials[i].x == fdX && unrecognizedMaterials[i].y == fdY && unrecognizedMaterials[i].z == fdZ)
				matInTable = true;
		}
		if (!matInTable)
			unrecognizedMaterials.push_back(vrmlDiffuse);
	}

	return nullptr;
}

void MaterialsFile::LoadUseAsBTFMaterials(std::string fileName)
{

}





float MaterialsFile::round (float x, int desetCisel)
{
	const float number = pow(10.0f, desetCisel);
	return (x < 0.0 ? (float)ceil(x * number - 0.5) : (float)floor(x * number + 0.5)) / number;
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
	const vec3 dif = material->vrmlDiffuse;
	for(int i = 0; i < materials.size(); i++){
		const vec3 difOld = materials[i]->vrmlDiffuse;
		// Z duvodu odladeni nepresnosti takto vypisu..
		float dxOld = round(difOld.x,5);
		float dyOld = round(difOld.y,5);
		float dzOld = round(difOld.z,5);
		float dxNew = round(dif.x,5);
		float dyNew = round(dif.y,5);
		float dzNew = round(dif.z,5);
		// Pokud je vrml difusni slozka stejna(nebo velmi podobna->pocitam s drobnou toleranci), jedna se o duplicitu!
		if((dxOld >= dxNew-0.00005 && dxOld <= dxNew+0.00005) 
			&& (dyOld >= dyNew-0.0005 && dyOld <= dyNew+0.00005) 
			&& (dzOld >= dzNew-0.0005 && dzOld <= dzNew+0.00005)) {
				//cerr <<"Warning:"<< dxNew <<" "<< dyNew <<" "<< dzNew <<" diffuse color is near early defined values:"<<dxOld<<" "<<dyOld<<" "<<dzOld<<endl;
			return false;
		}
		if(dxOld == dxNew && dyOld == dyNew && dzOld == dzNew){
			char * err = new char[100];
			printf_s(err, "%.4f %.4f %.4f", dxNew, dyNew, dzNew);
			return false;
		}
	}
	vec3 difN(dif.x,dif.y,dif.z);
	material->vrmlDiffuse = difN;
	materials.push_back(material);
	
	return true;
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

