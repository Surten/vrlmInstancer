#include "application.h"

#include <iostream>
#include <fstream>


void Application::AutomaticMode(std::string fileName){
    std::ifstream autoIn(fileName);
    if (!autoIn.is_open()) {
        std::cout << "Input file wasn't open!";
        return;
    }

    int sceneCount = 0, texturedSceneCound = 0;
    std::string command;
    std::string fileStringPlaceholder;
    std::string nameStringPlaceholder;
    std::string nameSecondStringPlaceholder;
    int idPlaceholder = -1;
    int idSecondPlaceholder = -1;
    while (!autoIn.eof()) {
        autoIn >> command;
        if (command == "Load") {
            autoIn >> fileStringPlaceholder;
            if (!sm.loadScene(fileStringPlaceholder)) {
                std::cout << "Could not load Scene " << fileStringPlaceholder << std::endl;
            }
            else {
                std::cout << std::endl << "Loaded, Id: " << sceneCount << " Name: " << fileStringPlaceholder << std::endl;
                sceneCount++;
            }
        }
        else if (command == "LoadTextured") {
            autoIn >> fileStringPlaceholder;
            if (!sm.loadTexturedScene(fileStringPlaceholder)) {
                std::cout << "Could not load Scene " << fileStringPlaceholder << std::endl;
            }
            else {
                std::cout << "Loaded " << fileStringPlaceholder << std::endl;
            }
        }
        else if (command == "PrintId")
        {
            autoIn >> idPlaceholder;
            if (!sm.writeGeometriesOfScene(idPlaceholder)) {
                std::cout << "Could not find and print the scene index " << idPlaceholder << std::endl;
            }
            else
            {
                std::cout << "Scene " << idPlaceholder << std::endl;
            }
        }
        else if (command == "PrintName")
        {
            autoIn >> nameStringPlaceholder;
            if (!sm.writeGeometriesOfScene(nameStringPlaceholder)) {
                std::cout << "Could not find and print the scene " << nameStringPlaceholder << std::endl;
            }
        }
        else if (command == "SaveId")
        {
            autoIn >> idPlaceholder >> fileStringPlaceholder;
            if (!sm.saveScene(fileStringPlaceholder, idPlaceholder)) {
                std::cout << "Could not find and save the scene index " << idPlaceholder << std::endl;
            }
            else {
                std::cout << "Saved " << idPlaceholder << std::endl;
            }
        }
        else if (command == "SaveName") 
        {
            autoIn >> nameStringPlaceholder >> fileStringPlaceholder;
            if (sm.saveScene(fileStringPlaceholder, nameStringPlaceholder)) {
                std::cout << "Could not find and save the scene " << nameStringPlaceholder << std::endl;
            }
        }
        else if (command == "InstanceId")
        {
            autoIn >> idPlaceholder;
            if (!sm.instanceGeometry(idPlaceholder)) {
                std::cout << "Could not find and instance the scene index " << idPlaceholder << std::endl;
            }
        }
        else if (command == "InstanceName")
        {
            autoIn >> nameStringPlaceholder;
            if (!sm.instanceGeometry(nameStringPlaceholder)) {
                std::cout << "Could not find and instance the scene " << nameStringPlaceholder << std::endl;
            }
        }
        else if (command == "InstanceAll")
        {
            sm.instanceAllGeometry();
        }
        else if (command == "CopyTextureCoordsToFromId")
        {
            autoIn >> idPlaceholder >> idSecondPlaceholder;
            sm.copyTextureCoordinatesBetweenScenes(idSecondPlaceholder, idPlaceholder);
        }
        else if (command == "CopyTextureCoordsToFromName")
        {
            autoIn >> nameStringPlaceholder >> nameSecondStringPlaceholder;
            sm.copyTextureCoordinatesBetweenScenes(nameSecondStringPlaceholder, nameStringPlaceholder);
        }
        else if (command == "CopyAllTextureCoordsToId")
        {
            autoIn >> idPlaceholder;
            sm.copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(idPlaceholder);
        }
        else if (command == "CopyAllTextureCoordsToName")
        {
            autoIn >> nameStringPlaceholder;
            sm.copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(nameStringPlaceholder);
        }
        else if (command == "CopyAllTextureCoordsFromTexturedScenes")
        {
            std::cout << "Copying texture coordinates from Textured scenes" << std::endl;
            sm.copyTextureCoordinatesFromAllTexturedScenesToAllOtherLoadedScenes();
            std::cout << "Done - Copy Texture" << std::endl;
        }
        else if (command == "UnifyTextureScale")
        {
            std::cout << "Unifying texture coords scale" << std::endl;
            sm.unifyTextrureCoordScaleOfAllScenes();
            std::cout << "Done - Texture coords scale" << std::endl;
        }
        else if (command == "ScaleAllScenes")
        {
            std::cout << "Scaling all scenes by 0.01f" << std::endl;
            sm.scaleAllScenesGeometry(0.01f);
            std::cout << "Done - coord scale" << std::endl;
        }
        else if (command == "SwitchLightsToGonio")
        {
            autoIn >> nameStringPlaceholder;
            autoIn >> fileStringPlaceholder;
            if (!sm.loadScene(fileStringPlaceholder)) {
                std::cout << "Could not load Scene " << fileStringPlaceholder << std::endl;
            }
            else {
                std::cout << "Loaded, Id: " << sceneCount << "Name: " << fileStringPlaceholder << std::endl;
                sceneCount++;
            }
            sm.convertSceneSpotLightsToGonioLights(nameStringPlaceholder, fileStringPlaceholder);
        }
        else if (command == "ExportToPBRT")
        {
            float customZoom = 1.f;
            autoIn >> idPlaceholder >> nameStringPlaceholder >> nameSecondStringPlaceholder >> customZoom;
            sm.exportAllToPBRT(idPlaceholder, nameStringPlaceholder, nameSecondStringPlaceholder, customZoom);
        }
        else if (command == "DeleteSceneByID")
        {
            autoIn >> idPlaceholder;
            sm.deleteScene(idPlaceholder);
            sceneCount--;
        }
        else if (command == "RotateAroundY")
        {
            autoIn >> idPlaceholder;
            sm.rotateSceneAroundY(0, idPlaceholder);
        }
    }
}

