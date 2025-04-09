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
        if (autoIn.eof()) break;

        if (command == "Load") {
            std::string sceneName;
            autoIn >> sceneName;
            if (!sm.loadScene(sceneName)) {
                std::cout << "Could not load Scene " << sceneName << std::endl;
            }
            else {
                std::cout << std::endl << "Loaded, Id: " << sceneCount << " Name: " << sceneName << std::endl;
                sceneCount++;
            }
        }
        else if (command == "LoadTextured") {
            std::string sceneTexturedName;
            autoIn >> sceneTexturedName;
            if (!sm.loadTexturedScene(sceneTexturedName)) {
                std::cout << "Could not load Scene " << sceneTexturedName << std::endl;
            }
            else {
                std::cout << "Loaded " << sceneTexturedName << std::endl;
            }
        }
        else if (command == "PrintId")
        {
            int sceneID = -1;
            autoIn >> sceneID;
            if (!sm.writeGeometriesOfScene(sceneID)) {
                std::cout << "Could not find and print the scene index " << sceneID << std::endl;
            }
            else
            {
                std::cout << "Scene " << idPlaceholder << std::endl;
            }
        }
        else if (command == "PrintName")
        {
            std::string sceneName;
            autoIn >> sceneName;
            if (!sm.writeGeometriesOfScene(sceneName)) {
                std::cout << "Could not find and print the scene " << sceneName << std::endl;
            }
        }
        else if (command == "SaveId")
        {
            int sceneID = -1;
            std::string outFileName;
            autoIn >> sceneID >> outFileName;
            if (!sm.saveScene(outFileName, sceneID)) {
                std::cout << "Could not find and save the scene index " << sceneID << std::endl;
            }
            else {
                std::cout << "Saved " << idPlaceholder << std::endl;
            }
        }
        else if (command == "SaveName") 
        {
            std::string sceneName;
            std::string outFileName;
            autoIn >> sceneName >> outFileName;
            if (sm.saveScene(outFileName, sceneName)) {
                std::cout << "Could not find and save the scene " << sceneName << std::endl;
            }
        }
        else if (command == "InstanceId")
        {
            int sceneID = -1;
            autoIn >> sceneID;
            if (!sm.instanceGeometry(sceneID)) {
                std::cout << "Could not find and instance the scene index " << sceneID << std::endl;
            }
        }
        else if (command == "InstanceName")
        {
            std::string sceneName;
            autoIn >> sceneName;
            if (!sm.instanceGeometry(sceneName)) {
                std::cout << "Could not find and instance the scene " << sceneName << std::endl;
            }
        }
        else if (command == "InstanceAll")
        {
            sm.instanceAllGeometry();
        }
        else if (command == "CopyTextureCoordsToFromId")
        {
            int sceneOneID = -1;
            int sceneTwoID = -1;
            autoIn >> sceneOneID >> sceneTwoID;
            sm.copyTextureCoordinatesBetweenScenes(sceneTwoID, sceneOneID);
        }
        else if (command == "CopyTextureCoordsToFromName")
        {
            std::string sceneOneName;
            std::string sceneTwoName;
            autoIn >> sceneOneName >> sceneTwoName;
            sm.copyTextureCoordinatesBetweenScenes(sceneTwoName, sceneOneName);
        }
        else if (command == "CopyAllTextureCoordsToId")
        {
            int sceneID = -1;
            autoIn >> sceneID;
            sm.copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(sceneID);
        }
        else if (command == "CopyAllTextureCoordsToName")
        {
            std::string sceneName;
            autoIn >> sceneName;
            sm.copyTextureCoordinatesFromAllMyScenesToSpecifiedScene(sceneName);
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
            std::string sceneName;
            std::string referenceSceneFilePath;
            autoIn >> sceneName;
            autoIn >> referenceSceneFilePath;
            if (!sm.loadScene(referenceSceneFilePath)) {
                std::cout << "Could not load Scene " << referenceSceneFilePath << std::endl;
            }
            else {
                std::cout << "Loaded, Id: " << sceneCount << "Name: " << referenceSceneFilePath << std::endl;
                sceneCount++;
            }
            sm.convertSceneSpotLightsToGonioLights(sceneName, fileStringPlaceholder);
        }
        else if (command == "ExportToPBRT")
        {
            int cameraID = -1;
            int createNewGeometry = 1;
            std::string outputHeaderName;
            std::string outputImageName;
            autoIn >> cameraID >> outputHeaderName >> outputImageName >> createNewGeometry;
            sm.exportAllToPBRT(cameraID, outputHeaderName, outputImageName, (bool)createNewGeometry);
        }
        else if (command == "ExportToMitsuba")
        {
            int cameraID = -1;
            int createNewGeometry = 1;
            std::string outputHeaderName;
            std::string outputImageName;
            autoIn >> cameraID >> outputHeaderName >> outputImageName >> createNewGeometry;
            sm.exportAllToMitsuba(cameraID, outputHeaderName, outputImageName, (bool)createNewGeometry);
        }
        else if (command == "DeleteSceneByID")
        {
            int sceneID = 0;
            autoIn >> sceneID;
            sm.deleteScene(sceneID);
            sceneCount--;
        }
        else if (command == "RotateAroundY")
        {
            int degrees = 0;
            autoIn >> degrees;
            sm.rotateSceneAroundY(0, degrees);
        }
        else if (command == "LoadMaterialsFile")
        {
            std::string file_path;
            autoIn >> file_path;
            bool success = sm.materialsFile->LoadMaterials(file_path);
            if (!success) std::cout << "Cannot find Materials file: " << file_path << std::endl;
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
