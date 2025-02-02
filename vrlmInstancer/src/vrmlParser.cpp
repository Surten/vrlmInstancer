#include <iostream>

#include "vrmlParser.h"


//#define LOGSTRANSFORM
//#define LOGSSHAPE
//#define LOGSGEOMETRY
#define LOGS



VrmlParser::VrmlParser() : scene(nullptr) {

    lastWasNumber = false;
    n = 0;
}


void VrmlParser::skipComments() {
    char next;
    do { vrlmFile.get(next); } while (next != '\n');
}

void VrmlParser::readSymbol() {
    int c;
    do {
        vrlmFile >> std::ws;        // eat leading spaces
        c = vrlmFile.peek();        // see the next symbol to know what to do with it
        if (c == EOF)
        {
            //std::cout << "End Of File" << std::endl;
            return;
        }
        if (c == '#') skipComments();   // if there are comments, skip them
    } while (static_cast<char>(c) == '#'); // there can be more consecutive lines of comments

    if (std::isdigit(c) || c == '-')    // determine if the next symbol is a number or a string
    {
        vrlmFile >> n;
        lastWasNumber = true;
    }
    else
    {
        if (c == ',' || c == '"') {
            char temp;
            vrlmFile.get(temp);
            str = temp;
        }
        else
        {
            vrlmFile >> str;
        }
        lastWasNumber = false;
    }

}

Scene* VrmlParser::parseFile(const char* vrlmFileName) {
    vrlmFile.open(vrlmFileName);
    if (!vrlmFile.is_open()) {
        std::cout << "Input file wasn't open!";
        return nullptr;
    }
    scene = new Scene(vrlmFileName);
    parseNextNode();
    return scene;
}

void VrmlParser::parseNextNode() {
    while (!vrlmFile.eof()) {   // read all root nodes
        readSymbol();
        if (str == "DEF") {     // we always expect the root nodes to start with a DEF keyword
            parseNode(nullptr, true);  // there is no parent node, so we pass nullptr
        }
        else if (str == "Transform" || str == "TimeSensor"
            || str == "SpotLight" || str == "PointLight" || str == "GonioLight" || str == "Viewpoint"
            || str == "NavigationInfo")
        {
            parseNode(nullptr, false);
        }
    }
    //std::cout << "done Parsing file" << std::endl;
    vrlmFile.close();           // finish
}



void VrmlParser::parseNode(TransformNode* parent, bool hasDEF) {
    std::string name;
    if (hasDEF)
    {
        readSymbol();
        name = str;
        readSymbol();
    }

    if (str == "Transform") {
        // create a new TransformNode to store the data that will be read
        #ifdef LOGSTRANSFORM
        std::cout << "Reading Transform node " << name << std::endl;
        #endif
        TransformNode* transformNode = new TransformNode(name);
        scene->AllNodes.push_back(transformNode);

        // fill it up with info, different if it is a Root node
        if (parent == nullptr) {
            scene->RootNodes.push_back(transformNode);
            transformNode->nodeDepth = 0;
        }
        else {
            parent->children.push_back(transformNode);
            transformNode->parent = parent;
            transformNode->nodeDepth = parent->nodeDepth + 1;
        }
        // continue reading the file to fill up the Transform node
        parseTransformNode(transformNode);
    }
    else if (str == "TimeSensor") {
        //we skip the TimeSensor for now, we will implemet it when the time comes, haha, get it? time
        readSymbol();
        readSymbol();
        readSymbol();
        readSymbol();
        readSymbol();
        readSymbol();

    }
    else if (str == "SpotLight") {
        LightNode* lightNode = new LightNode();
        lightNode->name = name;
        scene->AllNodes.push_back(lightNode);
        scene->lights.push_back(lightNode);
        if (parent == nullptr) {
            scene->RootNodes.push_back(lightNode);
            lightNode->nodeDepth = 0;
        }
        else {
            parent->children.push_back(lightNode);
            lightNode->parent = parent;
            lightNode->nodeDepth = parent->nodeDepth + 1;
        }
        parseSpotLight(lightNode);

    }
    else if (str == "GonioLight") {
        LightNode* lightNode = new LightNode();
        lightNode->name = name;
        scene->AllNodes.push_back(lightNode);
        scene->lights.push_back(lightNode);
        if (parent == nullptr) {
            scene->RootNodes.push_back(lightNode);
            lightNode->nodeDepth = 0;
        }
        else {
            parent->children.push_back(lightNode);
            lightNode->parent = parent;
            lightNode->nodeDepth = parent->nodeDepth + 1;
        }
        parseGonioLight(lightNode);

    }
    else if (str == "Viewpoint") {
        ViewPointNode* viewPointNode = new ViewPointNode();
        viewPointNode->name = name;
        scene->AllNodes.push_back(viewPointNode);
        scene->Cameras.push_back(viewPointNode);
        if (parent == nullptr) {
            scene->RootNodes.push_back(viewPointNode);
            viewPointNode->nodeDepth = 0;
        }
        else {
            parent->children.push_back(viewPointNode);
            viewPointNode->parent = parent;
            viewPointNode->nodeDepth = parent->nodeDepth + 1;
        }
        parseViewPoint(viewPointNode);

    }


}

