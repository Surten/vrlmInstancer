/**
@file matrix.cpp
Metody tridy matice.

@author Copyright (C) 2008 Jiri Drahokoupil
*/

#include <iostream>
#include <string>
#include "dataStructs.h"
#include "matrix.h"



using namespace std;


//----------------------------------------------------------------------------------------------
Matrix::Matrix()
//----------------------------------------------------------------------------------------------
{
	Identity();
}


//----------------------------------------------------------------------------------------------
Matrix::Matrix(float* numb)
//----------------------------------------------------------------------------------------------
{
	// Pro vyssi prehlednost radeji rucne(stejne jako v PBRT)
	// Prvni sloupec		Druhy sloupec			Treti sloupec			 Ctvrty sloupec
	matrix[0][0] = numb[0]; matrix[1][0] = numb[4]; matrix[2][0] = numb[8];  matrix[3][0] = numb[12];
	matrix[0][1] = numb[1]; matrix[1][1] = numb[5]; matrix[2][1] = numb[9];  matrix[3][1] = numb[13];
	matrix[0][2] = numb[2]; matrix[1][2] = numb[6]; matrix[2][2] = numb[10]; matrix[3][2] = numb[14];
	matrix[0][3] = numb[3]; matrix[1][3] = numb[7]; matrix[2][3] = numb[11]; matrix[3][3] = numb[15];
}


//----------------------------------------------------------------------------------------------
Matrix::Matrix(const Matrix& m)
//----------------------------------------------------------------------------------------------
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			matrix[i][j] = m(i,j);
}


//----------------------------------------------------------------------------------------------
void Matrix::setByFloats(float m00,float m01,float m02,float m03,float m10,float m11,float m12,float m13,
						float m20,float m21,float m22,float m23,float m30,float m31,float m32,float m33)
//----------------------------------------------------------------------------------------------
{
	matrix[0][0] = m00;
	matrix[0][1] = m01;
	matrix[0][2] = m02;
	matrix[0][3] = m03;
	
	matrix[1][0] = m10;
	matrix[1][1] = m11;
	matrix[1][2] = m12;
	matrix[1][3] = m13;

	matrix[2][0] = m20;
	matrix[2][1] = m21;
	matrix[2][2] = m22;
	matrix[2][3] = m23;

	matrix[3][0] = m30;
	matrix[3][1] = m31;
	matrix[3][2] = m32;
	matrix[3][3] = m33;
}


//----------------------------------------------------------------------------------------------
void Matrix::Identity()
//----------------------------------------------------------------------------------------------
{
	// Nejprve matici na vsech indexech vynuluju
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			matrix[i][j] = 0;
	// Nastavim jednicky na diagonale
	matrix[0][0] = 1;
	matrix[1][1] = 1;
	matrix[2][2] = 1;
	matrix[3][3] = 1;
}

//----------------------------------------------------------------------------------------------
void Matrix::Transpose()
//----------------------------------------------------------------------------------------------
{
	std::swap(matrix[0][1], matrix[1][0]);
	std::swap(matrix[0][2], matrix[2][0]);
	std::swap(matrix[0][3], matrix[3][0]);
	std::swap(matrix[1][2], matrix[2][1]);
	std::swap(matrix[1][3], matrix[3][1]);
	std::swap(matrix[2][3], matrix[3][2]);
}

//----------------------------------------------------------------------------------------------
std::string Matrix::GetAsString()
//----------------------------------------------------------------------------------------------
{
	std::string ret = "";
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			ret += std::to_string(matrix[i][j]) + " ";
		}
	}
	return ret;
}


//----------------------------------------------------------------------------------------------
Matrix& Matrix::operator=(const Matrix& m)
//----------------------------------------------------------------------------------------------
{
	if(this==&m) 
		return *this;
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			matrix[i][j] = m(i,j);
	return *this;
}


//----------------------------------------------------------------------------------------------
const float& Matrix::operator()(int idx1,int idx2) const
//----------------------------------------------------------------------------------------------
{
	if(idx1<0||idx2<0||idx1>3||idx2>3){
		std::cout << "Error, wrong matrix index" << std::endl;
	}
	return matrix[idx1][idx2];
}


//----------------------------------------------------------------------------------------------
float& Matrix::operator()(int idx1,int idx2)
//----------------------------------------------------------------------------------------------
{ 
	if(idx1<0||idx2<0||idx1>3||idx2>3){
		std::cout << "Error, wrong matrix index" << std::endl;
	}
	return matrix[idx1][idx2]; 
}


//----------------------------------------------------------------------------------------------
Matrix& Matrix::mTranslate(const vec3& vect)
//----------------------------------------------------------------------------------------------
{
	// Nastavim matici pro zacatek jako jednotkovou
	Identity();
	// Prepisu hodnoty na vyznamnych indexech
	matrix[3][0] = vect.x;
	matrix[3][1] = vect.y;
	matrix[3][2] = vect.z;
	return *this;
}


