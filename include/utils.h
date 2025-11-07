#pragma once
#include <iostream>
#include <vector>
#include <iomanip>

// prototyping

int getLastOperationId();
std::string getCurrentDate();
std::vector<std::string> existCoins();
bool isValidDate(int day, int month, int year);
std::string getValidDate(const std::string& date);

template<typename T>
T getValidInput(std::string label, std::string hint){
    T value;
    while(true){
        std::cout << marginLeft << " " << label;
        if(!hint.empty()) std::cout << "" << hint;
        std::cout << marginLeft << ": ";
        std::cin >> value;
        
        if(std::cin.fail()){
            handleMenuInputError("Entrada Invalida");
            continue;        
        }
    return value;
    }
}