void VrmlParser::parseTransformNode(TransformNode* transformNode) {

    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Transform node named " << transformNode->name << std::endl;

    do {
        readSymbol();   //we expect a property or '}'
        // translation
        if (str == "translation") {
            readSymbol();
            transformNode->translation.x = n;
            readSymbol();
            transformNode->translation.y = n;
            readSymbol();
            transformNode->translation.z = n;
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
            readSymbol();
            transformNode->scale.x = n;
            readSymbol();
            transformNode->scale.y = n;
            readSymbol();
            transformNode->scale.z = n;
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
        else if (str[0] != '}')     // if it does not contain a '}' any of the strings above, we assume there was someting wrong
            std::cout << "error reading properties of Transform named " << transformNode->name << std::endl;
        else
            break;       // if we successfully found the '}' ending symbol, we end the loop

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
        else if (str[0] != '}')     // if it does not contain a '}' any of the strings above, we assume there was someting wrong
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
        else if (str[0] != '}')     // if it does not contain a '}' any of the strings above, we assume there was someting wrong
            std::cout << "error reading properties of Material of node" << std::endl;
        else
            break;      // if we successfully found the '}' ending symbol, we end the loop

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
            readSymbol();   // read '"'
            readSymbol();

            shapeNode->textureFilePath = str.substr(0, str.size() - 1);
        }
        else if (str[0] != '}')     // if it does not contain a '}' any of the strings above, we assume there was someting wrong
            std::cout << "error reading properties of Texture of node" << std::endl;
        else
            break;      // if we successfully found the '}' ending symbol, we end the loop

    } while (true);
}

