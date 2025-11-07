#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <printers.h>
#include "reports.h"
#include "utils.h"
#include "cruddata.h"
#include "json.hpp"

void updateCoinPrice(std::vector<CoinData>& walletData, HistoryData& op){
   auto it = std::find_if(walletData.begin(), walletData.end(),
                                [&](const CoinData& coin){
                                return coin.coin == op.coin;
                                });
    it ->last_Coin_Price = op.coin_Price;
}
void buyCoins(std::vector<HistoryData>& operationData, std::vector<CoinData>& walletData) {
    readFromJson();
    std::string coinToBuy = "";
    HistoryData newBuy;
    char confirm;
    newBuy.id = getLastOperationId();
    newBuy.operation_Type = "BUY";

    while (true) {
        screenClear();
        const auto uniqueCoins = existCoins();
        printHeader("Nova Compra", 100);
        printFormattedLine("Moedas Existentes:", 100);

        std::ostringstream oss;
        for (size_t i = 0; i < uniqueCoins.size(); ++i) {
            oss << uniqueCoins[i];
            if (i < uniqueCoins.size() - 1) oss << " | ";
        }
        printFormattedLine(oss.str(), 100);
        printEmptyLine(1, 100);
        printFooter("Digite a sigla da moeda ou 0 para voltar", 100);

        std::cout << marginLeft << " > ";
        std::cin >> coinToBuy;
        for (auto& c : coinToBuy) c = toupper(c);
        if (coinToBuy == "0") return;

        auto it = std::find_if(walletData.begin(), walletData.end(), [&](const CoinData& c) {
            return c.coin == coinToBuy;
        });

        if (it == walletData.end()) {
            std::cout << marginLeft << "Deseja cadastrar " << coinToBuy << "? (y/n): ";
            std::cin >> confirm;
            if (confirm != 'y' && confirm != 'Y') return;
            insertCoin(coinToBuy);
            readFromJson();
            continue;
        }

        screenClear();
        printHeader("Nova Compra", 100);
        std::ostringstream line_str;
        line_str    << "Moeda " << it->coin << " | Ativos R$ " << std::fixed << std::setprecision(2) << it->active
                    << " | Quantidade " << std::fixed << std::setprecision(6) << it->amount
                    << " | Alvo: R$ " << std::fixed << std::setprecision(2) << it->target_Amount;
        printFormattedLine(line_str.str(), 100);
        printEmptyLine(1,100);
        line_str.str(""), line_str.clear();
        printFooter("",100);
        std::cout << marginLeft << " Moeda: " << coinToBuy << "\n";
        newBuy.amount = getValidInput<double>("Investimento", "R$");
        newBuy.coin_Price = getValidInput<double>("Preco da Moeda", "R$");
        newBuy.fee_Buy = getValidInput<double>("Taxa de Entrada", "%");
        newBuy.fee_Sell = getValidInput<double>("Taxa de Saida", "%");
        newBuy.target_Percentage = getValidInput<double>("Alvo", "%");

        std::string inputDate;
        while (true) {
            std::cout << marginLeft << " Data da Operacao (dd/mm/aaaa ou 0 para hoje):\n";
            std::cout << marginLeft << " > ";
            std::cin >> inputDate;
            std::string validDate = getValidDate(inputDate);
            if (!validDate.empty()) {
                newBuy.operation_Date = validDate;
                break;
            }
        }

        newBuy.coin = coinToBuy;
        double netAmount = newBuy.amount * (1 - newBuy.fee_Buy / 100);
        newBuy.coin_Amount = netAmount / newBuy.coin_Price;
        double targetValue = (newBuy.amount * (1 + newBuy.target_Percentage / 100)) / (1 - newBuy.fee_Sell / 100);
        newBuy.target_Coin_Value = targetValue / newBuy.coin_Amount;

        // update Wallet
        auto& coin = *std::find_if(walletData.begin(), walletData.end(), [&](CoinData& c) {
            return c.coin == coinToBuy;
        });

        coin.amount += newBuy.coin_Amount;
        coin.active += netAmount;
        coin.last_Coin_Price = newBuy.coin_Price;
        coin.last_Update = newBuy.operation_Date;

        if (coin.target_Amount == 0.0) {
            coin.target_Amount = targetValue;
            coin.target_Coin_Price = newBuy.target_Coin_Value;
        }else{
            double totalValueBefore = coin.target_Coin_Price * coin.target_Amount;
            double totalValueNew = newBuy.target_Coin_Value * targetValue;
            coin.target_Amount += targetValue;
            coin.target_Coin_Price = (totalValueBefore + totalValueNew) / coin.target_Amount;
        }

        operationData.push_back(newBuy);
        updateToCsv(operationData, headerCsv);
        updateToJson(walletData);

        coinToBuy.clear();
    }
}

