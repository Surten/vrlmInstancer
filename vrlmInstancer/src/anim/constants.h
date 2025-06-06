/**
@file constants.h
Deklarace dulezitych konstant a promennych.

@author Copyright (C) 2008 Jiri Drahokoupil, modified by Ondrej Linda 2009
*/

#ifndef __CONSTANTS__
#define __CONSTANTS__

// IDcka oken
#define ID_MAIN_FRAME 2001
#define ID_PANEL 2002

// IDcka nabidek, buttonu apod..
#define ID_DEFAULT -1 // Default
#define ID_FILE_EXIT 1000
#define ID_HELP_ABOUT 1001
#define ID_MAIN_OK 1002
#define ID_ABOUT_APPNAME 1003
#define ID_ABOUT_COPYRIGHT 1004
#define ID_ABOUT_ICON 1005
#define ID_ABOUT_VERSION 1006
#define ID_NEW_PROJECT 1007
#define ID_OPEN_PROJECT 1008
#define ID_OPEN_PROJECT_LIST 10081
#define ID_SAVE_PROJECT 1009
#define ID_SAVEAS_PROJECT 1010
#define ID_OPEN_VRML_FILE 1011
#define ID_CREATE_VRML_FILE 1012
#define ID_CREATE_ANIMATED_VRML_FILE 10121
#define ID_NEW_INDIVIDUAL_PBRT 1013
#define ID_NEW_ANIMATED_PBRT 10131
#define ID_NEW_SEPARATED_PBRT 1014
#define ID_ABOUT_COMPANY 1015
#define ID_BUTTON_NEW_VRML 1016
#define ID_BUTTON_NEW_PBRT 1017
#define ID_BUTTON_LOAD_VRML 1018
#define ID_BUTTON_LOAD_MAT 1019
#define ID_BUTTON_EXIT 1020
#define ID_NOTEBOOKCTRL 10201
#define ID_LISTCTRL 1021
#define ID_LISTCAMERACTRL 10210
#define ID_LISTDOORCTRL 10211
#define ID_LISTWINDOWCTRL 10212
#define ID_LISTSHUTTERCTRL 10213
#define ID_CONSOLE 1022
#define ID_BUTTON_RENDER 1023
#define ID_RENDER_RESET 1024
#define ID_RENDER_OK 1025
#define ID_RENDER_CANCEL 1026
#define ID_RENDER_STATIC1 1027
#define ID_RENDER_STATIC2 1028
//#define ID_RENDER_COMBO 1029
#define ID_RENDER_INTEGRATOR_NAME 1030
#define ID_RENDER_INTEGRATOR_REC 1031
#define ID_RENDER_INTEGRATOR_SETS 1032
#define ID_RENDER_INTEGRATOR_DISTANCE 1033
#define ID_RENDER_INTEGRATOR_THRESHOLD 1034
#define ID_RENDER_INTEGRATOR_INDIRECT 1035
#define ID_RENDER_INTEGRATOR_PATHS 1036
#define ID_RENDER_SAMPLER_NAME 1037
#define ID_RENDER_SAMPLER_PIXSAMPLES 1038
#define ID_RENDER_SAMPLER_JITTER 1039
#define ID_RENDER_SAMPLER_XSAMPL 1040
#define ID_RENDER_SAMPLER_YSAMPL 1041
#define ID_RENDER_FILTER_NAME 1042
#define ID_RENDER_FILTER_XWIDTH 1043
#define ID_RENDER_FILTER_YWIDTH 1044
#define ID_RENDER_FILTER_B 1045
#define ID_RENDER_FILTER_C 1046
#define ID_RENDER_FILTER_ALPHA 1047
#define ID_RENDER_TEXTURE_NAME 1048
#define ID_RENDER_TEXTURE_PATH 1049
#define ID_RENDER_XRES 1050
#define ID_RENDER_YRES 1051
#define ID_RENDER_RESOLUTION_BOX 1052
#define ID_RENDER_SPINX 1053
#define ID_RENDER_SPINY 1054
#define ID_RENDER_BUTTON1 1055
#define ID_RENDER_BUTTON2 1056
#define ID_RENDER_STATIC1_INTEG 1057
#define ID_RENDER_STATIC1_SAMPL 1058
#define ID_RENDER_STATIC1_FILT 1059
#define ID_RENDER_LEFT_BOX 1060
#define ID_RENDER_MIDDLE_BOX 1061
#define ID_RENDER_RIGHT_BOX 1062
#define ID_RENDER_SPIN_DEPTH 1063
#define ID_RENDER_SPIN_NPFL 1064
#define ID_RENDER_SPIN_NLSC 1065
#define ID_RENDER_SPIN_MSDL 1066
#define ID_RENDER_SPIN_RR 1067
#define ID_RENDER_SPIN_IS 1068
#define ID_RENDER_INTEGRATOR_COMBO 1069
#define ID_RENDER_SAMPLER_COMBO 1070
#define ID_RENDER_FILTER_COMBO 1071
#define ID_RENDER_SPIN_PSAMPLES 1072
#define ID_RENDER_SPIN_XSAMPLES 1073
#define ID_RENDER_SPIN_YSAMPLES 1074
#define ID_RENDER_JITTER 1075
#define ID_RENDER_SPIN_XWIDTH 1076
#define ID_RENDER_SPIN_YWIDTH 1077
#define ID_RENDER_SPIN_ALPHA 1078
#define ID_RENDER_SPIN_B 1079
#define ID_RENDER_SPIN_C 1080
#define ID_RENDER_TEXT_FILE 1081
#define ID_RENDER_TEXT_PATH 1082