void VrmlParser::parseGeometry(ShapeNode* shapeNode) {
    readSymbol();
    if (str == "USE") {
        readSymbol();
        for (size_t i = 0; i < scene->geometries.size(); i++)
        {
            if (scene->geometries.at(i)->name == str) {
                shapeNode->geometry = scene->geometries.at(i);
                shapeNode->usesOtherGeometry = true;
                shapeNode->geometry->otherShapeNodesUsingMe.push_back(shapeNode);
#ifdef LOGSGEOMETRY
                std::cout << "Using geometry of " << str << std::endl;
#endif
                break;
            }
        }
        if (shapeNode->geometry == nullptr) {
            std::cout << "tried to use geometry " << str << " but could not find it" << std::endl;;
        }
        return;
    }
    if (str != "DEF") std::cout << "error: expected DEF at the start of Geometry node named " << shapeNode->parent->name << std::endl;
    Geometry* geometry = new Geometry();
    shapeNode->geometry = geometry;
    geometry->parent = shapeNode;
    scene->geometries.push_back(geometry);
    readSymbol();
    geometry->name = str;
    readSymbol();
    if (str != "IndexedFaceSet")
        std::cout << "I currently handle only IndexedFaceSet construction in geometry->node " << geometry->name << std::endl;
    readSymbol();
    if (str[0] != '{')
        std::cout << "error: expected { at the start of IndexedFaceSet node named " << geometry->name << std::endl;
    do {
        readSymbol();
        if (str == "ccw") {
            readSymbol();
            geometry->ccw = (str == "TRUE") ? true : false;
        }
        else if (str == "solid") {
            readSymbol();
            geometry->solid = (str == "TRUE") ? true : false;
        }
        else if (str == "normalPerVertex") {
            readSymbol();
            geometry->normalPerVertex = (str == "TRUE") ? true : false;
        }
        else if (str == "colorPerVertex") {
            readSymbol();
            geometry->colorPerVertex = (str == "TRUE") ? true : false;
        }
        else if (str == "creaseAngle") {
            readSymbol();
            geometry->creaseAngle = n;
        }
        else if (str == "color") {
            parseColor(shapeNode);
        }
        else if (str == "coord") {
            parseCoords(shapeNode);
        }
        else if (str == "texCoord") {
            parseTexCoords(shapeNode);
        }
        else if (str == "colorIndex") {
            parseColorIndex(shapeNode);
        }
        else if (str == "coordIndex") {
            parseCoordIndex(shapeNode);
        }
        else if (str == "texCoordIndex") {
            parseTexCoordIndex(shapeNode);
        }
        else if (str[0] != '}')     // if it does not contain a '}' any of the strings above, we assume there was someting wrong
            std::cout << "error of Geometry node " << geometry->name << " error symbol " << str << std::endl;
        else
            break;      // if we successfully found the '}' ending symbol, we end the loop
    } while (true);
}