//----------------------------------------------------------------------------------------------
Matrix& Matrix::mRotateX(float angle)
//----------------------------------------------------------------------------------------------
{
	// Nastavim matici pro zacatek jako jednotkovou
	Identity();

	// Prepisu hodnoty na vyznamnych indexech
	matrix[1][1] = cosf(angle);
	matrix[1][2] = sinf(angle);
	matrix[2][1] = -sinf(angle);
	matrix[2][2] = cosf(angle);
	return *this;
}


//----------------------------------------------------------------------------------------------
Matrix& Matrix::mRotateY(float angle)
//----------------------------------------------------------------------------------------------
{
	// Pro zacatek nastavim matici na jednotkovou
	Identity();
	// Prepisu nektere z hodnot
	matrix[0][0] = cosf(angle);
	matrix[0][2] = -sinf(angle); 
 	matrix[2][0] = sinf(angle);
	matrix[2][2] = cosf(angle);
	return *this;
}


//----------------------------------------------------------------------------------------------
Matrix& Matrix::mRotateZ(float angle)
//----------------------------------------------------------------------------------------------
{
	// Pro zacatek nastavim matici na jednotkovou
	Identity();
	// Prepisu nektere z hodnot
	matrix[0][0] = cosf(angle);
	matrix[0][1] = sinf(angle);
	matrix[1][0] = -sinf(angle);
	matrix[1][1] = cosf(angle);
	return *this;
}


//----------------------------------------------------------------------------------------------
Matrix& Matrix::mRotate(const vec3& vec3D, const float angle)
//----------------------------------------------------------------------------------------------
{
	// Normalizuju vektor
	float length = sqrtf(vec3D.x*vec3D.x+vec3D.y*vec3D.y+vec3D.z*vec3D.z);
	vec3 v(vec3D.x/length, vec3D.y/length, vec3D.z/length);

	// Spoctu koeficienty
	float _sin = sinf(angle);
	float _cos = cosf(angle);
	
	matrix[0][0] = v.x * v.x * (1.f - _cos) + _cos;
	matrix[0][1] = v.x * v.y * (1.f - _cos) + v.z * _sin;
	matrix[0][2] = v.x * v.z * (1.f - _cos) - v.y * _sin;
	matrix[0][3] = 0;

	matrix[1][0] = v.x * v.y * (1.f - _cos) - v.z * _sin;
	matrix[1][1] = v.y * v.y * (1.f - _cos) + _cos;
	matrix[1][2] = v.y * v.z * (1.f - _cos) + v.x * _sin;
	matrix[1][3] = 0;

	matrix[2][0] = v.x * v.z * (1.f - _cos) + v.y *_sin;
	matrix[2][1] = v.y * v.z * (1.f - _cos) - v.x * _sin;
	matrix[2][2] = v.z * v.z * (1.f - _cos) + _cos;
	matrix[2][3] = 0;

	matrix[3][0] = 0;
	matrix[3][1] = 0;
	matrix[3][2] = 0;
	matrix[3][3] = 1;

	return *this;
}


//----------------------------------------------------------------------------------------------
Matrix& Matrix::mScale(const vec3& vec3D)
//----------------------------------------------------------------------------------------------
{
	// Na diagonale nastavim koeficienty zmenseni
	matrix[0][0] = vec3D.x;
	matrix[1][1] = vec3D.y;
	matrix[2][2] = vec3D.z;
	matrix[3][3] = 1;
	
	return *this;
}


//----------------------------------------------------------------------------------------------
Matrix& Matrix::mInverse()
//----------------------------------------------------------------------------------------------
{
	float inv[4][4];

	inv[0][0] =  matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1];
	inv[0][1] = -matrix[0][1] * matrix[2][2] + matrix[0][2] * matrix[2][1];
	inv[0][2] =  matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1];

	float det = inv[0][0] * matrix[0][0] + inv[0][1] * matrix[1][0] + inv[0][2] * matrix[2][0];	

	inv[1][0] = -matrix[1][0] * matrix[2][2] + matrix[1][2] * matrix[2][0];
	inv[1][1] =  matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0];
	inv[1][2] = -matrix[0][0] * matrix[1][2] + matrix[0][2] * matrix[1][0];

	inv[2][0] =  matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0];
	inv[2][1] = -matrix[0][0] * matrix[2][1] + matrix[0][1] * matrix[2][0];
	inv[2][2] =  matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

	// Podelim prvky matice determinantem
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			inv[i][j] = inv[i][j] / det;

	inv[3][0] = -matrix[3][0] * inv[0][0] - matrix[3][1] * inv[1][0] - matrix[3][2] * inv[2][0];
	inv[3][1] = -matrix[3][0] * inv[0][1] - matrix[3][1] * inv[1][1] - matrix[3][2] * inv[2][1];
	inv[3][2] = -matrix[3][0] * inv[0][2] - matrix[3][1] * inv[1][2] - matrix[3][2] * inv[2][2];

	inv[0][3] = .0f;
	inv[1][3] = .0f;
	inv[2][3] = .0f;
	inv[3][3] = 1.0f;
	
	this->setByFloats(inv[0][0],inv[0][1],inv[0][2],inv[0][3],inv[1][0],inv[1][1],inv[1][2],inv[1][3],
		inv[2][0],inv[2][1],inv[2][2],inv[2][3],inv[3][0],inv[3][1],inv[3][2],inv[3][3]);

	return *this;
}


