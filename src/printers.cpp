#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include <set>
#include <iomanip>
#include "printers.h"
#include "cruddata.h"
#include "utils.h"


void screenClear(){
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void handleMenuInputError(const std::string& text){
    screenClear();
    std::cerr <<marginLeft << " " << text << "\n\n";
    std::cin.clear();
    std::numeric_limits<std::streamsize>::max(), '\n';
    pause();
}

void printUniqueCoins(){
    std::vector<std::string> uniqueCoins = existCoins();
    std::cout << marginLeft << "Moedas Existentes:" << std::endl;
    int count =0;
    for(const auto& coin : uniqueCoins){
                std::cout << marginLeft << coin;
                if(++count < uniqueCoins.size()) std::cout << " | ";
            }   
}

void pause(){
    std::cout << std::endl << marginLeft << " " << "Tecle Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void printHeader(const std::string& title, const int screenSize){
    int sideHeader = (screenSize-title.length())/2;
    std::cout   << marginLeft << TOP_LEFT << std::string(screenSize, HORIZONTAL) << TOP_RIGHT << std::endl
                << marginLeft << VERTICAL << std::string((sideHeader+1), ' ') << title << std::string(sideHeader, ' ') << VERTICAL << std::endl
                << marginLeft << JUNK_LEFT << std::string(screenSize, HORIZONTAL) << JUNK_RIGHT << std::endl;
}

void printEmptyLine(int count, const int screenSize){
    for(int i = 0; i < count; ++i){
        std::cout << marginLeft << VERTICAL << std::string(screenSize, ' ') << VERTICAL << std::endl;
    }
}

void printSeparator(const int screenSize){
    std::cout << marginLeft << VERTICAL << " " << std::string((screenSize-2), '-') << " " << VERTICAL << std::endl;
}

void printFormattedLine(const std::string& text, const int screenSize){
    int lineSize = screenSize-text.length();
    std::cout << marginLeft << VERTICAL << text << std::string(lineSize, ' ')<< VERTICAL << std::endl;
}

void printPreFooter(const int screenSize){
    std::cout << marginLeft << JUNK_LEFT << std::string(screenSize, HORIZONTAL) << JUNK_RIGHT << std::endl;
}
void printFooter(const std::string& footer, const int screenSize){
    if(!footer.empty()){
        int sideFooter = screenSize-footer.length()-1;
        std::cout << marginLeft << VERTICAL << std::string(sideFooter, ' ') << footer << " " << VERTICAL << std::endl;
    }
    std::cout << marginLeft << LOWER_LEFT << std::string(screenSize, HORIZONTAL) << LOWER_RIGHT << std::endl;
}

void printCsvHeader(const int screenSize){
    int colWith[] = {4, 5, 10, 6, 10, 14, 12, 7, 7, 7, 18};
    int totalWith = 0;
    for(int w : colWith) totalWith +=(w+3);
    std::ostringstream header_str;
    for (size_t i = 0; i < headerCsv.size(); ++i) {
        header_str << std::right << std::setw(colWith[i]) << headerCsv[i];
        if (i < headerCsv.size() - 1) header_str << " | ";
    }
    printFormattedLine(header_str.str(), screenSize);
    header_str.str("");
    header_str.clear();
    printSeparator(screenSize);
}
