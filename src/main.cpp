#include<iostream>
#include<random>
#include<vector>
#include "config.hpp"
#include "simulation.hpp"
#include "play.hpp"

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    int choice;
    std::cout<<"Do you want to play, or do you want to perform a simulation?\n"<<"0: play\n"<<"1: simulation\n";
    std::cin >>choice;
    if(choice == 0){
        play_game(reels,gen);
    }else if(choice == 1){
        perform_simulation(reels,gen);
    }else{
        std::cout<<"Invalid input";
    }
    return 0;
}