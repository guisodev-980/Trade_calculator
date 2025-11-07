 #include <iostream>
 #include <string>
 #include <vector>
 #include <limits>
 #include <sstream>
 #include <set>
 #include "json.hpp"
 #include "cruddata.h"
 #include "printers.h"
 
 /*
    "1 - Historico Operacoes"   // Cópia posição menu e var de Entrada
    "2 - Operacoes por Moeda"   
    "3 - Moedas Ativas"         
    "4 - Alvos por Moeda"       
    "5 - Balanco por Moeda"     
    "6 - Balanco Geral"         
 */

void reportOperations(){
    readFromCsv();
    screenClear();
    int colWith[] = {4, 5, 10, 6, 10, 14, 12, 7, 7, 7, 18};
    int totalWith = 0;
    printHeader("Historico de Operacoes ", 134);
    printCsvHeader(134);
    std::ostringstream preFooter_strH;
    std::ostringstream preFooter_strR;
    double amountTotal = 0.0;
    double feeInAvarege = 0.0;
    double feeOutAvarege = 0.0;
    int count = 0;
    for(const auto& op : operationData){
        std:: ostringstream data_str;
        data_str    << std::right   << std::setw(colWith[0]) << op.id << " | "
                                    << std::setw(colWith[1]) << op.coin << " | "
                                    << std::setw(colWith[2]) << op.operation_Date << " | "
                                    << std::setw(colWith[3]) << op.operation_Type << " | "
                                    << std::setw(colWith[4]) << std::fixed << std::setprecision(2) << op.amount << " | "
                                    << std::setw(colWith[5]) << std::fixed << std::setprecision(6) << op.coin_Price << " | "
                                    << std::setw(colWith[6]) << std::fixed << std::setprecision(6) << op.coin_Amount << " | "
                                    << std::setw(colWith[7]) << std::fixed << std::setprecision(2) << op.fee_Buy << " | "
                                    << std::setw(colWith[8]) << std::fixed << std::setprecision(2) << op.fee_Sell << " | "
                                    << std::setw(colWith[9]) << std::fixed << std::setprecision(2) << op.target_Percentage << " | "
                                    << std::setw(colWith[10]) << std::fixed << std::setprecision(6) << op.target_Coin_Value; 
        printFormattedLine(data_str.str(), 134);
        data_str.str("");
        data_str.clear();
        ++count;
        amountTotal += op.amount;
        feeInAvarege += op.fee_Buy;
        feeOutAvarege += op.fee_Sell;
    }
    preFooter_strH  << std::right   << std::setw(14) << "Operacoes" << " | "
                                    << std::setw(24) << "Total Movimentado R$" << " | "
                                    << std::setw(12) << "% Media" << " | "
                                    << std::setw(12) << "% Media Saida";

    preFooter_strR  << std::right   << std::setw(14) << count << " | " 
                                    << std::setw(24) << std::fixed << std::setprecision(2) << amountTotal << " | "
                                    << std::setw(12) << std::fixed << std::setprecision(2) << feeInAvarege/count << " | "
                                    << std::setw(12) << std::fixed << std::setprecision(2) << feeOutAvarege/count;
    printEmptyLine(1,134);
    printPreFooter(134);
    printFormattedLine(preFooter_strH.str(), 134);
    printFormattedLine(preFooter_strR.str(), 134);
    preFooter_strH.str(""), preFooter_strH.clear();
    preFooter_strR.str(""), preFooter_strR.clear();
    printFooter("", 134);
    count = 0.0;
    pause();
 }

