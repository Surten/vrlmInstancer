#include "dataStructs.h"
#include <iostream> 
#include <iomanip>


std::ostream& operator<<(std::ostream& os, const vec3& obj)
{
    os << std::setprecision(6) << obj.x << " " << obj.y << " " << obj.z << " ";
    return os;
}

std::ostream& operator<<(std::ostream& os, const vec3i& obj)
{
    os << obj.x << " " << obj.y << " " << obj.z << " ";
    return os;
}

std::ostream& operator<<(std::ostream& os, const vec2& obj)
{
    os << std::setprecision(6) << obj.x << " " << obj.y << " ";
    return os;
}


AABB::AABB(): min(), max() {}

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

vec3 AABB::getDiagonal() {
    return max - min;
}
vec3 AABB::getArithmeticCenter() {
    return ((max - min) / 2.0f) + min;
}


void Geometry::calculateAABB() {
    aabb.min = coords[0];
    aabb.max = coords[1];
    for (int i = 2; i < coords.size(); i++)
    {
        aabb.uniteWithPoint(coords[i]);
    }
}

AABB Geometry::getAABB() {
    if (aabb.min.areEqual(vec3()) && aabb.max.areEqual(vec3()))
        calculateAABB();
    return aabb;
}


float Geometry::triangleArea(vec3 A, vec3 B, vec3 C) {
    float a = (A - B).len(), b = (B - C).len(), c = (C - A).len();
    float s = (a + b + c) * 0.5f;
    float temp = std::abs(s * (s - a) * (s - b) * (s - c));
    return std::sqrtf(temp);;
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
    if (massB <= 0.000001) return ret;
    ret = ret * (massB / x);
    ret += a;
    return ret;
}

vec3 Geometry::getCenterOfGravity() {

    if (centerOfGravity.x != INFINITY)
    {
        return centerOfGravity;
    }

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
    centerOfGravity = centerOfGrav;
    return centerOfGravity;
}


float Geometry::calculateTextureScale(vec3 sceneScale)
{
    if (textureCoords.size() == 0) return -1;

    double averageScale = 0.0;

    for (int i = 0; i < facesTextureIndex.size(); i++)
    {
        vec3i triangleCoordIndices = facesPointsIndex[i];
        vec3i triangleTextureCoordIndices = facesTextureIndex[i];

        double realWorldLength = ((coords[triangleCoordIndices.x] - coords[triangleCoordIndices.y]) * sceneScale).len();
        double textureLength = (textureCoords[triangleTextureCoordIndices.x] - textureCoords[triangleTextureCoordIndices.y]).len();

        if (textureLength < 0.0000001) continue;

        averageScale += realWorldLength / textureLength;
    }

    averageScale = averageScale / (double)facesTextureIndex.size();

    return (float)averageScale;
}

void Geometry::scaleTextureCoords(float desiredTextureScale, vec3 sceneScale) {
    float currentTextureScale = calculateTextureScale(sceneScale);
    if (currentTextureScale < 0) return;
    float textureScaleFactor = currentTextureScale / desiredTextureScale;
    for (auto &texCoord : textureCoords) {
        if (textureScaleFactor < 0.0001)
            std::cout << "problem" << std::endl;
        texCoord = texCoord * textureScaleFactor;
    }
}

void Geometry::scaleCoords(float scale) {
    for (auto& coord : coords) {
        coord = coord * scale;
    }
}


Material::Material() : diffuseColor(), ambientIntensity(0), specularColor(), shininess(0), transparency(0)
{

}

Material::Material(float* diffuseColor, float ambientIntensity,
    float* specularColor, float shininess, float transparency)
    :ambientIntensity(ambientIntensity), shininess(shininess), transparency(transparency)
{
    this->diffuseColor.setVector(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
    this->specularColor.setVector(specularColor[0], specularColor[1], specularColor[2]);
}

void Material::fillMaterial(float* diffuseColor, float ambientIntensity,
    float* specularColor, float shininess, float transparency)
{
    this->diffuseColor.setVector(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
    this->ambientIntensity = ambientIntensity;
    this->specularColor.setVector(specularColor[0], specularColor[1], specularColor[2]);
    this->shininess = shininess;
    this->transparency = transparency;
}

bool Material::compareDiffuseColor(const vec3& diffuseComponent) const{
    float epsilon = 0.0001f;
    return (diffuseColor - diffuseComponent).len2() < epsilon;
}
bool Material::compareDiffuseColor(const Material& otherMaterial) const {
    return compareDiffuseColor(otherMaterial.diffuseColor);
}
