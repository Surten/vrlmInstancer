#pragma once
#include <string>
#include <vector>


class vec3 {
public:
    float x, y, z;
public:
    vec3() {}
    vec3(float x, float y, float z): x(x), y(y), z(z) {}
};
class vec2 {
public:
    float x, y;
public:
    vec2() {}
    vec2(float x, float y) : x(x), y(y) {}
};
class vec3i {
public:
    int x, y, z;
public:
    vec3i() {}
    vec3i(int x, int y, int z) : x(x), y(y), z(z) {}
};


class Geometry {
public:
    std::vector<vec3> coords;
    std::vector<vec3> normals;
    std::vector<vec2> textureCoords;
    std::vector<vec3i> facesNormalIndex;
    std::vector<vec3i> facesPointsIndex;
    std::vector<vec3i> facesTextureIndex;

    std::string name;
    bool ccw;
    bool solid;
    bool normalPerVertex;
    float creaseAngle;
    Geometry() :
        ccw(true), solid(true), normalPerVertex(true), creaseAngle(0.0f)
    {
        
    }

};

class Material {
public:
    Material() : ambientIntensity(0), shininess(0), transparency(0)
    {
        diffuseColor[0] = 0; diffuseColor[1] = 0; diffuseColor[2] = 0;
        specularColor[0] = 0; specularColor[1] = 0; specularColor[2] = 0;
    }
    Material(float* diffuseColor, float ambientIntensity,
        float* specularColor, float shininess, float transparency) 
        :ambientIntensity(ambientIntensity), shininess(shininess), transparency(transparency)
    {
        std::memcpy(this->diffuseColor, diffuseColor, sizeof(float)* 3);
        std::memcpy(this->specularColor, specularColor, sizeof(float)* 3);
    }

    void fillMaterial(float* diffuseColor, float ambientIntensity,
        float* specularColor, float shininess, float transparency) 
    {
        std::memcpy(this->diffuseColor, diffuseColor, sizeof(float) * 3);
        this->ambientIntensity = ambientIntensity;
        std::memcpy(this->specularColor, specularColor, sizeof(float) * 3);
        this->shininess = shininess;
        this->transparency = transparency;
    }


public:
    float diffuseColor[3];
    float ambientIntensity;
    float specularColor[3];
    float shininess;
    float transparency;
};
