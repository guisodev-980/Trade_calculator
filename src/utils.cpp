#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <ctime>
#include <set>
#include "printers.h"
#include "cruddata.h"

int getLastOperationId(){
    readFromCsv();
    int lastId = 0;
    if(operationData.size() < 1 ) return 1;
    lastId = operationData.back().id;
    try{
        return lastId + 1;
    }
    catch(...){
        std::cerr << marginLeft << "Erro ao ler o último ID. Operação abortada!\n";
        pause();
        std::exit(EXIT_FAILURE);
        return -1;
    }
}

std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << localTime->tm_mday << "/"
        << std::setw(2) << std::setfill('0') << (localTime->tm_mon + 1) << "/"
        << (localTime->tm_year + 1900);

    return oss.str();
}

std::vector<std::string> existCoins(){
    readFromCsv();
    std::set<std::string> filterCoins;
        for(size_t i = 0; i < walletData.size(); ++i){
            filterCoins.insert(walletData[i].coin);
        }
    std::vector<std::string> resultExist(filterCoins.begin(), filterCoins.end());
        return resultExist;
}

bool isValidDate(int day, int month, int year){
    std::tm timeStruct{};
    timeStruct.tm_mday = day;
    timeStruct.tm_mon = month - 1;
    timeStruct.tm_year = year - 1900;
    
    std::time_t temp = std::mktime(&timeStruct);
    if (temp == -1) return false;

    return(timeStruct.tm_mday == day&&
            timeStruct.tm_mon == month -1 &&
            timeStruct.tm_year == year -1900);
}
std::string getValidDate(const std::string& date){
    std::string messageDateError = marginLeft + "Formato ou Data Invalidos";
    if(!(date == "0")){  
        int day, month, year;
        std::string day_str, month_str, year_str, result;
    
        std::stringstream ss(date);
        if(!(std::getline(ss, day_str, '/')&&
            std::getline(ss, month_str, '/')&&
            std::getline(ss, year_str)))
            std::cerr << messageDateError;
            pause();
            return "";
        try{
        day = std::stoi(day_str);
        month = std::stoi(month_str);
        year = std::stoi(year_str);

        }catch(...){
            std::cerr << messageDateError;
             pause();
            return "";
        }
        if(year < 100) year+=2000;

            
        if(!isValidDate(day, month, year)){
            std::cerr << messageDateError;
            pause(); 
            return "";}

        std::ostringstream formatted;
        formatted   << (day < 10 ? "0" : "") << day << "/"
                    << (month < 10 ? "0" : "") << month << "/"
                    << year;
    
        return formatted.str();
    }
    return getCurrentDate();
}