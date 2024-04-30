#include <iostream>
#include <fstream>
#include <string>

#include "scene.h"





int main()
{
    //scene.loadSceneFromVrmlFile("VRML/2_fl_furniture_W.WRL");
    //scene.loadSceneFromVrmlFile("skrinTextured.wrl");
    //scene.geometryFun();
    //scene.saveSceneToVrmlFile("out.wrl");
    std::vector<Scene * > scenes;
    int numOfScene;
    int numOfOtherScene;
    while (true) {
        std::cout << std::endl << std::endl;
        std::cout << "Welcome, you have the following options: " << std::endl;
        std::cout << "1 - Load a VRML file (.wrl)" << std::endl;
        std::cout << "2 - Save a scene to a VRML file" << std::endl;
        std::cout << "3 - Write out all geometries of a scene" << std::endl;
        std::cout << "4 - Try to find the same geometries in two different scenes" << std::endl;
        std::cout << "5 - Find Geometry duplicates" << std::endl;
        std::cout << "6 - Exit" << std::endl;

        char inputNumber = 0;
        std::cin >> inputNumber;
        if (!std::isdigit(inputNumber))
        {
            std::cout << "Not a Number" << std::endl;
            continue;
        }

        switch (inputNumber) {
        case '1':
        {
            Scene * scene = new Scene();
            std::string inputFile;
            std::cout << "Input file name and location: ";
            std::cin >> inputFile;
            if (inputFile == "a")
                inputFile = "files/skrinTextured.wrl";
            if (inputFile == "b")
                inputFile = "files/0_fl_furniture.wrl";

            scene->loadSceneFromVrmlFile(inputFile);
            std::cout << "Loaded into a scene with index " << scenes.size() << std::endl;
            scenes.push_back(scene);
            break;
        }
        case '2':
        {
            std::cout << "The index of the scene: ";
            std::cin >> numOfScene;
            std::string outputFile;
            std::cout << "Output file name: ";
            std::cin >> outputFile;
            if (outputFile == "a")
                outputFile = "files/out.wrl";
            if (outputFile == "b")
                outputFile = "files/out2.wrl";
            scenes.at(numOfScene)->saveSceneToVrmlFile(outputFile);
            break;
        }
        case '3':
            std::cout << "The index of the scene: ";
            std::cin >> numOfScene;
            scenes.at(numOfScene)->writeOutGeometries();
            break;
        case '4':
            std::cout << "The index of the scene: ";
            std::cin >> numOfScene;
            std::cout << "The index of the other scene : ";
            std::cin >> numOfOtherScene;
            scenes.at(numOfScene)->findSimilarObjects(scenes.at(numOfOtherScene));
            break;
        case '5':
            std::cout << "The index of the scene: ";
            std::cin >> numOfScene;
            scenes.at(numOfScene)->findDuplicateGeometry();
            break;
        case '6':

            return 0;
        }
    }
}

