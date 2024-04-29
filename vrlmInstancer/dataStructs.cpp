#include "dataStructs.h"
#include <iostream> 


std::ostream& operator<<(std::ostream& os, const vec3& obj)
{
    os << obj.x << " " << obj.y << " " << obj.z << " ";
    return os;
}


AABB::AABB() {}

AABB::AABB(vec3 a, vec3 b) {
    min.x = std::min(a.x, b.x);
    min.y = std::min(a.y, b.y);
    min.z = std::min(a.z, b.z);
    max.x = std::max(a.x, b.x);
    max.y = std::max(a.y, b.y);
    max.z = std::max(a.z, b.z);
}

AABB::AABB(vec3 a, vec3 b, vec3 c) {
    min.x = std::min(std::min(a.x, b.x), c.x);
    min.y = std::min(std::min(a.y, b.y), c.y);
    min.z = std::min(std::min(a.z, b.z), c.z);
    max.x = std::max(std::max(a.x, b.x), c.x);
    max.y = std::max(std::max(a.y, b.y), c.y);
    max.z = std::max(std::max(a.z, b.z), c.z);
}

void AABB::uniteWithPoint(vec3 point) {
    min.x = std::min(min.x, point.x);
    min.y = std::min(min.y, point.y);
    min.z = std::min(min.z, point.z);
    max.x = std::max(max.x, point.x);
    max.y = std::max(max.y, point.y);
    max.z = std::max(max.z, point.z);
}
void AABB::uniteWithAABB(AABB other) {
    min.x = std::min(min.x, other.min.x);
    min.y = std::min(min.y, other.min.y);
    min.z = std::min(min.z, other.min.z);
    max.x = std::max(max.x, other.max.x);
    max.y = std::max(max.y, other.max.y);
    max.z = std::max(max.z, other.max.z);
}

AABB Geometry::getAABB() {
    AABB ret(coords[0], coords[1]);
    for (int i = 2; i < coords.size(); i++)
    {
        ret.uniteWithPoint(coords[i]);
    }
    return ret;
}


float Geometry::triangleArea(vec3 A, vec3 B, vec3 C) {
    float a = (A - B).len(), b = (B - C).len(), c = (C - A).len();
    float s = (a + b + c) / 2;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

vec3 Geometry::getMidPoint(vec3 a, vec3 b) {
    vec3 ret = b - a;
    ret = ret / 2;
    ret += a;
    return ret;
}

vec3 Geometry::getWeightedMidPoint(vec3 a, float massA, vec3 b, float massB) {
    vec3 ret = b - a;
    float x = massA + massB;

    ret = ret * (massB / x);
    ret += a;
    return ret;
}

vec3 Geometry::getCenterOfGravity() {
    vec3 centerOfGrav;
    float massOfTheCenter = 0;
    for (size_t i = 0; i < facesPointsIndex.size(); i++)
    {
        vec3 a = coords[facesPointsIndex[i].x], b = coords[facesPointsIndex[i].y], c = coords[facesPointsIndex[i].z];
        vec3 COTriangle = getMidPoint(a, b);
        COTriangle = getWeightedMidPoint(c, 1, COTriangle, 2);
        float mass = triangleArea(a, b, c);
        centerOfGrav = getWeightedMidPoint(centerOfGrav, massOfTheCenter, COTriangle, mass);
        massOfTheCenter += mass;
    }
    return centerOfGrav;
}


Material::Material() : ambientIntensity(0), shininess(0), transparency(0)
{
    diffuseColor[0] = 0; diffuseColor[1] = 0; diffuseColor[2] = 0;
    specularColor[0] = 0; specularColor[1] = 0; specularColor[2] = 0;
}
Material::Material(float* diffuseColor, float ambientIntensity,
    float* specularColor, float shininess, float transparency)
    :ambientIntensity(ambientIntensity), shininess(shininess), transparency(transparency)
{
    std::memcpy(this->diffuseColor, diffuseColor, sizeof(float) * 3);
    std::memcpy(this->specularColor, specularColor, sizeof(float) * 3);
}

void Material::fillMaterial(float* diffuseColor, float ambientIntensity,
    float* specularColor, float shininess, float transparency)
{
    std::memcpy(this->diffuseColor, diffuseColor, sizeof(float) * 3);
    this->ambientIntensity = ambientIntensity;
    std::memcpy(this->specularColor, specularColor, sizeof(float) * 3);
    this->shininess = shininess;
    this->transparency = transparency;
}