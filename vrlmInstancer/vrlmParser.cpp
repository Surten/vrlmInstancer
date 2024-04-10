#include <iostream>
#include "vrlmParser.h"

std::ifstream vrlmFile;
std::string str;
float n;

void loadFile(const char* vrlmFileName) {
    vrlmFile.open(vrlmFileName);
    if (!vrlmFile.is_open()) {
        std::cout << "Input file wasn't open!";
    }

}

void skipComments() {
    char next;
    do { vrlmFile.get(next); } while (next != '\n');
}

void readSymbol(/*bool & wasNumber*/) {
    vrlmFile >> std::ws; // eat leading spaces
    int c = vrlmFile.peek();
    if (c == EOF) { std::cout << "End Of File" << std::endl; exit(2); }
    if (std::isdigit(c) || c == '-')
    {
        vrlmFile >> n;
        //wasNumber = true;
    }
    else
    {
        vrlmFile >> str;
        //wasNumber = false;
    }

}

void parseFile() {
    while (vrlmFile.is_open()) {
        readSymbol();
        if (str[0] == '#') { skipComments(); continue; };
        if (str == "DEF") {
            parseDEF();
        }
    }
}

void parseDEF() {
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

void parseTransformNode(std::string name) {

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
        else if (str[0] != '}') std::cout << "error reading properties of Transform named " << name << std::endl;

    } while (str[0] != '}');
}

void parseChildren(std::string name) {
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of children of Transform node named " << name << std::endl;

    do {
        readSymbol();
        if (str == "DEF") {
            parseDEF();
            continue;
        }
        if (str[0] == ',') {
            continue;
        }
        //shape
        //timeSensor

    } while (str[0] != ']');
}