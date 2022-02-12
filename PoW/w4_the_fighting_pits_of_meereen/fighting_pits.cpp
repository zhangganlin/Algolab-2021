#include<vector>
#include<iostream>
#include<algorithm>
#include<unordered_map>

using namespace std;

int calculate_bonus(int a, int b, int c){
    return (1 + (b!=0 && c!=b) + (a!=0 && a!=b && a!=c))*1000;
}

void try_insert(unordered_map<uint32_t,int>& next, uint32_t key, int new_score, int curr_score){
    if(new_score<0){return;}
    auto it = next.find(key);
    if(it!=next.end()){
        it->second = max(it->second, curr_score+new_score);
    }else{
        next[key] = curr_score+new_score;
    }
}

void runTest(){
    int n,k,m;
    cin >> n >> k >> m;
    vector<int> fighters(n);
    for(int i = 0; i < n; i++){
        int temp; cin >> temp;
        fighters[i] = temp + 1;
    }
    int diff_offset = 12;
    int mask = m==3? 255:15;

    /*
    map[state] = score
    state: difference|north_history|right_history
            16 bits     8 bits        8 bits
    north/south_history: last_2 | last_1
                         4bits    4bits
    because "difference" can be negative and in [-12,12], 
    add diff_offset = 12 to avoid negative number
    */
    unordered_map<uint32_t,int> current, next;
    next[diff_offset << 16] = 0;

    for(int fighter : fighters){
        swap(current,next);
        next.clear();

        for(auto& state: current){
            uint32_t key = state.first;
            int value = state.second;

            int diff = (key>>16)-diff_offset;
            int south_history = key&255;
            int north_history = (key>>8)&255;

            int new_south_history = ((south_history << 4) + fighter)& mask;
            int new_north_history = ((north_history << 4) + fighter)& mask;

            int south_score = calculate_bonus( south_history&15, (south_history>>4)&15, fighter) - (1<<abs(diff-1));
            int north_score = calculate_bonus( north_history&15, (north_history>>4)&15,  fighter) - (1<<abs(diff+1));

            uint32_t south_key = ( (diff+diff_offset-1) << 16) + (north_history<<8) + new_south_history;
            uint32_t north_key = ( (diff+diff_offset+1) << 16) + (new_north_history<<8) + south_history;

            try_insert(next,south_key,south_score,value);
            try_insert(next,north_key,north_score,value);
        }
    }

    int best= 0;
    for(auto& state:next){
        best = max(best,state.second);
    }
    cout << best << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}