#define ID_CAMERA_ONOFF_CHB 1101
#define ID_CAMERA_ANIMATE_CHB 1102
#define ID_CAMERA_ANIMATE_BOX 1103
#define ID_CAMERA_ANIMATE_LIST 11031
#define ID_CAMERA_PBRT_SELECT 11032
#define ID_DOOR_ANIMATE_LIST 1104
#define ID_DOOR_ANIMATE_CHB 1105
#define ID_SHUTTER_ANIMATE_LIST 1106
#define ID_SHUTTER_ANIMATE_CHB 1107
#define ID_WINDOW_ANIMATE_LIST 1108
#define ID_WINDOW_ANIMATE_CHB 1109
#define ID_WINDOW_TYPE_RDB 1110
#define ID_LIGHT_ANIMATE_CHB 1111
#define ID_LIGHT_ANIMATE_LIST 1112
#define ID_GLOBAL_HANDLE_CHB 1113


#define ID_PBRT_DIALOG 1201
#define ID_PBRT_BUTTON1 1202
#define ID_PBRT_BUTTON2 1203
#define ID_PBRT_BUTTON3 1204
#define ID_PBRT_BUTTON4 1205
#define ID_PBRT_BUTTON5 12051
#define ID_PBRT_BUTTON6 12052
#define ID_PBRT_BUTTON7 12053
#define ID_PBRT_BUTTON8 12054
#define ID_PBRT_BUTTON9 12055
#define ID_PBRT_BUTTON10 12056
#define ID_PBRT_BUTTON11 12057
#define ID_PBRT_FILENAME 1206
#define ID_PBRT_DIRECTORY 1207
#define ID_PBRT_BUTTON_DIR 1208
#define ID_PBRT_FILENAME2 1209
#define ID_PBRT_FILENAME3 1210
#define ID_PBRT_RADIO 1211
#define ID_BUTTON_ANIMATE_VRML 1212
#define ID_BUTTON_ANIMATE_PBRT 1213

#define ID_VRML_DIALOG 121
#define ID_VRML_BUTTON1 122
#define ID_VRML_BUTTON2 123
#define ID_VRML_BUTTON3 124
#define ID_VRML_BUTTON4 125
#define ID_VRML_BUTTON5 126
#define ID_VRML_BUTTON6 127
#define ID_VRML_BUTTON7 128
#define ID_VRML_BUTTON8 129
#define ID_VRML_BUTTON9 130
#define ID_VRML_FILENAME 131
#define ID_VRML_DIRECTORY 132
#define ID_VRML_BUTTON_DIR 133
#define ID_VRML_FILENAME2 134
#define ID_VRML_FILENAME3 135

#define ID_BUTTON_ON_OFF_FLOOR 1301
#define ID_BUTTON_ON_OFF_SECTION 1302
#define ID_BUTTON_ON_OFF_LIGHTS 1303
#define ID_BUTTON_CREATE_EXR 1304
#define ID_BUTTON_ASSIGN_EXR 1305
#define ID_BUTTON_TEMPERATURE 1306
#define ID_BUTTON_FLUX 1307
#define ID_BUTTON_ANIMATE_SET 1308

#define ID_EXR_DIALOG 1401
#define ID_EXR_TEXT1 1402
#define ID_EXR_TEXT2 1403
#define ID_EXR_BUTTON1 1404
#define ID_EXR_BUTTON2 1405
#define ID_EXR_TCTRL 1406
#define ID_EXR_CREATE 1407

#define ID_OOF_DIALOG 1501
#define ID_OOF_OK 1502
#define ID_OOF_RADIO 1503
#define ID_OOF_SPIN1 1504
#define ID_OOF_TEXT2 1505
#define ID_OOF_SPIN2 1506

#define ID_OOALL_DIALOG 1601
#define ID_ALL_OK 1602
#define ID_ALL_RADIO 1603

#define ID_ASSIGN_EXR_DLG 1701
#define ID_ASSIGN_OK 1702
#define ID_ASSIGN_TEXT1 1703
#define ID_ASSIGN_SPIN1 1704
#define ID_ASSIGN_AS 1705

