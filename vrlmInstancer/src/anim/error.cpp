/**
@file error.cpp
Soubor s definicemi chybovych dialogu.

@author Copyright (C) 2008 Jiri Drahokoupil
*/


#include "error.h"

void Error(std::string errorText)
{
	std::cout << "Error: " << errorText << errorCaption << std::endl;
}

void Error(char* errorText)
{
	std::cout << "Error: " << errorText << std::endl;
}

void Error(std::string errorText, std::string symbolType)
{
	std::cout << "Error: " << errorText << " Uncorrect input symbol: " << symbolType << std::endl;
}

void Error(char* errorText, char* symbolType)
{
	std::cout << "Error: " << std::string(errorText) << " Uncorrect input symbol: " << std::string(symbolType) << std::endl;
}

void Error(std::string errorText, char input)
{
	char text[1] = { input };
	std::string in(text);
	std::cout << "Error: " << errorText << " '" << input << "' " << std::endl;
}

void Error(char* errorText, char input)
{
	std::cout << "Error: " << std::string(errorText) << " '" << input << "'" << std::endl;
}
