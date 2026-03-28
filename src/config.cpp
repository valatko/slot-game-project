#include "config.h"

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
    {0,0,2,4,8}, //4
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
