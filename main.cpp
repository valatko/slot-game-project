#include<iostream>
#include<random>
#include<vector>
const int ROWS = 3;
const int COLUMNS = 3;
const int WILD = 0;
const std::vector<std::vector<int>> paylines = {
    {0,0,0},
    {1,1,1},
    {2,2,2},
    {0,1,2},
    {2,1,0}
};
const std::vector<std::vector<int>> payout_table={
    {0,0,10}, //WILD
    {0,0,1}, //1
    {0,0,1}, //2
    {0,0,3}, //3
    {0,0,3}, //4
    {0,2,5}, //5
    {0,2,5} //6
};
const std::vector<std::vector<int>> reels ={
    {WILD,1,1,1,1,2,2,2,2,3,3,3,4,4,5,6},
    {WILD,1,1,1,1,2,2,2,2,3,3,3,4,4,5,6},
    {WILD,1,1,1,1,2,2,2,2,3,3,3,4,4,5,6},
};
int pick_random_stop(const std::vector<int>& reel, std::mt19937& gen) {
    std::uniform_int_distribution<> dist(0, reel.size()-1);
    int stop = dist(gen);
    return stop;
}
int get_base_symbol(const std::vector<int>& payline){ //returns the first non-wild symbol in a payline
    for (int i=0;i<payline.size();i++){
        if(payline[i]!=WILD){
            return payline[i];
        }
    }
    return WILD;
}
int length_of_win_line(const std::vector<int>& spin_result){ 
    int base = get_base_symbol(spin_result);
    if(base == WILD){
        return spin_result.size();
    }
    int length=0;
    for (int i=0;i<spin_result.size();i++){
        if(spin_result[i]==base || spin_result[i]==WILD){
            length++;
        }
        else{
            break;
        }
    }
    return length;
}
std::vector<int> get_visible_window(const std::vector<int>& reel, int stop_index){
    std::vector<int> window;
    for (int i=0; i<ROWS;i++){
        window.push_back(reel[(stop_index+i)%reel.size()]);
    }
    return window;
}
std::vector<std::vector<int>> spin_screen(const std::vector<std::vector<int>>& reels, std::mt19937& gen){
    std::vector<std::vector<int>> screen;
    for(int i=0;i<reels.size();i++){
        int stop = pick_random_stop(reels[i],gen);
        std::vector<int> window = get_visible_window(reels[i],stop);
        screen.push_back(window);
    }
    return screen;
}
std::vector<int> get_payline(const std::vector<std::vector<int>>& screen,const std::vector<int>& line_definition){
    std::vector<int> payline;
    for(int i=0;i<line_definition.size();i++){
        payline.push_back(screen[i][line_definition[i]]);
    }
    return payline;
}
int get_payout(const std::vector<int>& spin_result){
    int base = get_base_symbol(spin_result);
    return payout_table[base][length_of_win_line(spin_result)-1];
}
int get_total_payout_for_screen(const std::vector<std::vector<int>>& screen, const std::vector<std::vector<int>>& paylines){
    int total_payout = 0;
    for(int i=0;i<paylines.size();i++){
        std::vector<int> payline = get_payline(screen,paylines[i]);
        int line_payout = get_payout(payline);
        total_payout += line_payout;
    }
    return total_payout;
}

struct SimulationStats {
    int total_number_of_bets=0;
    int matches=0;
    double total_payout=0;
    double hit_rate=0;
    double RTP=0;
};
void process_spin(const std::vector<std::vector<int>>& spin_screen, SimulationStats& stats){
    int spin_payout=get_total_payout_for_screen(spin_screen,paylines);
    stats.total_payout+=spin_payout;
    if(spin_payout!=0){
        stats.matches++;
    }
    stats.total_number_of_bets++;
}
SimulationStats run_simulation(const std::vector<std::vector<int>>& reels, int number_of_spins,std::mt19937& gen){
    SimulationStats stats;
    for(int i=0;i<number_of_spins;i++){
        std::vector<std::vector<int>> spin_result = spin_screen(reels,gen);
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