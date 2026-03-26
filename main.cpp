#include<iostream>
#include<random>
int pick_random_symbol(int reel[], int size, std::mt19937& gen) {
    std::uniform_int_distribution<> dist(0, size - 1);
    int index = dist(gen);
    return reel[index];
}
int payout_table[6][3] ={
    {0,0,10},
    {0,0,10},
    {0,7,14},
    {0,7,14},
    {0,9,18},
    {0,10,20}
};
int length_of_win_line(int symbol1, int symbol2, int symbol3){
    if(symbol1==symbol2&&symbol2!=symbol3){
        return 1;
    }
    else if(symbol1==symbol2&&symbol1==symbol3){
        return 2;
    }
    return 0;
}
int get_payout(int symbol1, int symbol2, int symbol3){
    return payout_table[symbol1-1][length_of_win_line(symbol1,symbol2,symbol3)];
}
int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    int reel1[10]={1,1,1,2,2,3,3,4,5,6};
    int reel2[10]={1,1,1,2,2,3,3,4,5,6};
    int reel3[10]={1,1,1,2,2,3,3,4,5,6};
    int length1=sizeof(reel1)/sizeof(reel1[0]);
    int length2=sizeof(reel2)/sizeof(reel2[0]);
    int length3=sizeof(reel3)/sizeof(reel3[0]);
    int matches = 0;
    double payout = 0;
    int total_number_of_bets;
    std::cout<<"Enter a number of bets: ";
    std::cin>>total_number_of_bets;
    for(int i=0;i<total_number_of_bets;i++){
        int reel_1_symbol=pick_random_symbol(reel1,length1,gen);
        int reel_2_symbol=pick_random_symbol(reel2,length2,gen);
        int reel_3_symbol=pick_random_symbol(reel3,length3,gen);
        int spin_payout = get_payout(reel_1_symbol,reel_2_symbol,reel_3_symbol);
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