void sellCoins(std::vector<HistoryData>& operationData, std::vector<CoinData>& walletData) {
    HistoryData newSell;
    std::string coinToSell;
    newSell.id = getLastOperationId();
    newSell.operation_Type = "SELL";
    char confirm;

    while (true) {
        screenClear();
        const auto uniqueCoins = existCoins();
        printHeader("Nova Venda", 100);
        printFormattedLine("Moedas Existentes:", 100);

        std::ostringstream oss;
        for (size_t i = 0; i < uniqueCoins.size(); ++i) {
            oss << uniqueCoins[i];
            if (i < uniqueCoins.size() - 1) oss << " | ";
        }
        printFormattedLine(oss.str(), 100);
        printEmptyLine(1, 100);
        printFooter("Digite a moeda ou 0 para voltar", 100);

        std::cout << marginLeft << " > ";
        std::cin >> coinToSell;
        for (auto& c : coinToSell) c = toupper(c);
        if (coinToSell == "0") return;

        auto it = std::find_if(walletData.begin(), walletData.end(),
                            [&](const CoinData& c) {
                            return c.coin == coinToSell;
                            });

        if (it == walletData.end() || it->amount <= 0.0) {
            std::cout << marginLeft << "[!] Moeda nao encontrada ou sem saldo.\n";
            pause();
            return;
        }

        screenClear();
        printHeader("Resumo da Posicao", 100);
        std::ostringstream line_str;
        line_str    << "Moeda " << it->coin << " | Ativos R$ " << std::fixed << std::setprecision(2) << it->active
                    << " | Quantidade " << std::fixed << std::setprecision(6) << it->amount
                    << " | Alvo: R$ " << std::fixed << std::setprecision(2) << it->target_Amount;
        printFormattedLine(line_str.str(), 100);
        printEmptyLine(1,100);
        line_str.str(""), line_str.clear();

        printFooter("", 100);
        double newPrice = getValidInput<double>("Preco atual da moeda", " R$");
        double feeOut = getValidInput<double>("Taxa de Saida", " %");

        double valuePrev = it->amount * newPrice * (1 - feeOut / 100);

        std::cout << marginLeft << " Voce possui: " << it->amount << " " << coinToSell
               << " | Valor estimado liquido: R$ " << std::fixed << std::setprecision(2) << valuePrev << std::endl;
        
        double sellValue = getValidInput<double>("Valor a vender ou 0 para venda Total", "");
        if (sellValue != 0){
            if(sellValue >= valuePrev){
                std::cout   << marginLeft << "Impossível vender R$" << sellValue << " em " << coinToSell << std::endl
                            << marginLeft << " por ser maior que o Disponivel Previsto" << valuePrev;
                return;
            }
            double sellQty = sellValue/newPrice;
            newSell.coin = coinToSell;
            newSell.coin_Price = newPrice;
            newSell.fee_Sell = feeOut;
            newSell.coin_Amount = sellQty;
            newSell.amount = sellValue;
        }
        if(sellValue == 0){
            newSell.coin = coinToSell;
            newSell.coin_Price = newPrice;
            newSell.fee_Sell = feeOut;
            newSell.coin_Amount = it->amount;
            newSell.amount = newPrice * it->amount;
        }
        std::string inputDate;
        while (true) {
            std::cout << marginLeft << " Data da Operacao (dd/mm/aaaa ou 0 para hoje):\n";
            std::cout << marginLeft << " > ";
            std::cin >> inputDate;
            std::string validDate = getValidDate(inputDate);
            if (!validDate.empty()) {
                newSell.operation_Date = validDate;
                break;
            }
        }

        // Lógica de venda
        double netValue = newSell.amount * (1 - newSell.fee_Sell / 100);
        double avgPrice = it->active / it->amount;
        double costBasis = avgPrice * newSell.coin_Amount;
        double profit = netValue - costBasis;

        screenClear();
        printEmptyLine(1, 100);
        printHeader("Resumo da Venda", 100);
        std::ostringstream resumoVenda;
        resumoVenda << "Quantidade a vender: " << std::fixed << std::setprecision(6) << newSell.coin_Amount << " " << newSell.coin;
        printFormattedLine(resumoVenda.str(), 100);
        resumoVenda.str(""); resumoVenda.clear();

        resumoVenda << "Valor bruto: R$ " << std::fixed << std::setprecision(2) << newSell.amount;
        printFormattedLine(resumoVenda.str(), 100);
        resumoVenda.str(""); resumoVenda.clear();

        resumoVenda << "Taxa (" << std::fixed << std::setprecision(2) << newSell.fee_Sell << " %): R$ " << (newSell.amount - netValue);
        printFormattedLine(resumoVenda.str(), 100);
        resumoVenda.str(""); resumoVenda.clear();

        resumoVenda << "Valor liquido: R$ " << std::fixed << std::setprecision(2) << netValue;
        printFormattedLine(resumoVenda.str(), 100);
        resumoVenda.str(""); resumoVenda.clear();

        resumoVenda << "Lucro estimado: R$ " << std::fixed << std::setprecision(2) << profit;
        printFormattedLine(resumoVenda.str(), 100);
        printFooter("", 100);
        std::cout << marginLeft << " Confirmar a venda? (y/n) ";

        std::cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') return;

        it->active -= costBasis;
        it->amount -= newSell.coin_Amount;
        it->target_Amount -= newSell.coin_Amount;
        it->profit += profit;
        it->last_Coin_Price = newSell.coin_Price;
        it->last_Update = newSell.operation_Date;
        if (it->target_Amount < 0) it->target_Amount = 0.0;

        operationData.push_back(newSell);
        updateToCsv(operationData, headerCsv);
        updateToJson(walletData);
    }
}
