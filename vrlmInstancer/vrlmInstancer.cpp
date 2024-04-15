#include <iostream>
#include <fstream>
#include <string>

#include "scene.h"





int main()
{
    Scene scene;
    scene.loadSceneFromVrmlFile("skrinTextured.wrl");
    scene.saveSceneToVrmlFile("out.wrl");
    std::cout << "done" << std::endl;
}

