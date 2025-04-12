/**
@file materials.h
Metody tridy materialu

@author Copyright (C) 2008 Jiri Drahokoupil
*/

#include "matData.h"


//----------------------------------------------------------------------------------------------
Mat::Mat() 
//----------------------------------------------------------------------------------------------
{
	// Nastavim defaultni hodnoty
	vrmlDiffuse = vec3(0,0,0);
	materialType = MaterialType::UNDEFINED;
	sigma = 0.0f;
	roughness = 0.1f;
	u_roughness = 0.1f;
	v_roughness = 0.1f;
	index = 1.5f;
	Kd = vec3(1.0,1.0,1.0);
	Ks = vec3(1.0,1.0,1.0);
	Kt = vec3(1.0,1.0,1.0);
	Kr = vec3(1.0,1.0,1.0);
	reflect = vec3(0.5,0.5,0.5);
	transmit = vec3(0.5,0.5,0.5);
	indexBRDF = 0;
}


//----------------------------------------------------------------------------------------------
Mat::Mat(const Mat& m)
//----------------------------------------------------------------------------------------------
{
	vrmlDiffuse = m.vrmlDiffuse;
	materialType = m.materialType;
	sigma = m.sigma;
	roughness = m.roughness;
	u_roughness = m.u_roughness;
	v_roughness = m.v_roughness;
	index = m.index;
	Kd = m.Kd;
	Ks = m.Ks;
	Kt = m.Kt;
	Kr = m.Kr;
	reflect = m.reflect;
	transmit = m.transmit;
	indexBRDF = 0;
}


//----------------------------------------------------------------------------------------------
Mat& Mat::operator=(const Mat& m)
//----------------------------------------------------------------------------------------------
{
	if(this==&m)
		return *this;
	vrmlDiffuse = m.vrmlDiffuse;
	materialType = m.materialType;
	sigma = m.sigma;
	roughness = m.roughness;
	u_roughness = m.u_roughness;
	v_roughness = m.v_roughness;
	index = m.index;
	Kd = m.Kd;
	Ks = m.Ks;
	Kt = m.Kt;
	Kr = m.Kr;
	reflect = m.reflect;
	transmit = m.transmit;
	indexBRDF = m.indexBRDF;
	return *this;
}