void Application::ManualMode(){

    int numOfScene;
    int numOfOtherScene;
    while (true) {
        std::cout << std::endl << std::endl;
        std::cout << "Welcome, you have the following options: " << std::endl;
        std::cout << "1 - Load a VRML file (.wrl)" << std::endl;
        std::cout << "2 - Save a scene to a VRML file" << std::endl;
        std::cout << "3 - Write out all geometries of a scene" << std::endl;
        std::cout << "4 - Find Geometry duplicates" << std::endl;
        std::cout << "5 - Find and use geometries from the second scene in the first scene" << std::endl;
        std::cout << "6 - Find and use geometries from all the scenes in the given scene" << std::endl;
        std::cout << "7 - Find Geometry duplicates for all scenes" << std::endl;
        std::cout << "9 - Not Exit" << std::endl;


        char inputNumber = 0;
        std::cin >> inputNumber;
        if (inputNumber > '9' || inputNumber < '1')
        {
            std::cout << "Not a Number" << std::endl;
            continue;
        }

        switch (inputNumber) {
        case '1':
        {
            std::string inputFile;
            std::cout << "Input file name and location: ";
            std::cin >> inputFile;
            if(!sm.loadScene(inputFile)) std::cout << "Scene load failed" << std::endl;
            break;
        }
        case '2':
        {
            std::cout << "The index of the scene: ";
            std::cin >> numOfScene;
            std::string outputFile;
            std::cout << "Output file name: ";
            std::cin >> outputFile;
            sm.saveScene(outputFile, numOfScene);
            break;
        }
        case '3':
            std::cout << "The index of the scene: ";
            std::cin >> numOfScene;
            if (!sm.writeGeometriesOfScene(numOfScene)) {
                std::cout << "Could not find the scene by this index" << std::endl;
            }
            break;
        case '4':
            std::cout << "The index of the scene: ";
            std::cin >> numOfScene;
            sm.instanceGeometry(numOfScene);
            break;
        case '5':
            std::cout << "The index of the scene: ";
            std::cin >> numOfScene;
            std::cout << "The index of the other scene : ";
            std::cin >> numOfOtherScene;
            sm.copyTextureCoordinatesBetweenScenes(numOfScene, numOfOtherScene);
            break;
        case '6':
            std::cout << "The index of the scene: ";
            std::cin >> numOfScene;
            sm.copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(numOfScene);
            break;
        case '7':
            sm.instanceAllGeometry();
            break;
        case '9':
            sm.combineAllScenesIntoOne();

            break;
        case '11':

            return;
        }

    }
}
