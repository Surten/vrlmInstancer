/**
@file error.h
Hlavickovy soubor definujici chybove hlasky.

@author Copyright (C) 2008 Jiri Drahokoupil
*/

#ifndef _ERROR_H_
#define _ERROR_H_

#include <iostream>
#include <string>


const std::string errorCaption = "Error";

/// Fce vypisujici chybova hlaseni
/**
 @param errorText Vystupni chybove hlaseni(typ wxString)
*/
void Error(std::string errorText);

/// Fce vypisujici chybova hlaseni
/**
 @param errorText Vystupni chybove hlaseni(typ char*)
*/
void Error(char* errorText);

/// Fce vypisujici chybove hlaseni s popisem nepovoleneho symbolu
/**
 @param errorText Vystupni chybove hlaseni(typ wxString)
 @param input Nepovoleny znak na vstupu
*/
void Error(std::string errorText, char input);

/// Fce vypisujici chybove hlaseni s popisem nepovoleneho symbolu
/**
 @param errorText Vystupni chybove hlaseni(typ char*)
 @param input Nepovoleny znak na vstupu
*/
void Error(char* errorText, char input);

/// Fce vypisujici navic chyby pri srovnani
/**
 @param errorText Vystupni chybove hlaseni(wxString)
 @param symbolType Druh ocekavaneho symbolu
*/
void Error(std::string errorText, std::string symbolType);

/// Fce vypisujici navic chyby pri srovnani
/**
 @param errorText Vystupni chybove hlaseni(char*)
 @param symbolType Druh ocekavaneho symbolu(char*)
*/
void Error(char* errorText, char* symbolType);

#endif
