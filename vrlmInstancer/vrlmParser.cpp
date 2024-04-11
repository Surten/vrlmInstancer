#include <iostream>
#include "vrlmParser.h"



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
            exit(2);
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
    while (vrlmFile.is_open()) {
        readSymbol();
        if (str == "DEF") {
            parseDEF();
        }
    }
}



void VrmlParser::parseDEF() {
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
        parseTransformNode(name);
    }

}

void VrmlParser::parseTransformNode(std::string name) {

    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Transform node named " << name << std::endl;

    float translation[] = { 0,0,0 };
    float rotation[] = { 0,0,0,0 };
    float scale[] = { 1,1,1 };
    float scaleOrientation[] = { 0,0,0,0 };

    do {
        readSymbol();
        // translation
        if (str == "translation") {
            for (int i = 0; i < 3; i++)
            {
                readSymbol();
                translation[i] = n;
            }
        }
        // rotation
        else if (str == "rotation") {
            for (int i = 0; i < 4; i++)
            {
                readSymbol();
                rotation[i] = n;
            }
        }
        // scale
        else if (str == "scale") {
            for (int i = 0; i < 3; i++)
            {
                readSymbol();
                scale[i] = n;
            }
        }
        // scaleOrientation
        else if (str == "scaleOrientation") {
            for (int i = 0; i < 4; i++)
            {
                readSymbol();
                scaleOrientation[i] = n;
            }
        }
        else if (str == "children") {
            parseChildren(name);
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Transform named " << name << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseApperance(std::string name) {
    readSymbol();
    std::string apperanceName;
    apperanceName = str;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Apperance node named " << name << std::endl;
    do {
        readSymbol();
        if (str == "material") {
            parseMaterial(name);
        }
        else if (str == "texture") {
            parseTexture(name);
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Shape of node named " << name << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseMaterial(std::string name) {
    readSymbol();
    std::string materialName;
    materialName = str;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Material node named " << name << std::endl;

    float diffuseColor[] = { 0,0,0 };
    float ambientIntensity = 0;
    float specularColor[] = { 0,0,0 };
    float shininess = 0;
    float transparency = 0;

    do {
        readSymbol();
        if (str == "diffuseColor") {
            for (int i = 0; i < 3; i++)
            {
                readSymbol();
                diffuseColor[i] = n;
            }
        }
        else if (str == "ambientIntensity") {
            readSymbol();
            ambientIntensity = n;
        }
        else if (str == "specularColor") {
            for (int i = 0; i < 3; i++)
            {
                readSymbol();
                specularColor[i] = n;
            }
        }
        else if (str == "shininess") {
            readSymbol();
            shininess = n;
        }
        else if (str == "transparency") {
            readSymbol();
            transparency = n;
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Material of node named " << name << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseTexture(std::string name) {
    readSymbol();
    std::string textureName;
    textureName = str;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Texture node named " << name << std::endl;
    std::string textureFilePath;
    do {
        readSymbol();
        if (str == "url") {
            readSymbol();
            textureFilePath = str;
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Texture of node named " << name << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseGeometry(std::string name) {
    readSymbol();
    if (str != "DEF") std::cout << "error: expected DEF at the start of Geometry node named " << name << std::endl;
    readSymbol();
    std::string geometryName = str;
    readSymbol();
    if (str != "IndexedFaceSet") 
        std::cout << "I currently handle only IndexedFaceSet construction in geometry node " << geometryName << std::endl;
    readSymbol();
    if (str[0] != '{')
        std::cout << "error: expected { at the start of IndexedFaceSet node named " << geometryName << std::endl;

    do {
        readSymbol();
        if (str == "ccw") {
            //save ccw after I create the Node structure
            readSymbol();
        }
        else if (str == "solid") {
            //save solid after I create the Node structure
            readSymbol();
        }
        else if (str == "coord") {
            parseCoords();
        }
        else if (str == "texCoord") {
            parseTexCoords();
        }
        else if (str == "coordIndex") {
            parseCoordIndex();
        }
        else if (str == "texCoordIndex") {
            parseTexCoordIndex();
        }
        else if (str[0] != '}')
            std::cout << "error of Geometry node " << geometryName << " error symbol " << str << std::endl;
        else
            break;
    } while (true);
}

void VrmlParser::parseCoords() {
    readSymbol();
    if (str != "DEF") std::cout << "error: expected DEF at the start of Coord node " << std::endl;
    readSymbol();
    std::string coordName = str;
    readSymbol();
    if(str != "Coordinate") std::cout << "error: expected Coordinate at the start of Coord node " << std::endl;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Coord node " << std::endl;
    readSymbol();
    if(str != "point") std::cout << "error: expected point at the start of Coord node " << std::endl;
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of Coord node " << std::endl;
    do {
        float vals[] = { 0,0,0 };
        for (int i = 0; i < 3; i++)
        {
            readSymbol();
            if (str[0] == ']') break;
            vals[i] = n;
        }
        readSymbol();
    } while (str[0] == ',' || str[0] == '[');
    readSymbol();
    if (str[0] != '}') std::cout << "error: expected } at the end of Coord node " << std::endl;
}

void VrmlParser::parseTexCoords() {
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
        float vals[] = { 0,0,0 };
        for (int i = 0; i < 2; i++)
        {
            readSymbol();
            if (str[0] == ']') break;
            vals[i] = n;
        }
        readSymbol();
    } while (str[0] == ',' || str[0] == '[');
    readSymbol();
    if (str[0] != '}') std::cout << "error: expected } at the end of TexCoord node " << std::endl;
}

void VrmlParser::parseCoordIndex() {
    readSymbol();
    if(str[0] != '[') std::cout << "error: expected [ at the start of CoordIndex node " << std::endl;
    do {
        float vals[] = { 0,0,0 };
        for (int i = 0; i < 3; i++)
        {
            readSymbol();
            if (str[0] == ']') break;
            vals[i] = n;
            readSymbol(); // read comma
        }
        readSymbol(); // read -1
        readSymbol(); // read comma
    } while (str[0] == ',' || str[0] == '[');
}

void VrmlParser::parseTexCoordIndex() {
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of TexCoordIndex node " << std::endl;
    do {
        float vals[] = { 0,0,0 };
        for (int i = 0; i < 3; i++)
        {
            readSymbol();
            if (str[0] == ']') break;
            vals[i] = n;
            readSymbol(); // read comma
        }
        readSymbol(); // read -1
        readSymbol(); // read comma
    } while (str[0] == ',' || str[0] == '[');
}

void VrmlParser::parseShape(std::string name) {
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Shape node named " << name << std::endl;

    do {
        readSymbol();
        if (str == "appearance") {
            parseApperance(name);
        }
        else if (str == "geometry") {
            parseGeometry(name);
        }
        else if (str[0] != '}')
            std::cout << "error reading properties of Shape of node named " << name << std::endl;
        else
            break;

    } while (true);
}

void VrmlParser::parseChildren(std::string name) {
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of children of Transform node named " << name << std::endl;

    do {
        readSymbol();
        if (str == "DEF") {
            parseDEF();
            continue;
        }
        else if (str[0] == ',') {
            continue;
        }
        else if (str == "Shape") {
            parseShape(name);
            continue;
        }
        else if (str == "timeSensor") {
            //timeSensor
        }
        else if (str[0] != ']')
            std::cout << "error reading properties of children of Transform node named " << name << std::endl;
        else
            break;
    } while (true);
}