#include <iostream>
#include "vrlmParser.h"

#define LOGS



void VrmlParser::loadFile(const char* vrlmFileName) {
    vrlmFile.open(vrlmFileName);
    if (!vrlmFile.is_open()) {
        std::cout << "Input file wasn't open!";
    }

}

void VrmlParser::skipComments() {
    char next;
    do { vrlmFile.get(next); } while (next != '\n');
}

void VrmlParser::readSymbol() {
    int c;
    do {
        vrlmFile >> std::ws; // eat leading spaces
        c = vrlmFile.peek();
        if (c == EOF)
        {
            std::cout << "End Of File" << std::endl;
            return;
        }
        if (c == '#') skipComments();
    } while (static_cast<char>(c) == '#');

    if (std::isdigit(c) || c == '-')
    {
        vrlmFile >> n;
        lastWasNumber = true;
    }
    else
    {
        vrlmFile >> str;
        lastWasNumber = false;
    }

}

void VrmlParser::parseFile(const char* vrlmFileName) {
    loadFile(vrlmFileName);
    parseNextNode();
}

void VrmlParser::parseNextNode() {
    while (!vrlmFile.eof()) {
        readSymbol();
        if (str == "DEF") {
            parseDEF(nullptr);
        }
    }
}



void VrmlParser::parseDEF(TransformNode* parent) {
    std::string name;
    readSymbol();

    if (str != "Transform" && str != "," && str != "TimeSensor"
        && str != "kwSpotLight" && str != "PointLight" && str != "Viewpoint"
        && str != "NavigationInfo")
        //to determine if there is a name filter out all the possible node keywords, because I cannot think of better solution
    {
        name = str;
        readSymbol();
    }

    if (str == "Transform") {
        #ifdef LOGS
        std::cout << "Reading Transform node " << name << std::endl;
        #endif
        TransformNode* transformNode = new TransformNode(name);
        AllNodes.push_back(transformNode);
        if (parent == nullptr) RootNodes.push_back(transformNode);
        else {
            parent->children.push_back(transformNode); 
            transformNode->parent = parent;
        }
        parseTransformNode(transformNode);
    }
    else if (str == "TimeSensor") {
        readSymbol();
        readSymbol();
        readSymbol();
        readSymbol();
        readSymbol();
        readSymbol();

    }


}

