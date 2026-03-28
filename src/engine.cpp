#include "engine.hpp"
#include "config.hpp"

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
LineResult evaluate_line(const std::vector<std::vector<int>>& screen, const std::vector<int>& line_definition){
    LineResult result;
    result.symbols = get_payline(screen,line_definition);
    result.base_symbol = get_base_symbol(result.symbols);
    result.win_length = length_of_win_line(result.symbols);
    result.payout = get_line_payout(result.symbols);
    return result;
}
int get_awarded_free_spins(const std::vector<std::vector<int>>& screen){
    int scatter_count = count_symbol_on_screen(screen,SCATTER);
    if (scatter_count>=3){
        return BONUS_SPINS;
    }
    else{
        return 0;
    }
}
BonusResult run_free_spins(const std::vector<std::vector<int>>& reels, int number_of_spins, std::mt19937& gen){
    BonusResult result;
    int spins_played = 0;
    for(int i = 0; i<number_of_spins; i++){
        std::vector<std::vector<int>> screen = spin_screen(reels,gen);
        ScreenEvaluation free_spin_eval = evaluate_screen(screen,paylines,scatter_payout_table);
        result.total_payout += free_spin_eval.total_payout;
        spins_played++;
    }
    result.spins_played = spins_played;
    return result;
}
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
    result.awarded_free_spins = get_awarded_free_spins(screen);
    result.total_payout+=result.scatter_payout;
    return result;
}
