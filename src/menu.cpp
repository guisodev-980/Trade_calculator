#include "printers.h"
#include "utils.h"
#include "reports.h"
#include "cruddata.h"
#include "operation.h"
#include <iostream>

void sMenuOperation(){
    while(true){
        int subMenuOption = 0;
        screenClear();
        printHeader("Operacoes", 80);
        printFormattedLine(" 1 - Nova Compra", 80);
        printFormattedLine(" 2 - Nova Venda", 80);
        printEmptyLine(2, 80);
        printFooter("[!] Digite 0 para voltar", 80);
        std::cout << marginLeft << " " << "Digite uma Opcao: ";
        std::cin >> subMenuOption; 
        if(subMenuOption == 0) return;
        if(!(subMenuOption) || (subMenuOption < 1 || subMenuOption > 2)){
            std::string text = "Entrada Invalida";
            handleMenuInputError(text);
            continue;
        }
        switch(subMenuOption){
            case 1: buyCoins(operationData, walletData); break; 
            case 2: sellCoins(operationData, walletData); break; 
            case 3: "Alterar"; break; // updateData()
            
        }
    }
}
void sMenuCoin(){
    while(true){
        int subMenuOption = 0;
        screenClear();
        printHeader("Moedas ", 80);
        printFormattedLine(" 1 - Nova Moeda", 80);
        printFormattedLine(" 2 - Alterar Moeda", 80);
        printFormattedLine(" 3 - Excluir Moeda", 80);
        printEmptyLine(2, 80);
        printFooter("[!] Digite 0 para Voltar!", 80);
        std::cout << marginLeft << " " << "Digite uma opcao: ";
        std::cin >> subMenuOption;
        if(subMenuOption == 0) return;
        if(!(subMenuOption) || subMenuOption < 1 || subMenuOption > 3){
            std::string text = "Entrada Invalida";
            handleMenuInputError(text);
            continue;
        }
        switch(subMenuOption){
            case 1: insertCoin(""); break; 
            case 2: listCoinToChange(1); break;
            case 3: listCoinToChange(2); break;
            case 4: return;
        }
    }
}
void sMenuLists(){
    while(true){
        int subMenuOption = 0;
        std::string filterBy = "";
        screenClear();
        printHeader("Relatorios ", 80);
        printFormattedLine(" 1 - Historico Operacoes", 80);
        printFormattedLine(" 2 - Operacoes por Moeda", 80);
        printFormattedLine(" 3 - Moedas Ativas", 80);
        printFormattedLine(" 4 - Alvos por Moeda", 80);
        printFormattedLine(" 5 - Balanco por Moeda", 80);
        printFormattedLine(" 6 - Balanco Geral", 80);
        printEmptyLine(2, 80);
        printFooter("[!] Digite 0 para Voltar!", 80);
        std::cout << marginLeft << " " << "Digite uma Opcao: ";
        std::cin >> subMenuOption;
        if(subMenuOption == 0) return;
        if(!(subMenuOption) || subMenuOption < 1 || subMenuOption > 6){
            std::string text = "Entrada Invalida";
            handleMenuInputError(text);
            continue;
        }
        std::cout << marginLeft << " " << "Digite uma Opcao: ";
        switch(subMenuOption){
            case 1: reportOperations(); break;
            case 2: reportByCoin(); break;
            case 3: reportActiveCoins(); break;
            case 4: reportTargetByCoin(); break;
            case 5: reportProfitByCoin(); break;
            case 6: reportOvehallProfit(); break;
            case 7: return;
        }
    }
}

void menu(){
    
    while(true){
        int menuOption = 0;
        screenClear();
        printHeader("Menu Principal ", 80);
        printFormattedLine(" 1 - Operacoes", 80);
        printFormattedLine(" 2 - Moedas", 80);
        printFormattedLine(" 3 - Relatorios", 80);
        printEmptyLine(3, 80);
        printFooter("[!] Digite 0 para Sair", 80);

        std::cout << marginLeft << " " << "Digite uma opcao: ";
        std::cin >> menuOption;
        if(menuOption == 0) std::exit(EXIT_SUCCESS); // implementar Exit
        if(!(menuOption) || menuOption < 1 || menuOption > 3){
            std::string text = "Entrada Invalida";
            handleMenuInputError(text);
            continue;
        }
        switch(menuOption){
            case 1: sMenuOperation(); break;
            case 2: sMenuCoin(); break;
            case 3: sMenuLists(); break;
        }
    }
}
