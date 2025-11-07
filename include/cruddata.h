#pragma once
#include <iostream>
#include <vector>

struct CoinData{
    double active, amount, last_Coin_Price, profit, target_Amount, target_Coin_Price; 
    std::string coin, last_Update;
};

struct HistoryData{
    int id;
    double amount, coin_Price, coin_Amount, fee_Buy, fee_Sell, target_Percentage, target_Coin_Value;
    std::string coin, operation_Date, operation_Type;
};

extern std::string operationFileName;
extern std::string walletFileName;
extern std::vector<std::string> headerCsv;
extern std::vector<HistoryData> operationData;
extern std::vector<CoinData> walletData;

//prototyping

void readFromCsv();
void readFromJson();
void listCoinToChange(int changeOpetion);
void deleteCoin(std::string coinToChange);
void alterCoin(std::string coinToChange);
void insertCoin(std::string newCoin);
void updateToJson(const std::vector<CoinData>& walletData);
void updateToCsv(const std::vector<HistoryData>& operationData, const std::vector<std::string>& headerCsv);
