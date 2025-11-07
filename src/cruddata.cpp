#include "cruddata.h"
#include "printers.h"
#include "utils.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

std::string operationFileName = "../data/operation.csv";
std::string walletFileName = "../data/wallet.json";
std::vector<std::string> headerCsv;
std::vector<HistoryData> operationData;
std::vector<CoinData> walletData;


void insertCoin(std::string newCoin){
    readFromJson();

    screenClear();
    printHeader("Cadastro de Moedas ", 100);
    std::ostringstream line_str;
    int count = 0;
    for(const auto & coin : walletData){
        line_str << coin.coin;
        if(++count < walletData.size()) line_str << " | ";
        if(line_str.str().length() >= 85){
            printFormattedLine(line_str.str(),100);
            line_str.str("");
            line_str.clear();
        }
    }
    if(!line_str.str().empty()){
        printFormattedLine(line_str.str(),100);
    }
    printEmptyLine(1, 100);
    printFooter("[!] Digite 0 para Voltar", 100);
    line_str.str("");
    line_str.clear();
    
    if(newCoin.empty()){
        std::cout << marginLeft << " Nome da Moeda: \n";
        std::cout << marginLeft << " > ";
        std::cin >> newCoin;
        for (auto & c: newCoin) c = toupper(c);
            if(newCoin == "0") return;

        for (const auto& coin : walletData){
            if(coin.coin == newCoin){
                std::cout << marginLeft << " " << newCoin << " ja existe!" << std::endl;
                pause();
                return;
            }
        }
    }
    
    std::cout << marginLeft << " " << "Preparando para inserir... " << newCoin << std::endl;
    CoinData newEntry;
    newEntry.coin = newCoin;
    newEntry.amount = 0.0;
    newEntry.active = 0.0;
    newEntry.profit = 0.0;
    newEntry.target_Amount = 0.0;
    newEntry.target_Coin_Price = 0.0;
    newEntry.last_Update = getCurrentDate();
    newEntry.last_Coin_Price = 0.0;

    walletData.push_back(newEntry);
    std::cout << marginLeft << " "<< newCoin <<" Inserida com Sucesso! " << std::endl;
    updateToJson(walletData);
    pause();
    
}
void listCoinToChange(int changeOption){
    //int changeOption = 1 - Alter Coin
    //int changeOption = 2 - Delete Coin
    std::string optionToDo;
    std::string coinToChange;
    readFromCsv();
    readFromJson();
    if(changeOption == 1) optionToDo = "Alterar";
    else if(changeOption == 2) optionToDo = "Excluir";
    
    while(true){
        screenClear();
        printHeader("Moedas Cadastradas ", 100);
        std::ostringstream line_str;
        int count = 0;
        for(const auto & coin : walletData){
            line_str << coin.coin;
            if(++count < walletData.size()) line_str << " | ";
        }
        printFormattedLine(line_str.str(), 100);
        printEmptyLine(1, 100);
        printFooter("[!] Digite 0 para Voltar", 100);
        line_str.str("");
        line_str.clear();
        std::cout << marginLeft << " Digite o Nome de Uma Moeda Existente ou 0 para voltar: \n";
        std::cout << marginLeft << " > ";
        std::cin >> coinToChange;
        if(coinToChange == "0") return;
        for(auto & c: coinToChange) c = toupper(c);

        auto it = std::find_if(walletData.begin(), walletData.end(),
                [&](const CoinData& coin){
                    return coin.coin == coinToChange;
                });

        if(it == walletData.end()){
            std::cout << "\n " << marginLeft << coinToChange << " Nao foi Encontrada!\n";
            pause();
            continue;
        }
        if(changeOption == 2 && (it->amount > 0.0 || it->active > 0.0)){
            std::cout   << marginLeft << " [!] A moeda " << coinToChange << " nao pode ser excluida porque possui "
                        << it->amount << " moedas ativas\n"
                        << marginLeft << " totalizando R$" << it->active << " na ultima atualizacao.\n";
                        pause();
            return;
        }

        char confirm;
        std::cout << marginLeft << " Deseja mesmo " << optionToDo << " " << coinToChange << " ? (y/n): ";
        std::cin >> confirm;
        if(confirm != 'y' && confirm != 'Y') return;
        switch(changeOption){
            case 1: alterCoin(coinToChange); break;
            case 2: deleteCoin(coinToChange); break;
        }
    }
}

void deleteCoin(std::string coinToChange){
    std::cout << marginLeft << " Excluindo " << coinToChange << "..." << std::endl;

    walletData.erase(
        std::remove_if(walletData.begin(), walletData.end(), [&](const CoinData& coin){
                        return coin.coin == coinToChange;
                        }),
        walletData.end()
        );
        updateToJson(walletData);
        std::cout << marginLeft << " " << coinToChange << " foi removida com sucesso.\n";
        pause();
        return;
}

void alterCoin(std::string oldCoinName){
    std::string newName;
    std::cout << '\n' << marginLeft << " Digite o Novo Nome da Moeda " << oldCoinName << ": \n";
    std::cout << marginLeft << " > ";
    std::cin >> newName;
    for(auto & c : newName)c = toupper(c);
    auto it = std::find_if(walletData.begin(), walletData.end(), [&](const CoinData& coin){
        return coin.coin == newName;
    });
    if(it != walletData.end()){
        std::cout << marginLeft << " Ja Existe uma Moeda com esse Nome " << newName << std::endl;
        pause();
        return;
    }
    for(auto& coin : walletData){
        if(coin.coin == oldCoinName){
            coin.coin = newName;
            break;
        }
    }
    for(auto& coin : operationData){
        if(coin.coin == oldCoinName){
            coin.coin = newName;
        }
    }
    std::cout << marginLeft << " "<< oldCoinName << " Atualizado para "<< newName << std::endl;
    pause();
    updateToCsv(operationData, headerCsv);
    updateToJson(walletData);
    
}

