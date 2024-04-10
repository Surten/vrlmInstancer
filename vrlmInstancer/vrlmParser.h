#pragma once

#include <fstream>
#include <string>

void loadFile(const char* vrlmFileName);

void skipComments();

void readSymbol(/*bool & wasNumber*/);

void parseFile();

void parseDEF();

void parseTransformNode(std::string name);

void parseChildren(std::string name);