void reportByCoin(){
    while(true){
        readFromCsv();
        screenClear();
        std::string filterBy;
        char confirm;
        std::set<std::string> uniqueCoins;
        for(const auto& op : operationData){
            uniqueCoins.insert(op.coin);
        }
        int count = 0;
        printHeader("Historico por Moedas ", 134);
        printFormattedLine("Moedas Existentes ", 134);
        std::ostringstream unique_line;
        for(const auto& coin : uniqueCoins){
            unique_line << coin;
            if(++count < uniqueCoins.size()) unique_line << " | ";
        }
        printFormattedLine(unique_line.str(), 134);
        unique_line.str("");
        unique_line.clear();
        printEmptyLine(1, 134);
        printFooter("[!] Digite 0 Para Voltar", 134);
        std::cout << std::endl;
        std::cout << marginLeft << " Nome da Moeda ou 0 Para Voltar: ";
        std::cin >> filterBy;
        for (auto& c : filterBy) c = toupper(c);
        if(filterBy == "0") return;

        bool coinExists = std::any_of(
            operationData.begin(),
            operationData.end(),
            [&](const HistoryData& coin){
                return coin.coin == filterBy;
            });
        if(!coinExists){
            std::cout << marginLeft << " " << filterBy << " Nao encontrada!";
            pause();
            continue;
        }

        int colWith[] = {4, 5, 10, 6, 10, 14, 12, 7, 7, 7, 18};
        int totalWith = 0;
        bool printLine = true;
        for(int w : colWith) totalWith +=(w+3);
        screenClear();
        printHeader("Historico Por Moedas ", 134);
        printCsvHeader(134);
        std::ostringstream line_str;
        for(const auto& c : operationData){
            if(c.coin != filterBy) continue;
        
            line_str    << std::right   << std::setw(colWith[0]) << c.id << " | "
                                        << std::setw(colWith[1]) << c.coin << " | "
                                        << std::setw(colWith[2]) << c.operation_Date << " | "
                                        << std::setw(colWith[3]) << c.operation_Type << " | "
                                        << std::setw(colWith[4]) << std::fixed << std::setprecision(2) << c.amount << " | "
                                        << std::setw(colWith[5]) << std::fixed << std::setprecision(6) << c.coin_Price << " | "
                                        << std::setw(colWith[6]) << std::fixed << std::setprecision(6) << c.coin_Amount << " | "
                                        << std::setw(colWith[7]) << std::fixed << std::setprecision(2) << c.fee_Buy << " | "
                                        << std::setw(colWith[8]) << std::fixed << std::setprecision(2) << c.fee_Sell << " | "
                                        << std::setw(colWith[9]) << std::fixed << std::setprecision(2) << c.target_Percentage << " | "
                                        << std::setw(colWith[10]) << std::fixed << std::setprecision(6) << c.target_Coin_Value;
            printFormattedLine(line_str.str(), 134);
            line_str.str("");
            line_str.clear();

        }
        printEmptyLine(1, 134);
        printFooter("[!] Digite 0 Para Voltar", 134);
        std::cout << '\n' << std::endl;
        std::cout << marginLeft << "Nova Consulta? (y/n): ";
        std::cin >> confirm;
        if(confirm != 'y' && confirm !='Y') break;
    }
 }

void reportActiveCoins(){
    screenClear();
    readFromJson();
    printHeader("Moedas Com Ativos", 120);
    std::ostringstream line_str;
    
    line_str    << std::right << std::setw(7) << "Moedas" << " | "
                << std::setw(11) << "Ativos R$" << " | "
                << std::setw(22) << "Qtd" << " | "
                << std::setw(22) << "Preco" << " | "
                << std::setw(12) << "Atualizado" << " | "
                << std::setw(10) << "Balaco" << " | "
                << std::setw(10) << "Valor Alvo" << " | ";
                            
    printFormattedLine(line_str.str(), 120);
    line_str.str("");
    line_str.clear();
    printSeparator(120);
    int count = 0;
    double totalActives = 0;
    double totalProfit = 0;
    for(const auto & coin : walletData){
        if(coin.active > 0){
            ++count;
            totalActives +=coin.active;
            totalProfit += coin.profit;
            line_str    << std::right   << std::setw(7) << coin.coin << " | "
                                        << std::setw(11) << std::fixed << std::setprecision(2) << coin.active << " | "
                                        << std::setw(22) << std::fixed << std::setprecision(6) << coin.amount << " | "
                                        << std::setw(22) << std::fixed << std::setprecision(6) << coin.last_Coin_Price << " | "
                                        << std::setw(12) << coin.last_Update << " | "
                                        << std::setw(10) << std::fixed << std::setprecision(2) << coin.profit << " | "
                                        << std::setw(10) << std::fixed << std::setprecision(2) << coin.target_Coin_Price << " | ";
        printFormattedLine(line_str.str(), 120);
        line_str.str("");
        line_str.clear();
        }   
    }
    printPreFooter(120);
    line_str    << " Quantidade: " << count << " || " 
                << "Total de Ativos: " << totalActives << " || "
                << "Balanco Total: " << totalProfit << " || ";
    printFormattedLine(line_str.str(), 120);
    printFooter("", 120);
    pause();
}

