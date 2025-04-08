/**
@file materials.h
Definice tridy materialu

@author Copyright (C) 2008 Jiri Drahokoupil
*/

#ifndef __MAT_DATA_H__
#define __MAT_DATA_H__

#include "dataStructs.h"


enum class MaterialType{
	DIFFUSE,
	COATED_DIFFUSE,
	DIFFUSE_TRANSMISSIVE,
	DIELECTRIC,
	SHINY,
	SUBSTRATE,
	UBER,
	CONDUCTOR,
	CONDUCTOR_REFLECTANCE,
	COATED_CONDUCTOR,
	NONE 
};

class Mat
{
public:
	vec3 vrmlDiffuse;
	MaterialType materialType;
	std::string name;
	float sigma;
	float roughness;
	float u_roughness;
	float v_roughness;
	float index;
	vec3 Kd;
	vec3 Ks;
	vec3 Kt;
	vec3 Kr;
	vec3 reflect;
	vec3 transmit;
	int indexBRDF;

	std::string eta_str;
	std::string k_str;

public:
	Mat();
	Mat(const Mat&);
	Mat& operator= (const Mat&);

};


#endif //__MAT_DATA_H__

