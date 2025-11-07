#pragma once
#include <iostream>



//Max size with consistency SCREEN_SIZE = 134;
const char TOP_RIGHT = 187;
const char TOP_LEFT = 201;
const char HORIZONTAL = 205;
const char VERTICAL = 186;
const char JUNK_LEFT = 204;
const char JUNK_RIGHT = 185;
const char LOWER_LEFT = 200;
const char LOWER_RIGHT = 188;

const std::string marginLeft = "   ";

void menu();
void screenClear();
void pause();
void printUniqueCoins();
void printPreFooter(const int screenSize);
void printSeparator(const int screenSize);
void printCsvHeader(const int screenSize);
void handleMenuInputError(const std::string& text);
void printHeader(const std::string& title, const int screenSize);
void printFormattedLine(const std::string& text, const int screenSize);
void printEmptyLine(int count, const int screenSize);
void printFooter(const std::string& footer, const int screenSize);