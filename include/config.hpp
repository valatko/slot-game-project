#pragma once
#include<vector>

extern const int ROWS;
extern const int COLUMNS;
extern const int WILD;
extern const int SCATTER;
extern const int BONUS_SPINS;
extern const int CASH;
extern const int EMPTY;

extern const std::vector<std::vector<int>>paylines;
extern const std::vector<std::vector<int>>payout_table;
extern const std::vector<int>scatter_payout_table;
extern const std::vector<std::vector<int>>reels;
extern const std::vector<std::vector<int>>bonus_reels;
extern const std::vector<int> cash_multipliers;
extern const std::vector<double> cash_multipliers_probabilities;