#pragma once
#include <string>
#include <vector>
#include <cmath>


/// <summary>
/// Ordinary vec3 class with overloaded operators and some common methods
/// </summary>
class vec3 {
public:
    float x, y, z;
public:
    vec3() { x = y = z = 0; }
    vec3(float x, float y, float z): x(x), y(y), z(z) {}
    vec3 operator+(const vec3 &other) const{
        return vec3(x + other.x, y + other.y, z + other.z);
    }
    vec3 operator/(float num) const {
        return vec3(x / num, y  / num, z / num);
    }
    vec3& operator+=(const vec3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
    vec3 operator-(const vec3& other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }
    vec3 operator*(float other) const {
        return vec3(x * other, y * other, z * other);
    }
    vec3 operator*(vec3 other) const {
        return vec3(x * other.x, y * other.y, z * other.z);
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
    void normalize() {
        *this = *this / len();
    }
    std::string toString() {
        return std::string(std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z));
    }
};
std::ostream& operator<<(std::ostream& os, const vec3& obj);

/// <summary>
/// Ordinary vec2 class with overloaded operators and some common methods
/// </summary>
class vec2 {
public:
    float x, y;
public:
    vec2() { x = y = 0; }
    vec2(float x, float y) : x(x), y(y) {}

    vec2 operator*(float other) {
        return vec2(x * other, y * other);
    }
    vec2 operator-(const vec2& other) {
        return vec2(x - other.x, y - other.y);
    }
    double len() {
        return std::sqrt(x * x + y * y);
    }
};

class vec4 {
public:
    float x; float y; float z; float par;
    vec4() : x(0.f), y(0.f), z(0.f), par(0.f) {}
    vec4(float px, float py, float pz, float p) : x(px), y(py), z(pz), par(p) {}
    vec4(const float* pp) : x(pp[0]), y(pp[1]), z(pp[2]), par(pp[3]) {}
    /// Operator prirazeni
    vec4& operator=(const vec4&);
    /// Kopirovaci konstruktor
    vec4(const vec4&);
    // Metoda nastavi definovane hodnoty
    void setVector(float vx, float vy, float vz, float vpar) { x = vx; y = vy; z = vz; par = vpar; }
    /// Spratelena metoda pro vypis vektoru do proudu
    friend std::ostream& operator<<(std::ostream&, const vec4&);
    /// Vraci true, iff vektor nulovy
    bool retEmptyVector();
};

std::ostream& operator<<(std::ostream& os, const vec2& obj);

/// <summary>
/// Ordinary vec3 for integers class with overloaded operators and some common methods
/// </summary>
class vec3i {
public:
    int x, y, z;
public:
    vec3i() { x = y = z = 0; }
    vec3i(int x, int y, int z) : x(x), y(y), z(z) {}
};
std::ostream& operator<<(std::ostream& os, const vec3i& obj);

/// <summary>
/// AABB class for Axis-aligned bounding box
/// We have an implementation ensuring that we always have minimal 
/// coordinates in min variable and vice versa
/// Methods are for building up AABB by unifying it with a point or AABB
/// Other methods return basic informations about the AABB
/// </summary>
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

//we need this to remember the parent Shape node in the geometry
class ShapeNode;


/// <summary>
/// Contains a single mesh, with optional normals and texture coordinates
/// Has some bool variables similar to what can be set inside VRML files
/// Name serves as Identifier for instancing
/// otherShapeNodesUsingMe is an array of all Shape nodes that have reference to this geometry except for the direct parent
/// aabb stores the AABB of the entire mesh so that we will not need to calculate it again if we ask for it
/// </summary>
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

    vec3 centerOfGravity;
    std::vector<ShapeNode*> otherShapeNodesUsingMe;
    Geometry() : ccw(true), solid(true), normalPerVertex(true), creaseAngle(0.0f), aabb(), parent(nullptr), colorPerVertex(false), centerOfGravity(INFINITY, INFINITY, INFINITY){}

public:
    // calculate AABB if needed and return it
    AABB getAABB();

    // get center of gravity of the mesh, calculated with area of triangles as mass
    vec3 getCenterOfGravity();

    void scaleTextureCoords(float desiredTextureScale, vec3 sceneScale);
    void scaleCoords(float scale);
    float calculateTextureScale(vec3 sceneScale);

private:
    void calculateAABB();
    AABB aabb;
    float triangleArea(vec3 A, vec3 B, vec3 C);
    vec3 getMidPoint(vec3 a, vec3 b);
    vec3 getWeightedMidPoint(vec3 a, float massA, vec3 b, float massB);
};


/// <summary>
/// Simple class holding the information about Material
/// </summary>
class Material {
public:
    Material();
    Material(float* diffuseColor, float ambientIntensity,
        float* specularColor, float shininess, float transparency);

    void fillMaterial(float* diffuseColor, float ambientIntensity,
        float* specularColor, float shininess, float transparency);

    bool compareDiffuseColor(const vec3& diffuseComponent) const;
    bool compareDiffuseColor(const Material& otherMaterial) const;


public:
    vec3 diffuseColor;
    float ambientIntensity;
    vec3 specularColor;
    float shininess;
    float transparency;
};
