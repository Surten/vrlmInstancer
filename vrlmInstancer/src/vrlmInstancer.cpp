#include <iostream>
#include <fstream>
#include <string>

#include "scene.h"
#include "sceneManager.h"
#include "application.h"



int main()
{
    //std::string fileName = "automaticFileTest.txt";
    //std::string fileName = "switchLightsToGonio.txt";
    //std::string fileName = "automaticControls/ExportIndividualObjectsWithTextureToPBRT.txt";
    //std::string fileName = "automaticControls/FullPass.txt";
    //std::string fileName = "automaticControls/FullPassNoTexture.txt";
    //std::string fileName = "automaticControls/geometryConvert.txt";
    //std::string fileName = "automaticControls/TestPBRTExport.txt";
    //std::string fileName = "automaticControls/TestMitsubaExport.txt";
    //std::string fileName = "automaticControls/FullExportToMitsuba.txt";
    //std::string fileName = "automaticControls/generateAllStaticCamsMitsuba.txt";
    //std::string fileName = "automaticControls/generateAllStaticCamsPBRT.txt";
    //std::string fileName = "automaticControls/animTest.txt";
    //std::string fileName = "automaticControls/tamp.txt";
    //std::string fileName = "automaticControls/generateDynamicPBRT.txt";
    //std::string fileName = "automaticControls/DebugTexturedExport.txt";

    {
        std::string hallway = "automaticControls/Hallway.txt";
        Application app;
        app.AutomaticMode(hallway);
    }
    {
        std::string intoAtrium = "automaticControls/IntoAtrium.txt";
        Application app2;
        app2.AutomaticMode(intoAtrium);
    }
    {
        std::string roundel = "automaticControls/Roundel.txt";
        Application app3;
        app3.AutomaticMode(roundel);
    }

}













//#define MANUAL
//#define AUTOMATIC

#ifdef AUTOMATIC
/// <summary>
/// loads all files in the given array and instances geometry for each of them, saving them to a given folder
/// </summary>
//void instanceFiles(std::vector<std::string> & fileNames, std::string folderToSaveIn) {
//    
//    for (auto namaeva : fileNames) {
//        std::cout << "Parsing file: " << namaeva << std::endl;
//        Scene* scene = new Scene(namaeva.substr(5));
//        if (!scene->loadSceneFromVrmlFile(namaeva)) std::cout << "error loading " << namaeva << std::endl;
//        std::cout << "Finding identical geometry of file: " << scene->name << std::endl;
//        scene->findAndUseIdenticalGeometry();
//        std::cout << "Saving file: " << scene->name << std::endl;
//        scene->saveSceneToVrmlFile(folderToSaveIn + "/" + scene->name);
//        delete scene;
//
//    }
//}

/// <summary>
/// loads all files in the second array, expecting texture coordinates in them,
/// then loads the files in the first array, instances the geometry of them and
/// copies the texture coordinates from geometries in the second array to the appropriate
/// geometries in the loaded file
/// </summary>
//void applyTexturesFromFilesToOtherFiles(std::vector<std::string>& fileNames, std::vector<std::string>& otherFileNames) {
//    std::vector<Scene* > scenes;
//    for (auto fileName : otherFileNames) {
//        Scene* scene = new Scene(fileName);
//        if (!scene->loadSceneFromVrmlFile(fileName)) {
//            std::cout << "Error loading, could not find  " << fileName << std::endl;
//        }
//        std::cout << "Loaded  " << fileName << std::endl;
//        scene->findAndUseIdenticalGeometry();
//        scenes.push_back(scene);
//    }
//
//    for (auto fileName : fileNames) {
//        std::cout << "Automatic mode: Parsing file: " << fileName << std::endl;
//        Scene* scene = new Scene(fileName.substr(10));
//        if (!scene->loadSceneFromVrmlFile(fileName)) std::cout << "error loading " << fileName << std::endl;
//        std::cout << "Finding identical geometry of file: " << scene->name << std::endl;
//        scene->findAndUseIdenticalGeometry();
//        std::cout << "Finding same geometry in other files: " << scene->name << std::endl;
//        scene->findAndUseSameObjectsFromOtherScenesInThisScene(scenes);
//        std::cout << "Saving file: " << scene->name << std::endl;
//        scene->saveSceneToVrmlFile("VRML2/" + scene->name);
//        delete scene;
//    }
//}

/// <summary>
/// just loads and saves the files, for testing of the parser and exporter
/// </summary>
//void loadAndSave(std::vector<std::string>& fileNames) {
//    std::vector<Scene* > scenes;
//    for (auto fileName : fileNames) {
//        std::cout << "Automatic mode: Parsing file: " << fileName << std::endl;
//        Scene* scene = new Scene(fileName);
//        if (!scene->loadSceneFromVrmlFile(fileName)) std::cout << "error loading " << fileName << std::endl;
//        scenes.push_back(scene);
//        //std::cout << "Saving file: " << scene->name << std::endl;
//        //scene->saveSceneToVrmlFile(scene->name);
//        //delete scene;
//    }
//}