void VrmlParser::parseTransformNode(TransformNode* transformNode) {

    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Transform node named " << transformNode->name << std::endl;

    do {
        readSymbol();
        // translation
        if (str == "translation") {
            for (int i = 0; i < 3; i++)
            {
                readSymbol();
                transformNode->translation[i] = n;
            }
        }
        // rotation
        else if (str == "rotation") {
            for (int i = 0; i < 4; i++)
            {
                readSymbol();
                transformNode->rotation[i] = n;
            }
        }
        // scale
        else if (str == "scale") {
            for (int i = 0; i < 3; i++)
            {
                readSymbol();
                transformNode->scale[i] = n;
            }
        }
        // scaleOrientation
        else if (str == "scaleOrientation") {
            for (int i = 0; i < 4; i++)
            {
                readSymbol();
                transformNode->scaleOrientation[i] = n;
            }
        }
        else if (str == "children") {
            parseChildren(transformNode);
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Transform named " << transformNode->name << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseApperance(ShapeNode* shapeNode) {
    readSymbol();
    std::string apperanceName;
    apperanceName = str;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Apperance node" << std::endl;
    do {
        readSymbol();
        if (str == "material") {
            parseMaterial(shapeNode);
        }
        else if (str == "texture") {
            parseTexture(shapeNode);
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Shape of node" << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseMaterial(ShapeNode* shapeNode) {
    readSymbol();
    std::string materialName;
    materialName = str;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Material node" << std::endl;

    do {
        readSymbol();
        if (str == "diffuseColor") {
            for (int i = 0; i < 3; i++)
            {
                readSymbol();
                shapeNode->material.diffuseColor[i] = n;
            }
        }
        else if (str == "ambientIntensity") {
            readSymbol();
            shapeNode->material.ambientIntensity = n;
        }
        else if (str == "specularColor") {
            for (int i = 0; i < 3; i++)
            {
                readSymbol();
                shapeNode->material.specularColor[i] = n;
            }
        }
        else if (str == "shininess") {
            readSymbol();
            shapeNode->material.shininess = n;
        }
        else if (str == "transparency") {
            readSymbol();
            shapeNode->material.transparency = n;
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Material of node" << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseTexture(ShapeNode* shapeNode) {
    readSymbol();
    shapeNode->textureType = str;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Texture node" << std::endl;
    do {
        readSymbol();
        if (str == "url") {
            readSymbol();

            shapeNode->textureFilePath = str.substr(1, str.size()-2);
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Texture of node" << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseGeometry(ShapeNode* shapeNode) {
    readSymbol();
    if (str != "DEF") std::cout << "error: expected DEF at the start of Geometry node named " << shapeNode->parent->name << std::endl;
    readSymbol();
    shapeNode->geometry.name = str;
    readSymbol();
    if (str != "IndexedFaceSet") 
        std::cout << "I currently handle only IndexedFaceSet construction in geometry node " << shapeNode->geometry.name << std::endl;
    readSymbol();
    if (str[0] != '{')
        std::cout << "error: expected { at the start of IndexedFaceSet node named " << shapeNode->geometry.name << std::endl;
    do {
        readSymbol();
        if (str == "ccw") {
            readSymbol();
            shapeNode->geometry.ccw = (str == "TRUE") ? true : false;
        }
        else if (str == "solid") {
            readSymbol();
            shapeNode->geometry.solid = (str == "TRUE") ? true : false;
        }
        else if (str == "normalPerVertex") {
            readSymbol();
            shapeNode->geometry.normalPerVertex = (str == "TRUE") ? true : false;
        }
        else if (str == "creaseAngle") {
            readSymbol();
            shapeNode->geometry.creaseAngle = n;
        }
        else if (str == "coord") {
            parseCoords(shapeNode);
        }
        else if (str == "texCoord") {
            parseTexCoords(shapeNode);
        }
        else if (str == "coordIndex") {
            parseCoordIndex(shapeNode);
        }
        else if (str == "texCoordIndex") {
            parseTexCoordIndex(shapeNode);
        }
        else if (str[0] != '}')
            std::cout << "error of Geometry node " << shapeNode->geometry.name << " error symbol " << str << std::endl;
        else
            break;
    } while (true);
}

void VrmlParser::parseCoords(ShapeNode* shapeNode) {
    readSymbol();
    if (str != "DEF") std::cout << "error: expected DEF at the start of Coord node " << std::endl;
    readSymbol();
    std::string coordName = str;
    readSymbol();
    if (str != "Coordinate") std::cout << "error: expected Coordinate at the start of Coord node " << std::endl;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Coord node " << std::endl;
    readSymbol();
    if (str != "point") std::cout << "error: expected point at the start of Coord node " << std::endl;
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of Coord node " << std::endl;
    do {
        float vals[3];
        for (int i = 0; i < 3; i++)
        {
            readSymbol();
            if (str[0] == ']') break;
            vals[i] = n;
        }
        shapeNode->geometry.coords.push_back(vec3(vals[0], vals[1], vals[2]));
        readSymbol();
    } while (str[0] == ',' || str[0] == '[');
    readSymbol();
    if (str[0] != '}') std::cout << "error: expected } at the end of Coord node " << std::endl;
}

void VrmlParser::parseTexCoords(ShapeNode* shapeNode) {
    readSymbol();
    if (str != "DEF") std::cout << "error: expected DEF at the start of TexCoord node " << std::endl;
    readSymbol();
    std::string coordName = str;
    readSymbol();
    if (str != "TextureCoordinate") std::cout << "error: expected Coordinate at the start of TexCoord node " << std::endl;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of TexCoord node " << std::endl;
    readSymbol();
    if (str != "point") std::cout << "error: expected point at the start of TexCoord node " << std::endl;
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of TexCoord node " << std::endl;
    do {
        float vals[] = { 0,0};
        for (int i = 0; i < 2; i++)
        {
            readSymbol();
            if (str[0] == ']') break;
            vals[i] = n;
        }
        shapeNode->geometry.textureCoords.push_back(vec2(vals[0], vals[1]));
        readSymbol();
    } while (str[0] == ',' || str[0] == '[');
    readSymbol();
    if (str[0] != '}') std::cout << "error: expected } at the end of TexCoord node " << std::endl;
}

void VrmlParser::parseCoordIndex(ShapeNode* shapeNode) {
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of CoordIndex node " << std::endl;
    do {
        int vals[] = { 0,0,0 };
        for (int i = 0; i < 3; i++)
        {
            readSymbol();
            if (str[0] == ']') break;
            vals[i] = n;
            readSymbol(); // read comma
        }
        shapeNode->geometry.facesPointsIndex.push_back(vec3i(vals[0], vals[1], vals[2]));
        readSymbol(); // read -1
        readSymbol(); // read comma
    } while (str[0] == ',' || str[0] == '[');
}

void VrmlParser::parseTexCoordIndex(ShapeNode* shapeNode) {
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of TexCoordIndex node " << std::endl;
    do {
        int vals[] = { 0,0,0 };
        for (int i = 0; i < 3; i++)
        {
            readSymbol();
            if (str[0] == ']') break;
            vals[i] = n;
            readSymbol(); // read comma
        }
        shapeNode->geometry.facesTextureIndex.push_back(vec3i(vals[0], vals[1], vals[2]));
        readSymbol(); // read -1
        readSymbol(); // read comma
    } while (str[0] == ',' || str[0] == '[');
}

void VrmlParser::parseShape(TransformNode* parent) {
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Shape node named " << parent->name << std::endl;

    ShapeNode* shapeNode = new ShapeNode();
    parent->children.push_back(shapeNode);
    shapeNode->parent = parent;
    do {
        readSymbol();
        if (str == "appearance") {
            parseApperance(shapeNode);
        }
        else if (str == "geometry") {
            parseGeometry(shapeNode);
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Shape of node named " << parent->name << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseChildren(TransformNode* parent) {
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of children of Transform node named " << parent->name << std::endl;

    do {
        readSymbol();
        if (str == "DEF") {
            parseDEF(parent);
            continue;
        }
        else if (str[0] == ',') {
            continue;
        }
        else if (str == "Shape") {
            #ifdef LOGS
            std::cout << "Reading Shape node of parent " << parent->name << std::endl;
            #endif
            parseShape(parent);
            continue;
        }
        else if (str == "timeSensor") {
            //timeSensor
        }
        else if (str[0] != ']')
            std::cout << "error reading properties of children of Transform node named " << parent->name << std::endl;
        else
            break;
    } while (true);
}