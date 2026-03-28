#pragma once
#include <random>
#include <vector>

struct SimulationStats {
    int total_number_of_bets = 0;
    int matches = 0;
    double total_payout = 0;
    double base_game_payout = 0;
    double free_spins_payout = 0;
    int free_spins_triggered = 0;
    double hit_rate = 0;
    double RTP;
    double free_spins_RTP;
    double base_game_RTP;
    int free_spins_frequency = 0;
    double free_spins_mean;
};

void process_spin(const std::vector<std::vector<int>>& spin_screen, SimulationStats& stats);
SimulationStats run_simulation(const std::vector<std::vector<int>>& reels, int number_of_spins, std::mt19937& gen);
void perform_simulation(const std::vector<std::vector<int>>& reels, std::mt19937& gen);