//void combineScenes(std::vector<std::string>& fileNames) {
//    SceneManager sm;
//    for (auto s : fileNames) {
//        if(sm.loadScene(s))
//            std::cout << "Loaded " << s << std::endl;
//    }
//    sm.combineAllScenesIntoOne();
//    sm.combinedScene->findAndUseIdenticalGeometry();
//    sm.combinedScene->writeOutGeometries();
//    sm.combinedScene->saveSceneToVrmlFile("out.wrl");
//}


#endif






#ifdef AUTOMATIC
    automaticMode("automaticFileTest.txt");

    std::vector<std::string> fileNames;
    {
        //fileNames.push_back("VRMLUntex/0_fl_furniture.WRL");
        //fileNames.push_back("VRMLUntex/1_fl_furniture.WRL");
        //fileNames.push_back("VRMLUntex/2_fl_furniture_E.WRL");
        //fileNames.push_back("VRMLUntex/2_fl_furniture_N.WRL");
        //fileNames.push_back("VRML2/2_fl_furniture_S.WRL");
        //fileNames.push_back("VRML2/2_fl_furniture_W.WRL");
        //fileNames.push_back("VRML2/3_fl_furniture_E.WRL");
        //fileNames.push_back("VRML2/3_fl_furniture_N.WRL");
        //fileNames.push_back("VRML2/3_fl_furniture_S.WRL");
        //fileNames.push_back("VRML2/3_fl_furniture_W.WRL");
        //fileNames.push_back("VRML2/4_fl_furniture_E.WRL");
        //fileNames.push_back("VRML2/4_fl_furniture_N.WRL");
        //fileNames.push_back("VRML2/4_fl_furniture_S.WRL");
        //fileNames.push_back("VRML2/4_fl_furniture_W.WRL");
        //fileNames.push_back("VRML2/5_fl_furniture_E.WRL");
        //fileNames.push_back("VRML2/5_fl_furniture_N.WRL");
        //fileNames.push_back("VRML2/5_fl_furniture_S.WRL");
        //fileNames.push_back("VRML2/5_fl_furniture_W.WRL");
        //fileNames.push_back("VRML2/6_fl_furniture_E.WRL");
        //fileNames.push_back("VRML2/6_fl_furniture_N.WRL");
        //fileNames.push_back("VRML2/6_fl_furniture_S.WRL");
        //fileNames.push_back("VRML2/6_fl_furniture_W.WRL");
        //fileNames.push_back("VRML2/0-6_equipment.WRL");
        //fileNames.push_back("VRML2/0-6_furniture_halls.WRL");
        //fileNames.push_back("VRML2/0-6_heating.WRL");
        //fileNames.push_back("VRML2/0-6_lamps.WRL");
        //fileNames.push_back("VRML2/0-6_Lights.WRL");
        //fileNames.push_back("VRML2/0-6_stairs_fences.WRL");
        //fileNames.push_back("VRML2/0-7_construction.WRL");
        //fileNames.push_back("VRML2/0-7_geometry.WRL");
        //fileNames.push_back("VRML2/0-7_tin_plates.WRL");
        //fileNames.push_back("VRML2/0-7_ventilation.WRL");
        //fileNames.push_back("Door/U-6_doors_F0_new.WRL");
        //fileNames.push_back("Door/U-6_doors_F1_new.WRL");
        //fileNames.push_back("Door/U-6_doors_F2_new.WRL");
        //fileNames.push_back("Door/U-6_doors_F3_new.WRL");
        //fileNames.push_back("Door/U-6_doors_F4_new.WRL");
        //fileNames.push_back("Door/U-6_doors_F5_new.WRL");
        //fileNames.push_back("Door/U-6_doors_F6_new.WRL");
        //fileNames.push_back("Door/U-6_doors_FU_new.WRL");
        //fileNames.push_back("Door/U-6_doors_Stair_Mid_new.WRL");
        //fileNames.push_back("Door/U-6_doors_Stair_Side2_new.WRL");
        //fileNames.push_back("Door/U-6_doors_Stair_Side1_new.WRL");
        //fileNames.push_back("Door/U-6_windows_0F_new.WRL");
        //fileNames.push_back("Door/U-6_windows_1F_new.WRL");
        //fileNames.push_back("Door/U-6_windows_2F_new.WRL");
        //fileNames.push_back("Door/U-6_windows_3F_new.WRL");
        //fileNames.push_back("Door/U-6_windows_4F_new.WRL");
        //fileNames.push_back("Door/U-6_windows_5F_new.WRL");
        //fileNames.push_back("Door/U-6_windows_6F_new.WRL");
        //fileNames.push_back("Door/U-6_windows_46_new.WRL");
        //fileNames.push_back("Door/U-6_windows_shutters_0F.WRL");
        //fileNames.push_back("Door/U-6_windows_shutters_1F.WRL");
        //fileNames.push_back("Door/U-6_windows_shutters_2F.WRL");
        //fileNames.push_back("Door/U-6_windows_shutters_3F.WRL");
        //fileNames.push_back("Door/U-6_windows_shutters_4F.WRL");
        //fileNames.push_back("Door/U-6_windows_shutters_5F.WRL");
        //fileNames.push_back("Door/U-6_windows_shutters_6F.WRL");
        //fileNames.push_back("Door/U-6_windows_shutters_UF.WRL");
        //fileNames.push_back("Door/U-6_windows_Stairway01_new.WRL");
        //fileNames.push_back("Door/U-6_windows_Stairway02_new.WRL");
        //fileNames.push_back("Door/U-6_windows_UF_new.WRL");
    }
    std::vector<std::string> fileNamesTwo;
    {
        fileNamesTwo.push_back("textured/basket_01.wrl");
        fileNamesTwo.push_back("textured/basket_03.wrl");
        fileNamesTwo.push_back("textured/bench.wrl");
        fileNamesTwo.push_back("textured/blackboard_01.wrl");
        fileNamesTwo.push_back("textured/blackboard_02.wrl");
        fileNamesTwo.push_back("textured/blackboard_03.wrl");
        fileNamesTwo.push_back("textured/caffe_set.wrl");
        fileNamesTwo.push_back("textured/case_type_R-01.wrl");
        fileNamesTwo.push_back("textured/case_type_R-02.wrl");
        fileNamesTwo.push_back("textured/CaseType1.wrl");
        fileNamesTwo.push_back("textured/CaseType21.wrl");
        fileNamesTwo.push_back("textured/CaseTypeXX.wrl");
        fileNamesTwo.push_back("textured/chair_01new.wrl");
        fileNamesTwo.push_back("textured/chair_02new.wrl");
        fileNamesTwo.push_back("textured/chair_03new.wrl");
        fileNamesTwo.push_back("textured/chair_03oldB.wrl");
        fileNamesTwo.push_back("textured/circle_table_low.wrl");
        fileNamesTwo.push_back("textured/FirstAidKit.wrl");
        fileNamesTwo.push_back("textured/netPlug.wrl");
        fileNamesTwo.push_back("textured/powerPlug.wrl");
        fileNamesTwo.push_back("textured/rack.wrl");
        fileNamesTwo.push_back("textured/roundTable01.wrl");
        fileNamesTwo.push_back("textured/Sofa01.wrl");
        fileNamesTwo.push_back("textured/Sofa02.wrl");
        fileNamesTwo.push_back("textured/Sofa03.wrl");
        fileNamesTwo.push_back("textured/table_03low.wrl");
        fileNamesTwo.push_back("textured/table_06.wrl");
        fileNamesTwo.push_back("textured/table_08.wrl");
        fileNamesTwo.push_back("textured/Table02.wrl");
        fileNamesTwo.push_back("textured/Table03.wrl");
        fileNamesTwo.push_back("textured/Table04.wrl");
        fileNamesTwo.push_back("textured/Table04_B.wrl");
        fileNamesTwo.push_back("textured/Table05.wrl");
        fileNamesTwo.push_back("textured/Table07.wrl");
        fileNamesTwo.push_back("textured/Table30.wrl");
        fileNamesTwo.push_back("textured/tableCornerLlongRshort.wrl");
        fileNamesTwo.push_back("textured/tableCornerLshortRlong.wrl");
        fileNamesTwo.push_back("textured/tableLength80.wrl");
        fileNamesTwo.push_back("textured/tableM4.wrl");
        fileNamesTwo.push_back("textured/tables3X_room211.wrl");
        fileNamesTwo.push_back("textured/tableSTM3a.wrl");
        fileNamesTwo.push_back("textured/tableWhite309A.wrl");
        fileNamesTwo.push_back("textured/tableWide.wrl");
        fileNamesTwo.push_back("textured/tableWideB.wrl");
        fileNamesTwo.push_back("textured/TreeBigGroundFloor.wrl");
        fileNamesTwo.push_back("textured/typ40a.wrl");
    }



    //instanceFiles(fileNames, "VRMLUntex");
    //applyTexturesFromFilesToOtherFiles(fileNames, fileNamesTwo);
    //loadAndSave(fileNames);
    //combineScenes(fileNames);

#endif