#include "simulation.hpp"
#include "engine.hpp"
#include "config.hpp"
#include <iostream>



void process_spin(const std::vector<std::vector<int>>& spin_screen, SimulationStats& stats){
    ScreenEvaluation screen_stats=evaluate_screen(spin_screen,paylines,scatter_payout_table);
    stats.total_payout+=screen_stats.total_payout;
    if(screen_stats.total_payout!=0){
        stats.matches++;
    }
    stats.total_number_of_bets++;
}
SimulationStats run_simulation(const std::vector<std::vector<int>>& reels, int number_of_spins,std::mt19937& gen){
    SimulationStats stats;
    int free_spins_count = 0;
    for(int i=0;i<number_of_spins;i++){
        std::vector<std::vector<int>> spin_result = spin_screen(reels,gen);
        process_spin(spin_result,stats);
        if(feature_trigger(spin_result)){
            free_spins_count++;
        }
    }
    stats.hit_rate=static_cast<double>(stats.matches)/stats.total_number_of_bets;
    stats.RTP=stats.total_payout/stats.total_number_of_bets;
    if(free_spins_count==0){
        stats.free_spins_frequency = 0;
    }
    else{
        stats.free_spins_frequency = stats.total_number_of_bets/free_spins_count;
    }
    
    return stats;
}
void perform_simulation(const std::vector<std::vector<int>>& reels, std::mt19937& gen){
    std::cout<<"Enter a number of bets: ";
    int number_of_spins;
    std::cin>>number_of_spins;
    SimulationStats stats = run_simulation(reels,number_of_spins,gen);
    std::cout<<"\nTotal bets performed: "<<stats.total_number_of_bets<<"\nHit rate: "<<stats.hit_rate<<"\nRTP: "<<stats.RTP<<"\nFree spins frequency: "<<stats.free_spins_frequency;
}