/**
@file matrix.h
Hlavickovy soubor tridy matice.

@author Copyright (C) 2008 Jiri Drahokoupil
*/

#pragma once

#include <stack>

/// Format transformacni matice 4x4
/**
 Indexace stejna jako v PBRT(tzn.po sloupcich)
 @verbatim
     | m00 m10 m20 m30 |
 M = | m01 m11 m21 m31 |
     | m02 m12 m22 m32 |
     | m03 m13 m23 m33 |
 @endverbatim
*/;

class Matrix
{
private:
	/// Samotne polozky matice reprezentovane polem, 1.index = sloupec, 2.index = radek
	float matrix[4][4];
public:
	/// Bezparametricky konstruktor(nastavi matici na jednotkovou)
	Matrix();
	/// Konstruktor matice pomoci pole floatu
	/**
	 @param mat Pole 16-ti float hodnot
	*/
	Matrix(float* mat);
	/// Kopirujici konstruktor
	Matrix(const Matrix&);

	/// Nastavi prvky matice pomoci 16ti float hodnot
	void setByFloats(float m00,float m01,float m02,float m03,float m10,float m11,float m12,float m13,
		float m20,float m21,float m22,float m23,float m30,float m31,float m32,float m33);

	/// Nastavi matici na jednotkovou
	void Identity();

	void Transpose();
	std::string GetAsString();

	/// Operator prirazeni
	Matrix& operator=(const Matrix&);

	bool operator==(const Matrix&);

	/// Operator pristupu k prvkum matice
	/**
	 @param idx1 Index do sloupce
	 @param idx2 Index do radku
	 @return Vraci float hodnotu na specifikovanem indexu
	*/
	const float& operator()(int idx1,int idx2) const;
	float& operator()(int idx1,int idx2);

	/// Metoda sestavi matici posunuti pro dany vektor specifikujici posunuti
	/**
	@verbatim
	     |  1  0  0  t.x |
	 T = |  0  1  0  t.y |
	     |  0  0  1  t.z |
	     |  0  0  0   1  |
	@endverbatim
	@param vect3D Parametr specifikujici vektor posunuti
	@return Vrati vytvorenou matici posunu
	*/
	Matrix& mTranslate(const vec3& vect3D);

	/// Metoda sestavi matici rotace kolem osy X o dany uhel
	/**
	@verbatim
	     |  1   0       0      0 |
	Rx = |  0 cos(a)  -sin(a)  0 |
	     |  0 sin(a)   cos(a)  0 |
	     |  0   0       0      1 |
	@endverbatim
	@param angle Parametr urcujici uhel rotace
	@return Vrati matici rotace kolem osy X
	*/
	Matrix& mRotateX(float angle);

	/// Metoda sestavi matici rotace kolem osy Y o dany uhel
	/**
	@verbatim
	     | cos(a)  0   sin(a)  0 |
	Ry = |  0      1    0      0 |
	     | -sin(a) 0   cos(a)  0 |
	     |  0      0    0      1 |
	@endverbatim
	@param angle Parametr urcujici uhel rotace kolem osy Y
	@return Vrati matici rotace kolem osy Y
	*/
	Matrix& mRotateY(float angle);

	/// Metoda sestavi matici rotace kolem osy Z o dany uhel
	/**
	@verbatim
	     | cos(a)  -sin(a)  0  0 |
	Rz = | sin(a)   cos(a)  0  0 |
	     |  0         0     1  0 |
	     |  0         0     0  1 |
	@endverbatim
	@param angle Uhel urcujici rotaci kolem osy Z
	@return Vrati matici rotace kolem osy Z
	*/
	Matrix& mRotateZ(float angle);

	/// Metoda vrati rotacni matici pro vektor specifikujici rotaci(vypocet dle PBRT)
	/**
	 @param vec3D 3D Vektor specifikujici vektor rotace 
	 @param angle Uhel, o ktery rotujeme kolem daneho vektoru rotace
	 @return Vysledna matice
	*/
	Matrix& mRotate(const vec3& vec3D, const float angle);

	/// Metoda vrati matici zmeny meritka pro zadany vektor
	/**
	 @verbatim
	     | s.x  0   0  0 |
	 S = |  0  s.y  0  0 |
	     |  0   0  s.z 0 |
	     |  0   0   0  1 |
	 @endverbatim
	 @param vec3D Vektor obsahujici 3 hodnoty zmeny meritka v jednotlivych osach
	 @return Vysledna matice
	*/
	Matrix& mScale(const vec3& vec3D);

	void applyTransforms(std::stack<Transform> transforms);

	/// Metoda vraci inverzni matici 
	Matrix& mInverse();

	/// Nasobeni matic M = m1*m2
	/**
	 @param m1 Matice leveho cinitele
	 @param m2 Matice praveho cinitele
	 @return Soucin matic
	*/
	friend Matrix operator*(const Matrix& m1,const Matrix& m2);

	/// Transformace vektoru pomoci tranformacni matice
	/**
	 @param mat Transformacni matice
	 @param vec Vektor podrobeny transformaci
	 @return Transformovany vektor
	*/
	friend vec3 operator*(const Matrix& mat,const vec3& vec);

};