void reportTargetByCoin(){
    readFromJson();
    screenClear();
    printHeader("Alvos Por Moeda", 130);
    std::ostringstream line_str;
    line_str << std::right  << std::setw(5) << "Moeda" << " | "
                            << std::setw(12) << "Ativos R$" << " | "
                            << std::setw(18) << "Quantidade" << " | "
                            << std::setw(18) << "Preco" << " | "
                            << std::setw(12) << "Atualizado" << " | "
                            << std::setw(12) << "Alvo R$" << " | "
                            << std::setw(18) << "Valor Alvo" << " | "
                            << std::setw(12) << "Previsao";
    printFormattedLine(line_str.str(), 130);
    printSeparator(130);
    line_str.str("");
    line_str.clear();
    for(const auto & coin : walletData){
        if(coin.target_Coin_Price > 0){
            std::ostringstream line_str;
            line_str << std::right  << std::setw(5) << coin.coin << " | "
                                    << std::setw(12) << std::fixed << std::setprecision(2) << coin.active << " | "
                                    << std::setw(18) << std::fixed << std::setprecision(6) << coin.amount << " | "
                                    << std::setw(18) << std::fixed << std::setprecision(6) << coin.last_Coin_Price << " | "
                                    << std::setw(12) << coin.last_Update << " | "
                                    << std::setw(12) << std::fixed << std::setprecision(6) << coin.target_Amount << " | "
                                    << std::setw(18) << std::fixed << std::setprecision(6) << coin.target_Coin_Price << " | "
                                    << std::setw(12) << std::fixed << std::setprecision(2) << coin.target_Amount-coin.active;
            printFormattedLine(line_str.str(), 130);
            line_str.str("");
            line_str.clear();
        }
        
    }
    printEmptyLine(1, 130);
    printFooter("[!] Digite 0 Para Voltar", 130);
    pause();
}
void reportProfitByCoin(){
    readFromJson();
    while(true){
        screenClear();
        std::string filterBy;
        char confirm;
        printHeader("Balanco Por Moeda", 70);
        std::stringstream unique_line;
        for(size_t i = 0; i < walletData.size(); ++i){
            unique_line << walletData[i].coin;
                if(i < walletData.size()-1) unique_line << " | ";
        }
        printFormattedLine(unique_line.str(), 70);
        unique_line.str("");
        unique_line.clear();
        printFooter("", 70);
        std::cout << '\n' << marginLeft << " Digite o Nome da Moeda ou 0 para voltar: ";
        std::cin >> filterBy;
        for (auto& c : filterBy) c = toupper(c);
        std::cout << std::endl;
        if(filterBy == "0") break;
        
        bool coinExists = std::any_of(
            walletData.begin(),
            walletData.end(),
            [&](const CoinData& coin){
                return coin.coin == filterBy;
            });
        if(!coinExists){
            std::cout << "\n " << marginLeft << filterBy << " Nao Encontrada!";
            pause();
            continue;
        }
        screenClear();
        printHeader("Balanco Por Moeda", 70);
        std::stringstream line_str;
        line_str    << std::setw(5) << "Moeda" << " | "
                    << std::setw(12) << "Ativos R$" << " | "
                    << std::setw(18) << "Quantidade" << " | "
                    << std::setw(12) << "Balanco";
        printFormattedLine(line_str.str(), 70);
        printSeparator(70);
        line_str.str("");
        line_str.clear();

        for(const auto & coin : walletData){
            if(coin.coin == filterBy){
                std::stringstream line_str;
                line_str    << std::setw(5) << coin.coin << " | "
                            << std::fixed << std::setprecision(2) << std::setw(12) << coin.active << " | "
                            << std::fixed << std::setprecision(6) << std::setw(18) << coin.amount << " | "
                            << std::fixed << std::setprecision(2) << std::setw(12) << coin.profit;
            printFormattedLine(line_str.str(), 70);
            printEmptyLine(1, 70);
            line_str.str("");
            line_str.clear();
            }
        }
        printFooter("[!] Digite 0 Para Voltar", 70);
        std::cout << "\n\n" << marginLeft << " Nova Consulta? (y/n): ";
        std::cin >> confirm;
        if(confirm != 'y' && confirm !='Y') break;
    }
 }
void reportOvehallProfit(){
    readFromJson();
    screenClear();
    std::stringstream line_str;
    double overhallActive = 0;
    double overhallProfit = 0;
    printHeader("Balanco Geral", 70);

    line_str    << std::setw(5) << "Moeda" << " | "
                << std::setw(12) << "Ativos R$" << " | "
                << std::setw(18) << "Quantidade" << " | "
                << std::setw(12) << "Balanco" << " | ";
    printFormattedLine(line_str.str(), 70);
    printSeparator(70);
    line_str.str("");
    line_str.clear();
    
    for(const auto & coin : walletData){
        overhallActive += coin.active;
        overhallProfit += coin.profit;
        line_str    << std::setw(5) << coin.coin << " | "
                    << std::fixed << std::setprecision(2) << std::setw(12) << coin.active << " | "
                    << std::fixed << std::setprecision(8) << std::setw(18) << coin.amount << " | "
                    << std::fixed << std::setprecision(2) << std::setw(12) << coin.profit << " | ";
        printFormattedLine(line_str.str(), 70);
        line_str.str("");
        line_str.clear();
    }
    printPreFooter(70);
    line_str << " Total de Ativos R$: " << overhallActive << " || " << "Balanco Total R$: " << overhallProfit;
    printFormattedLine(line_str.str(), 70);
    printFooter("", 70);
    pause();
}