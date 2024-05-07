#pragma once
#include <string>
#include <vector>
#include <cmath>



class vec3 {
public:
    float x, y, z;
public:
    vec3() { x = y = z = 0; }
    vec3(float x, float y, float z): x(x), y(y), z(z) {}
    vec3 operator+(const vec3 &other) {
        return vec3(x + other.x, y + other.y, z + other.z);
    }
    vec3 operator/(float num) {
        return vec3(x / num, y  / num, z / num);
    }
    vec3& operator+=(const vec3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
    vec3 operator-(const vec3& other) {
        return vec3(x - other.x, y - other.y, z - other.z);
    }
    vec3 operator*(float other) {
        return vec3(x * other, y * other, z * other);
    }
    void setVector(float a, float b, float c) {
        x = a; y = b; z = c;
    }
    float len() {
        return std::sqrt(x * x + y * y + z * z);
    }
    float len2() {
        return (x * x + y * y + z * z);
    }
    bool areEqual(vec3 other) {
        return (other - *this).len2() < 0.0001f ;
    }
};
std::ostream& operator<<(std::ostream& os, const vec3& obj);


class vec2 {
public:
    float x, y;
public:
    vec2() { x = y = 0; }
    vec2(float x, float y) : x(x), y(y) {}
};
class vec3i {
public:
    int x, y, z;
public:
    vec3i() { x = y = z = 0; }
    vec3i(int x, int y, int z) : x(x), y(y), z(z) {}
};


class AABB {
public:
    vec3 min, max;

public:
    AABB();
    AABB(vec3 a, vec3 b);
    AABB(vec3 a, vec3 b, vec3 c);

    void uniteWithPoint(vec3 point);
    void uniteWithAABB(AABB other);
    vec3 getDiagonal();
    vec3 getArithmeticCenter();
};
class ShapeNode;

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
    bool colorPerVertex;
    float creaseAngle;
    ShapeNode* parent;
    std::vector<ShapeNode*> otherShapeNodesUsingMe;
    AABB aabb;
    Geometry() : ccw(true), solid(true), normalPerVertex(true), creaseAngle(0.0f), aabb(), parent(nullptr){}

    void calculateAABB();
    AABB getAABB();

    vec3 getCenterOfGravity();
private:
    float triangleArea(vec3 A, vec3 B, vec3 C);
    vec3 getMidPoint(vec3 a, vec3 b);
    vec3 getWeightedMidPoint(vec3 a, float massA, vec3 b, float massB);
};

class Material {
public:
    Material();
    Material(float* diffuseColor, float ambientIntensity,
        float* specularColor, float shininess, float transparency);

    void fillMaterial(float* diffuseColor, float ambientIntensity,
        float* specularColor, float shininess, float transparency);


public:
    float diffuseColor[3];
    float ambientIntensity;
    float specularColor[3];
    float shininess;
    float transparency;
};