void readFromCsv(){
    operationData.clear();
    headerCsv.clear();
    std::string csvLine ="";
    std::string valueHeader = "";
    std::ifstream datasOperation_f(operationFileName);
    if(!datasOperation_f.is_open()){
        screenClear();
        std::cerr << marginLeft << " Erro ao tentar abrir " << operationFileName << std::endl;
        pause();
        return;
    }
    if(std::getline(datasOperation_f, csvLine)){ // CSV Header Read
        std::stringstream ss(csvLine);
        while(std::getline(ss, valueHeader, ',')){
            headerCsv.push_back(valueHeader);
        }
    }

    while(std::getline(datasOperation_f, csvLine)){ // CSV Values Reader
        std::stringstream ss(csvLine);
        HistoryData entry;
        std::string value;

        std::getline(ss, value, ',');
        entry.id = stoi(value);
        std::getline(ss, entry.coin, ',');
        std::getline(ss, entry.operation_Date, ',');
        std::getline(ss, entry.operation_Type, ',');
        std::getline(ss, value, ',');
        entry.amount = stod(value);
        std::getline(ss, value, ',');
        entry.coin_Price = stod(value);
        std::getline(ss, value, ',');
        entry.coin_Amount = stod(value);
        std::getline(ss, value, ',');
        entry.fee_Buy = stod(value);
        std::getline(ss, value, ',');
        entry.fee_Sell = stod(value);
        std::getline(ss, value, ',');
        entry.target_Percentage = stod(value);
        std::getline(ss, value, ',');
        entry.target_Coin_Value = stod(value);

        operationData.push_back(entry);
    }
    datasOperation_f.close();
}

void readFromJson(){
    walletData.clear();
    std::ifstream dataWallet_f(walletFileName);
    if(!dataWallet_f.is_open()){
        screenClear();
        std::cerr << marginLeft << " Erro ao tentar abrir " << walletFileName << std::endl;
        pause();
        return;
    }
    nlohmann::json j; // Vetor Make from JSON
    dataWallet_f >> j;
    for (const auto& item : j){
        CoinData coin;
        coin.coin = item.value("coin", "");
        coin.amount = item.value("amount", 0.0);
        coin.active = item.value("active", 0.0);
        coin.profit = item.value("profit", 0.0);
        coin.target_Amount = item.value("target_Amount", 0.0);
        coin.target_Coin_Price = item.value("target_Coin_Price", 0.0);
        coin.last_Update = item.value("last_Update", "");
        coin.last_Coin_Price = item.value("last_Coin_Price", 0.0);

        walletData.push_back(coin);
    } // End Vector Make From JSON
    dataWallet_f.close();
}

void updateToCsv(const std::vector<HistoryData>& operationData, const std::vector<std::string>& headerCsv){
    
    std::ofstream datasOperation_f(operationFileName);
    if(!datasOperation_f.is_open()){
        screenClear();
        std::cerr << marginLeft << " Erro ao tentar abrir " << operationFileName << std::endl;
        pause();
        return;
    }

    for(size_t i = 0; i < headerCsv.size(); ++i){
        datasOperation_f << headerCsv[i];
        if(i != headerCsv.size() - 1)
            datasOperation_f << ',';
    }
    datasOperation_f << std::endl;
      
    for(const auto& value : operationData){
        datasOperation_f    << value.id << ','
                            << value.coin << ','
                            << value.operation_Date << ','
                            << value.operation_Type << ','
                            << std::fixed << std::setw(2) << value.amount << ','
                            << std::fixed << std::setw(2) << value.coin_Price << ','
                            << std::fixed << std::setw(6) << value.coin_Amount << ','
                            << std::fixed << std::setw(2) << value.fee_Buy << ','
                            << std::fixed << std::setw(2) << value.fee_Sell << ','
                            << std::fixed << std::setw(2) << value.target_Percentage << ','
                            << std::fixed << std::setw(6) << value.target_Coin_Value << std::endl;
    }
//ID,Coin,Date,Type,Amount,Coin Price,Coin Amount,Feebuy,FeeSell,Target,Target Coin Value
    datasOperation_f.close();

}

void updateToJson(const std::vector<CoinData>& walletData){
    nlohmann::json j;
    std::cout << marginLeft <<  " Atualizando a Carteira...";
    for (const auto& coin : walletData) {
        j.push_back({
            {"coin", coin.coin},
            {"amount", coin.amount},
            {"active", coin.active},
            {"profit", coin.profit},
            {"target_Amount", coin.target_Amount},
            {"target_Coin_Price", coin.target_Coin_Price},
            {"last_Update", coin.last_Update},
            {"last_Coin_Price", coin.last_Coin_Price}
        });
    }
    std::ofstream outCoin(walletFileName);
    if (!outCoin.is_open()) {
        std::cerr << "\n "<< marginLeft << "Erro ao abrir " << walletFileName << std::endl;
        return;
    }
    outCoin << std::setw(4) << j << std::endl;
    outCoin.close();
    std::cout << std::endl;
    std::cout << marginLeft << " Carteira atualizada com sucesso!\n";
    readFromJson();
}
