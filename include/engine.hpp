#pragma once
#include <random>
#include <vector>

struct LineResult {
    std::vector<int> symbols;
    int base_symbol = 0;
    int win_length = 0;
    int payout = 0;
};

struct ScreenEvaluation {
    int total_payout = 0;
    int winning_lines = 0;
    int scatter_count = 0;
    int scatter_payout = 0;
    bool free_spins_triggered = false;
    int awarded_free_spins = 0;
    double free_spins_payout = 0;
    std::vector<LineResult> line_results;
};
struct BonusResult {
    int spins_played = 0;
    double total_payout = 0;
};

int pick_random_stop(const std::vector<int>& reel, std::mt19937& gen);
int get_base_symbol(const std::vector<int>& payline);
int count_symbol_on_screen(const std::vector<std::vector<int>>& screen, int symbol);
int length_of_win_line(const std::vector<int>& spin_result);
std::vector<int> get_visible_window(const std::vector<int>& reel, int stop_index);
std::vector<std::vector<int>> spin_screen(const std::vector<std::vector<int>>& reels, std::mt19937& gen);
std::vector<int> get_payline(const std::vector<std::vector<int>>& screen, const std::vector<int>& line_definition);
int get_line_payout(const std::vector<int>& spin_result);
int get_scatter_payout(const std::vector<std::vector<int>>& screen, const std::vector<int>& scatter_payout);
bool feature_trigger(const std::vector<std::vector<int>>& screen);
LineResult evaluate_line(const std::vector<std::vector<int>>& screen, const std::vector<int>& line_definition);
int get_awarded_free_spins(const std::vector<std::vector<int>>& screen);
BonusResult run_free_spins(const std::vector<std::vector<int>>& reels, int number_of_spins, std::mt19937& gen);
ScreenEvaluation evaluate_screen(const std::vector<std::vector<int>>& screen, const std::vector<std::vector<int>>& paylines, const std::vector<int>& scatter_payout);