//----------------------------------------------------------------------------------------------
Matrix operator*(const Matrix& m1,const Matrix& m2)
//----------------------------------------------------------------------------------------------
{
	// Vysledna matice(pro nazornost radeji nasobeni rozepsano)
	Matrix ret;

	// Prvni radek vysledne matice
	ret(0,0) = m1(0,0)*m2(0,0)+m1(1,0)*m2(0,1)+m1(2,0)*m2(0,2)+m1(3,0)*m2(0,3);
	ret(1,0) = m1(0,0)*m2(1,0)+m1(1,0)*m2(1,1)+m1(2,0)*m2(1,2)+m1(3,0)*m2(1,3);
	ret(2,0) = m1(0,0)*m2(2,0)+m1(1,0)*m2(2,1)+m1(2,0)*m2(2,2)+m1(3,0)*m2(2,3);
	ret(3,0) = m1(0,0)*m2(3,0)+m1(1,0)*m2(3,1)+m1(2,0)*m2(3,2)+m1(3,0)*m2(3,3);

	// Druhy radek vysledne matice
	ret(0,1) = m1(0,1)*m2(0,0)+m1(1,1)*m2(0,1)+m1(2,1)*m2(0,2)+m1(3,1)*m2(0,3);
	ret(1,1) = m1(0,1)*m2(1,0)+m1(1,1)*m2(1,1)+m1(2,1)*m2(1,2)+m1(3,1)*m2(1,3);
	ret(2,1) = m1(0,1)*m2(2,0)+m1(1,1)*m2(2,1)+m1(2,1)*m2(2,2)+m1(3,1)*m2(2,3);
	ret(3,1) = m1(0,1)*m2(3,0)+m1(1,1)*m2(3,1)+m1(2,1)*m2(3,2)+m1(3,1)*m2(3,3);

	// Treti radek vysledne matice
	ret(0,2) = m1(0,2)*m2(0,0)+m1(1,2)*m2(0,1)+m1(2,2)*m2(0,2)+m1(3,2)*m2(0,3);
	ret(1,2) = m1(0,2)*m2(1,0)+m1(1,2)*m2(1,1)+m1(2,2)*m2(1,2)+m1(3,2)*m2(1,3);
	ret(2,2) = m1(0,2)*m2(2,0)+m1(1,2)*m2(2,1)+m1(2,2)*m2(2,2)+m1(3,2)*m2(2,3);
	ret(3,2) = m1(0,2)*m2(3,0)+m1(1,2)*m2(3,1)+m1(2,2)*m2(3,2)+m1(3,2)*m2(3,3);

	// Ctvrty radek vysledne matice
	ret(0,3) = m1(0,3)*m2(0,0)+m1(1,3)*m2(0,1)+m1(2,3)*m2(0,2)+m1(3,3)*m2(0,3);
	ret(1,3) = m1(0,3)*m2(1,0)+m1(1,3)*m2(1,1)+m1(2,3)*m2(1,2)+m1(3,3)*m2(1,3);
	ret(2,3) = m1(0,3)*m2(2,0)+m1(1,3)*m2(2,1)+m1(2,3)*m2(2,2)+m1(3,3)*m2(2,3);
	ret(3,3) = m1(0,3)*m2(3,0)+m1(1,3)*m2(3,1)+m1(2,3)*m2(3,2)+m1(3,3)*m2(3,3);

	return ret;
}



//----------------------------------------------------------------------------------------------
vec3 operator*(const Matrix& mat,const vec3& vec)
//----------------------------------------------------------------------------------------------
{
	float vX = mat(0,0)*vec.x + mat(1,0)*vec.y + mat(2,0)*vec.z + mat(3 ,0) * 1.f;
	float vY = mat(0,1)*vec.x + mat(1,1)*vec.y + mat(2,1)*vec.z + mat(3, 1) * 1.f;
	float vZ = mat(0,2)*vec.x + mat(1,2)*vec.y + mat(2,2)*vec.z + mat(3, 2) * 1.f;
	float den = mat(0,3) + mat(1,3) + mat(2,3) + mat(3,3);
	if(den!=1.f){
		vX/=den;
		vY/=den;
		vZ/=den;
	}
	return vec3(vX,vY,vZ);
}