void VrmlParser::parseColor(ShapeNode* shapeNode) {
    readSymbol();
    if (str != "Color") std::cout << "error: expected Color at the start of Color node " << std::endl;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Color node " << std::endl;
    readSymbol();
    if (str != "color") std::cout << "error: expected color at the start of Color node " << std::endl;
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of Color node " << std::endl;
    do {
        float vals[3];
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

void VrmlParser::parseCoords(ShapeNode* shapeNode) {
    std::string aaa = str;
    readSymbol();
    if (str == "DEF") { //read name for the coordinates if DEF
        readSymbol();
        std::string coordName = str;
        readSymbol();
    }


    if (str != "Coordinate") 
        std::cout << "error: expected Coordinate at the start of Coord node " << std::endl;
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Coord node " << std::endl;
    readSymbol();
    if (str != "point") std::cout << "error: expected point at the start of Coord node " << std::endl;
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of Coord node " << std::endl;
    do {
        readSymbol();
        if (str[0] == ']') break;
        float x = n;
        readSymbol();
        float y = n;
        readSymbol();
        float z = n;

        shapeNode->geometry->coords.push_back(vec3(x, y, z));
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
        readSymbol();
        if (str[0] == ']') return;
        float u = n;
        readSymbol();
        if (str[0] == ']') return;
        float v = n;
        shapeNode->geometry->textureCoords.push_back(vec2(u, v));
        readSymbol();
    } while (str[0] == ',' || str[0] == '[');
    readSymbol();
    if (str[0] != '}') std::cout << "error: expected } at the end of TexCoord node " << std::endl;
}

void VrmlParser::parseColorIndex(ShapeNode* shapeNode) {
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of CoordIndex node " << std::endl;
    do {
        readSymbol();
        if (str[0] == ']') return;
        readSymbol(); // read comma
        if (str[0] == ']') break;
    } while (str[0] == ',' || str[0] == '[');
}

void VrmlParser::parseCoordIndex(ShapeNode* shapeNode) {
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of CoordIndex node " << std::endl;
    do {
        int vals[] = { 0,0,0 };
        for (int i = 0; i < 3; i++)
        {
            if(!lastWasNumber) readSymbol();
            if (str[0] == ']') return;
            vals[i] = (int)n;
            readSymbol(); // read comma
        }
        shapeNode->geometry->facesPointsIndex.push_back(vec3i(vals[0], vals[1], vals[2]));
        readSymbol(); // read -1
        readSymbol(); // read comma
        if (str[0] == ']') break;
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
            if (str[0] == ']') return;
            vals[i] = (int)n;
            readSymbol(); // read comma
        }
        shapeNode->geometry->facesTextureIndex.push_back(vec3i(vals[0], vals[1], vals[2]));
        readSymbol(); // read -1
        readSymbol(); // read comma
        if (str[0] == ']') break;
    } while (str[0] == ',' || str[0] == '[');
}

void VrmlParser::parseShape(TransformNode* parent) {
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of Shape node named " << parent->name << std::endl;

    ShapeNode* shapeNode = new ShapeNode();
    parent->children.push_back(shapeNode);
    shapeNode->nodeDepth = parent->nodeDepth + 1;
    shapeNode->parent = parent;
    scene->AllNodes.push_back(shapeNode);
    scene->ShapeNodes.push_back(shapeNode);
    if (scene->AllNodes.size() == 11)
        std::cout << std::endl;
    do {
        readSymbol();
        if (str == "appearance") {
            parseApperance(shapeNode);
        }
        else if (str == "geometry") {
            parseGeometry(shapeNode);
        }
        else if (str[0] != '}')     // if it does not contain a '}' any of the strings above, we assume there was someting wrong
            std::cout << "error reading properties of Shape of node named " << parent->name << std::endl;
        else
            break;      // if we successfully found the '}' ending symbol, we end the loop

    } while (true);
}

void VrmlParser::parseChildren(TransformNode* parent) {
    readSymbol();
    if (str[0] != '[') std::cout << "error: expected [ at the start of children of Transform node named " << parent->name << std::endl;

    do {
        readSymbol();
        if (str == "DEF") {         // we foud a DEF keyword and need to determine what to parse, effectively doing recursion
            parseNode(parent, true);
            continue;
        }
        else if (str == "Transform" || str == "TimeSensor"
            || str == "SpotLight" || str == "PointLight" || str == "GonioLight" || str == "Viewpoint"
            || str == "NavigationInfo")
        {
            parseNode(parent, false);
        }
        else if (str[0] == ',') {   // comma is somehow valid
            continue;
        }
        else if (str == "Shape") {  // we foud a Shape node to parse
            #ifdef LOGSSHAPE
            std::cout << "Reading Shape node of parent " << parent->name << std::endl;
            #endif
            parseShape(parent);
            continue;
        }
        else if (str == "timeSensor") { //We never found one on depth greater than 0
            //timeSensor
        }
        else if (str[0] != ']')     // if it does not contain a ']' any of the strings above, we assume there was someting wrong
            std::cout << "error reading properties of children of Transform node named " << parent->name << std::endl;
        else
            break;      // if we successfully found the ']' ending symbol, we end the loop
    } while (true);
}

void VrmlParser::parseSpotLight(LightNode* light) {
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of light named " << light->name << std::endl;
    
    do {
        readSymbol();
        if (str == "intensity") {
            readSymbol();
            light->intensity = n;
            continue;
        }
        else if (str == "color") {
            readSymbol();
            light->color.x = n;
            readSymbol();
            light->color.y = n;
            readSymbol();
            light->color.z = n;
            continue;
        }
        else if (str == "location") {
            readSymbol();
            light->location.x = n;
            readSymbol();
            light->location.y = n;
            readSymbol();
            light->location.z = n;
            continue;
        }
        else if (str == "direction") {
            readSymbol();
            light->direction.x = n;
            readSymbol();
            light->direction.y = n;
            readSymbol();
            light->direction.z = n;
            continue;
        }
        if (str == "cutOffAngle") {
            readSymbol();
            light->cutOffAngle = n;
            continue;
        }
        if (str == "beamWidth") {
            readSymbol();
            light->beamWidth = n;
            continue;
        }
        if (str == "on") {
            readSymbol();
            str == "FALSE" ? light->on = false : light->on = true;
            continue;
        }
        if (str == "radius") {
            readSymbol();
            light->radius = n;
            continue;
        }
        else if (str[0] != '}')     // if it does not contain a '}' any of the strings above, we assume there was someting wrong
            std::cout << "error reading properties of light named " << light->name << std::endl;
        else
            break;      // if we successfully found the '}' ending symbol, we end the loop
    } while (true);

    light->lightType = LightNode::LightType::SPOTLIGHT;
}

void VrmlParser::parseGonioLight(LightNode* light) {
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of light named " << light->name << std::endl;

    do {
        readSymbol();
        if (str == "intensity") {
            readSymbol();
            light->intensity = n;
            continue;
        }
        else if (str == "url") {
            readSymbol();   // read "["
            readSymbol();   // read '"'
            readSymbol();
            light->url = str.substr(0, str.size() - 1);
            readSymbol();   // read "]"
            continue;
        }
        else if (str == "color") {
            readSymbol();
            light->color.x = n;
            readSymbol();
            light->color.y = n;
            readSymbol();
            light->color.z = n;
            continue;
        }
        else if (str == "location") {
            readSymbol();
            light->location.x = n;
            readSymbol();
            light->location.y = n;
            readSymbol();
            light->location.z = n;
            continue;
        }
        else if (str == "direction") {
            readSymbol();
            light->direction.x = n;
            readSymbol();
            light->direction.y = n;
            readSymbol();
            light->direction.z = n;
            continue;
        }
        else if (str == "on") {
            readSymbol();
            str == "FALSE" ? light->on = false : light->on = true;
            continue;
        }
        else if (str[0] != '}')     // if it does not contain a '}' any of the strings above, we assume there was someting wrong
            std::cout << "error reading properties of light named " << light->name << std::endl;
        else
            break;      // if we successfully found the '}' ending symbol, we end the loop
    } while (true);

    light->lightType = LightNode::LightType::GONIOLIGHT;
}

void VrmlParser::parseViewPoint(ViewPointNode* viewPointNode) {
    readSymbol();
    if (str[0] != '{') std::cout << "error: expected { at the start of viewport named " << viewPointNode->name << std::endl;

    do {
        readSymbol();
        if (str == "position") {
            readSymbol();
            viewPointNode->position.x = n;
            readSymbol();
            viewPointNode->position.y = n;
            readSymbol();
            viewPointNode->position.z = n;
            continue;
        }
        else if (str == "orientation") {
            readSymbol();
            viewPointNode->orientation[0] = n;
            readSymbol();
            viewPointNode->orientation[1] = n;
            readSymbol();
            viewPointNode->orientation[2] = n;
            readSymbol();
            viewPointNode->orientation[3] = n;
            continue;
        }
        if (str == "fieldOfView") {
            readSymbol();
            viewPointNode->fieldOfView = n;
            continue;
        }
        if (str == "description") {
            readSymbol();
            do 
            {
                readSymbol();
                viewPointNode->description += str;
            } while (str.find('"') == std::string::npos);

            //get rid of the last '"'
            viewPointNode->description = viewPointNode->description.substr(0, viewPointNode->description.size() - 1);

            continue;
        }
        else if (str[0] != '}')     // if it does not contain a '}' any of the strings above, we assume there was someting wrong
            std::cout << "error reading properties of viewPointNode named " << viewPointNode->name << std::endl;
        else
            break;      // if we successfully found the '}' ending symbol, we end the loop
    } while (true);
}
