#include <iostream>
#include <fstream>
#include <string>

#include "scene.h"





int main()
{
    Scene scene;
    scene.loadSceneFromVrmlFile("0_fl_furniture.wrl");
    std::cout << "done" << std::endl;
}

