#include "play.hpp"
#include "config.hpp"
#include "engine.hpp"
#include<iostream>

void print_screen(const std::vector<std::vector<int>>& screen){
    for(int i = 0; i<screen[0].size();i++){
        for(int j = 0; j<screen.size(); j++){
            std::cout<<screen[j][i]<<" ";
        }
        std::cout<<"\n";
    }
}
void print_line_result(const LineResult& result){
    std::cout << "Line: ";
    for(int symbol : result.symbols){
        std::cout<<symbol<<" ";
    }
    std::cout<<"| base: "<<result.base_symbol<<"| length: "<<result.win_length<<"| payout: "<<result.payout<<"\n";
}
void print_screen_evaluation(const ScreenEvaluation& result){
    std::cout<<"Total payout: "<<result.total_payout<<"| winning lines: "<<result.winning_lines<<"| scatter count: "<<result.scatter_count<<"| scatter payout: "<<result.scatter_payout<<"\n";
    for(int i = 0; i<result.line_results.size();i++){
        print_line_result(result.line_results[i]);
    }
    std::cout<<"Free spins triggered: ";
    if(result.free_spins_triggered){
        std::cout<<"yes\n";
    }else{
        std::cout<<"no\n";
    }
}
void play_free_spins(const std::vector<std::vector<int>>& reels, int number_of_spins,std::mt19937& gen){
    int total_payout = 0;
    int remaining_spins = 3;
    int current_spin = 1;
    while(remaining_spins>0){
        std::cout<<"Spin "<<current_spin<<"/"<<remaining_spins<<"\n";
        current_spin++;
        std::vector<std::vector<int>> screen = spin_screen(bonus_reels,gen);
        print_screen(screen);
        int cash_count = count_symbol_on_screen(screen,CASH);
        if(cash_count>0){
            std::cout<<"Cash tile values: ";
            remaining_spins = 3;
            int spin_payout = 0;
            for(int i = 0;i<cash_count;i++){
                int value = pick_random_multiplier(cash_multipliers,cash_multipliers_probabilities);
                spin_payout+=value;
                std::cout<<value<<", ";
            }
            std::cout<<"\nSpin payout: "<<spin_payout<<"\n";
            total_payout += spin_payout;
        }
        else{
            remaining_spins--;
        }
    }
    std::cout<<"Total Free Spins payout: "<<total_payout<<"\n";
}

void play_game(const std::vector<std::vector<int>>& reels, std::mt19937& gen){
    std::vector<std::vector<int>> screen = spin_screen(reels,gen);
    print_screen(screen);
    ScreenEvaluation result = evaluate_screen(screen,paylines,scatter_payout_table);
    print_screen_evaluation(result);
    if(result.free_spins_triggered){
        play_free_spins(reels,BONUS_SPINS,gen);
    }
    char play_again;
    std::cout<<"Do you want to play again? (y/n)\n";
    std::cin>>play_again;
    if(play_again == 'y'){
        play_game(reels,gen);
    }else if(play_again == 'n'){
        std::cout<<"Okay, bye!\n";
    }else{
        std::cout<<"Invalid input, boss, perform another spin to clear your head, and try again. :)\n";
        play_game(reels,gen);
    }
}