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
int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    int matches = 0;
    double payout = 0;
    int total_number_of_bets;
    std::cout<<"Enter a number of bets: ";
    std::cin>>total_number_of_bets;
    for(int i=0;i<total_number_of_bets;i++){
        std::vector<int> spin_result;
        for(int j=0;j<reels.size();j++){
            int symbol=pick_random_symbol(reels[j],gen);
            spin_result.push_back(symbol);
        }
        int spin_payout = get_payout(spin_result);
        payout+=spin_payout;
        if(spin_payout!=0){
            matches++;
        }
    }
    double hit_rate;
    hit_rate=total_number_of_bets/matches;
    double RTP;
    RTP=payout/total_number_of_bets;
    std::cout<<"\nTotal bets performed: "<<total_number_of_bets<<"\nHit rate: 1/"<<hit_rate<<"\nRTP: "<<RTP;
    return 0;
}