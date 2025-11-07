#pragma once
#include "utils.h"
#include "cruddata.h"

//prototyping

void walletInputs(const HistoryData& op, std::vector<CoinData>& walletData);
void buyCoins(std::vector<HistoryData>& operationData, std::vector<CoinData>& walletData);
void sellCoins(std::vector<HistoryData>& OperationData, std::vector<CoinData>& walletData);