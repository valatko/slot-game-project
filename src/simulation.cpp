#include "simulation.hpp"
#include "engine.hpp"
#include "config.hpp"
#include <iostream>



void process_spin(const std::vector<std::vector<int>>& spin_screen, SimulationStats& stats, std::mt19937& gen){
    ScreenEvaluation screen_stats=evaluate_screen(spin_screen,paylines,scatter_payout_table);
    stats.base_game_payout+=screen_stats.total_payout;
    if(screen_stats.total_payout!=0){
        stats.matches++;
    }
    if (screen_stats.free_spins_triggered){
        stats.free_spins_triggered ++;
    }
    if(screen_stats.awarded_free_spins>0){
        BonusResult bonus = run_free_spins(reels,screen_stats.awarded_free_spins,gen);
        stats.free_spins_payout+=bonus.total_payout;
    }
    stats.total_number_of_bets++;
    stats.total_payout=stats.base_game_payout+stats.free_spins_payout;
}
SimulationStats run_simulation(const std::vector<std::vector<int>>& reels, int number_of_spins,std::mt19937& gen){
    SimulationStats stats;
    for(int i=0;i<number_of_spins;i++){
        std::vector<std::vector<int>> spin_result = spin_screen(reels,gen);
        process_spin(spin_result,stats,gen);
    }
    stats.hit_rate=static_cast<double>(stats.matches)/stats.total_number_of_bets;
    stats.RTP=stats.total_payout/stats.total_number_of_bets;
    stats.base_game_RTP=stats.base_game_payout/stats.total_number_of_bets;
    stats.free_spins_RTP=stats.free_spins_payout/stats.total_number_of_bets;
    if(stats.free_spins_triggered==0){
        stats.free_spins_frequency = 0;
    }
    else{
        stats.free_spins_frequency = stats.total_number_of_bets/stats.free_spins_triggered;
        stats.free_spins_mean = stats.free_spins_payout/stats.free_spins_triggered;
    }
    
    return stats;
}
void perform_simulation(const std::vector<std::vector<int>>& reels, std::mt19937& gen){
    std::cout<<"Enter a number of bets: ";
    int number_of_spins;
    std::cin>>number_of_spins;
    SimulationStats stats = run_simulation(reels,number_of_spins,gen);
    std::cout<<"\nTotal bets performed: "
        <<stats.total_number_of_bets
        <<"\nHit rate: "
        <<stats.hit_rate
        <<"\nRTP: "
        <<stats.RTP
        <<"\nFree spins frequency: "
        <<stats.free_spins_frequency
        <<"\nFree spins mean: "
        <<stats.free_spins_mean
        <<"\nFree spins RTP: "
        <<stats.free_spins_RTP
        <<"\nBase game RTP: "
        <<stats.base_game_RTP;
}