#define ID_SET_COLOR_DLG 1801
#define ID_COLOR_SPIN1 1802
#define ID_COLOR_RADIO 1803
#define ID_COLOR_CTRL_R 1804
#define ID_COLOR_CTRL_G 1805
#define ID_COLOR_CTRL_B 1806
#define ID_COLOR_TEMP 1807
#define ID_COLOR_OK 1808

#define ID_INTENSITY_DLG 1901
#define ID_INTENSITY_OK 1902
#define ID_INTENSITY_SPIN1 1903
#define ID_INTENSITY_CTRLF 1904

#define ID_SINGLE_DLG 2001
#define ID_SINGLE_OK 2002
#define ID_SINGLE_OK_SECTION 20021
#define ID_SINGLE_OK_FLOOR 20022
#define ID_SINGLE_OK_ALL 20023
#define ID_SINGLE_ADD 20024
#define ID_SINGLE_RADIO 2003
#define ID_SINGLE_TYPE 2004
#define ID_SINGLE_SPIN0 2005
#define ID_SINGLE_SPIN1 2006
#define ID_SINGLE_SPIN2 2007
#define ID_SINGLE_R 2008
#define ID_SINGLE_G 2009
#define ID_SINGLE_B 2010
#define ID_SINGLE_T 2011
#define ID_SINGLE_SOURCE 2012
#define ID_SINGLE_COMPUTE 2013
#define ID_SINGLE_CHECK 2014
#define ID_SINGLE_I 2015
#define ID_SINGLE_O 2016
#define ID_BUTTON_SHUTTER_ANIMATE 2017
#define ID_BUTTON_DOOR_ANIMATE 2018
#define ID_BUTTON_WINDOW_ANIMATE 2019

// code words for recognizing objects from the node names
#define CODE_DOOR_FLOOR "DOOR_FLOOR"
#define CODE_DOOR "DOOR_F"
#define CODE_WINDOW "WINDOW"
#define CODE_DOOR_MOVE "DOOR_MOVE"
#define CODE_DOOR_AXIS "DOOR_JOINTS"
#define CODE_DOOR_HANDLES "DOOR_HANDLES"
#define CODE_WINDOW_SHUTTER "WINDOW_SHUTTERS"
#define CODE_SHUTTERS_MOVE "SHUTTERS_MOVE"
#define CODE_SHUTTER "SHUTTER_F"
#define CODE_SHUTTER_ROTATE "SHUTTER_ROTATE"
#define CODE_SHUTTER_TOP "SHUTTERS_TOP"
#define CODE_WINDOWS "WINDOWS"
#define CODE_WINDOW_OPEN "WINDOW_F"
#define CODE_WINDOW_MOVE "WINDOW_MOVE"
#define CODE_WINDOW_JOINTS "WINDOW_JOINTS"
#define CODE_WINDOW_HANDLES "WINDOW_HANDLE"

// Offset for translating the shutters of type 2
#define SHIFT_SHUTTER_TYPE_2 0.6
// Offset for translating the shutters of type 3
#define SHIFT_SHUTTER_TYPE_3 1.2
// Offset for translating the shutters of type 4
#define SHIFT_SHUTTER_TYPE_4 0.6
// Offset for automatic rotation of doors without joints
#define CENTER_DOOR_NO_JOINTS 0.5
// Offset for automatic rotation of doors without joints type 6
#define CENTER_DOOR_NO_JOINTS_4 0.4
// Offset for automatic rotation of doors without joints type 6
#define CENTER_DOOR_NO_JOINTS_12 0.6
// Offset for automatic rotation of narrow doors without joints
#define CENTER_DOOR_NO_JOINTS_NARROW 0.25
// Offset for automatic rotation of door handles
#define CENTER_DOOR_HANDLES 0.07
// Offset for automatic rotation of door handles
#define CENTER_DOOR_HANDLES3 0.06
// Offset for automatic rotation of door handles
#define CENTER_DOOR_HANDLES2 0.1
// Offset for automatic rotation of window handles
#define CENTER_WINDOW_HANDLES 0.05
// Offset for automatic vertical opening of windows
#define CENTER_WINDOW_VERTICAL 0.85
// Offset for shifting the center of rotation of window type 5 and 6
#define CENTER_WINDOW_TYPE_56 0.1

/// Konverze z radianu na stupne
#define RAD_TO_DEG(angle)   ((angle)*57.29577951308f)

/// degress to radians
#define DEG_TO_RAD(angle)   ((angle) / 57.29577951308f)


;//Pro potreby linkeru

// Maximalni delka radku
const short MaxDelkaRadku = 512;
// Max delka identifikatoru
const short MaxLenIdent = 75;

#define NOPE 4294967295

#endif
