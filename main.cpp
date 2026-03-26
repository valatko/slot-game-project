#include<iostream>
#include<random>
#include<vector>
int pick_random_symbol(const std::vector<int>& reel, std::mt19937& gen) {
    std::uniform_int_distribution<> dist(0, reel.size()-1);
    int index = dist(gen);
    return reel[index];
}
const std::vector<std::vector<int>> payout_table={
    {0,0,10},
    {0,0,10},
    {0,7,14},
    {0,7,14},
    {0,9,18},
    {0,10,20}
};
const std::vector<std::vector<int>> reels ={
    {1,1,1,2,2,3,3,4,5,6},
    {1,1,1,2,2,3,3,4,5,6},
    {1,1,1,2,2,3,3,4,5,6}
};
int length_of_win_line(const std::vector<int>& spin_result){
    int length=1;
    for (int i=1;i<spin_result.size();i++){
        if(spin_result[i]==spin_result[0]){
            length++;
        }
        else{
            break;
        }
    }
    return length;
}
int get_payout(const std::vector<int>& spin_result){
    return payout_table[spin_result[0]-1][length_of_win_line(spin_result)-1];
}
std::vector<int> spin(const std::vector<std::vector<int>>& reels, std::mt19937& gen){
    std::vector<int> spin_result;
    for(int j=0;j<reels.size();j++){
            int symbol=pick_random_symbol(reels[j],gen);
            spin_result.push_back(symbol);
        }
    return spin_result;
}
struct SimulationStats {
    int total_number_of_bets=0;
    int matches=0;
    double total_payout=0;
    double hit_rate=0;
    double RTP=0;
};
void process_spin(const std::vector<int>& spin_result, SimulationStats& stats){
    int spin_payout=get_payout(spin_result);
    stats.total_payout+=spin_payout;
    if(spin_payout!=0){
        stats.matches++;
    }
    stats.total_number_of_bets++;
}
SimulationStats run_simulation(const std::vector<std::vector<int>>& reels, int number_of_spins,std::mt19937& gen){
    SimulationStats stats;
    for(int i=0;i<number_of_spins;i++){
        std::vector<int> spin_result = spin(reels,gen);
        process_spin(spin_result,stats);
    }
    stats.hit_rate=static_cast<double>(stats.matches)/stats.total_number_of_bets;
    stats.RTP=stats.total_payout/stats.total_number_of_bets;
    return stats;
}
int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::cout<<"Enter a number of bets: ";
    int number_of_spins;
    std::cin>>number_of_spins;
    SimulationStats stats = run_simulation(reels,number_of_spins,gen);
    std::cout<<"\nTotal bets performed: "<<stats.total_number_of_bets<<"\nHit rate: "<<stats.hit_rate<<"\nRTP: "<<stats.RTP;
    return 0;
}