#include<iostream>
#include<random>
#include<vector>
const int ROWS = 3;
const int COLUMNS = 5;
const int WILD = 0;
const int SCATTER = 7;
const std::vector<std::vector<int>> paylines = {
    {0,0,0,0,0},
    {1,1,1,1,1},
    {2,2,2,2,2},
    {0,1,2,1,0},
    {2,1,0,1,2}
};
const std::vector<std::vector<int>> payout_table={
    {0,0,0,0,30}, //WILD
    {0,0,1,2,4}, //1
    {0,0,1,2,4}, //2
    {0,0,2,4,8}, //3
    {0,0,2,4,8}, //4s
    {0,0,3,5,10}, //5
    {0,0,3,5,10} //6
};
const std::vector<int> scatter_payout_table = {
    0, //0 scatters
    0, //1
    0, //2
    3, //3
    6, //4
    15 //5
};
const std::vector<std::vector<int>> reels ={
    {WILD,WILD,1,1,1,1,2,2,2,2,3,3,3,4,4,5,6,SCATTER},
    {WILD,1,1,1,1,2,2,2,2,3,3,3,4,4,5,6,SCATTER},
    {WILD,1,1,1,1,2,2,2,2,3,3,3,4,4,5,6,SCATTER},
    {WILD,1,1,1,1,2,2,2,2,3,3,3,4,4,5,6,SCATTER},
    {WILD,WILD,1,1,1,1,2,2,2,2,3,3,3,4,4,5,6,SCATTER},
};
int pick_random_stop(const std::vector<int>& reel, std::mt19937& gen) {
    std::uniform_int_distribution<> dist(0, reel.size()-1);
    int stop = dist(gen);
    return stop;
}
int get_base_symbol(const std::vector<int>& payline){ //returns the first non-wild symbol in a payline
    if(!payline.empty()&&payline[0]==SCATTER){
        return SCATTER;
    }
    for (int i=0;i<payline.size();i++){
        if(payline[i]!=WILD && payline[i]!=SCATTER){
            return payline[i];
        }
    }
    return WILD;
}
int count_symbol_on_screen(const std::vector<std::vector<int>>& screen, const int symbol){
    int count = 0;
    for(int i = 0; i<screen.size();i++){
        for(int j = 0; j<screen[0].size();j++){
            if(screen[i][j]==symbol){
                count++;
            }
        }
    }
    return count;
}
int length_of_win_line(const std::vector<int>& spin_result){ 
    int base = get_base_symbol(spin_result);
    if(base == WILD){
        return spin_result.size();
    }
    int length=0;
    for (int i=0;i<spin_result.size();i++){
        if(base == SCATTER){
            if(spin_result[i]==SCATTER){
                length++;
            }
            else{
                break;
            }
        }
        else{
            if(spin_result[i]==base || spin_result[i]==WILD){
            length++;
            }
            else{
                break;
            }
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
int get_line_payout(const std::vector<int>& spin_result){
    int base = get_base_symbol(spin_result);
    if(base==SCATTER){
        return 0;
    }
    return payout_table[base][length_of_win_line(spin_result)-1];
}
int get_scatter_payout(const std::vector<std::vector<int>>& screen, const std::vector<int>& scatter_payout){
    int scatter_count = count_symbol_on_screen(screen,SCATTER);
    return scatter_payout[scatter_count];
}
bool feature_trigger(const std::vector<std::vector<int>>& screen){
    int scatter_count = count_symbol_on_screen(screen,SCATTER);
    if(scatter_count>=3){
        return true;
    }
    return false;
}
struct LineResult {
    std::vector<int> symbols;
    int base_symbol = 0;
    int win_length = 0;
    int payout = 0;
};
LineResult evaluate_line(const std::vector<std::vector<int>>& screen, const std::vector<int>& line_definition){
    LineResult result;
    result.symbols = get_payline(screen,line_definition);
    result.base_symbol = get_base_symbol(result.symbols);
    result.win_length = length_of_win_line(result.symbols);
    result.payout = get_line_payout(result.symbols);
    return result;
}
struct ScreenEvaluation {
    int total_payout = 0;
    int winning_lines = 0;
    int scatter_count = 0;
    int scatter_payout = 0;
    bool free_spins_triggered = false;
    std::vector<LineResult> line_results;
};
ScreenEvaluation evaluate_screen(const std::vector<std::vector<int>>& screen, const std::vector<std::vector<int>>& paylines, const std::vector<int>& scatter_payout){
    ScreenEvaluation result;
    for(int i = 0; i < paylines.size(); i++){
        LineResult line_result = evaluate_line(screen,paylines[i]);
        result.line_results.push_back(line_result);
        result.total_payout += line_result.payout;
        if(line_result.payout!=0){
            result.winning_lines++;
        }
    }
    result.scatter_count = count_symbol_on_screen(screen,SCATTER);
    result.scatter_payout = get_scatter_payout(screen,scatter_payout);
    result.free_spins_triggered = feature_trigger(screen);
    result.total_payout+=result.scatter_payout;
    return result;
}
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

struct SimulationStats {
    int total_number_of_bets=0;
    int matches=0;
    double total_payout=0;
    double hit_rate=0;
    double RTP=0;
    int free_spins_frequency;
};
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

void play_game(const std::vector<std::vector<int>>& reels, std::mt19937& gen){
    std::vector<std::vector<int>> screen = spin_screen(reels,gen);
    print_screen(screen);
    ScreenEvaluation result = evaluate_screen(screen,paylines,scatter_payout_table);
    print_screen_evaluation(result);
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
void perform_simulation(const std::vector<std::vector<int>>& reels, std::mt19937& gen){
    std::cout<<"Enter a number of bets: ";
    int number_of_spins;
    std::cin>>number_of_spins;
    SimulationStats stats = run_simulation(reels,number_of_spins,gen);
    std::cout<<"\nTotal bets performed: "<<stats.total_number_of_bets<<"\nHit rate: "<<stats.hit_rate<<"\nRTP: "<<stats.RTP<<"\nFree spins frequency: "<<stats.free_spins_frequency;
}
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