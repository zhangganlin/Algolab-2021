#include<iostream>
#include<vector>
#include<fstream>
#include <limits>

using namespace std;

int min_step(int start, int target, vector<vector<int>>& transitions,vector<int>& dp_min,vector<int>& dp_max);
int max_step(int start, int target, vector<vector<int>>& transitions,vector<int>& dp_min,vector<int>& dp_max){
    if(start==target){
        dp_max[start] = 0;
        return 0;
    }
    int res_max = INT32_MIN;
    for(auto next_pos:transitions[start]){
        if (dp_min[next_pos]==-1){
            dp_min[next_pos] = min_step(next_pos,target,transitions,dp_min,dp_max);
        }
        res_max = dp_min[next_pos]>res_max?dp_min[next_pos]:res_max;
    }
    dp_max[start]= res_max+1;
    return dp_max[start];
}
int min_step(int start, int target, vector<vector<int>>& transitions,vector<int>& dp_min,vector<int>& dp_max){
    if(start==target){
        dp_min[start] = 0;
        return 0;
    }
    int res_min = INT32_MAX;
    for(auto next_pos:transitions[start]){
        if (dp_max[next_pos]==-1){
            dp_max[next_pos] = max_step(next_pos,target,transitions,dp_min,dp_max);
        }
        res_min = dp_max[next_pos]<res_min?dp_max[next_pos]:res_min;
    }
    dp_min[start]= res_min+1;
    return dp_min[start];
}


void runTest(){
    int n,m; //position start from 1
    cin >> n >> m;
    int red_start, black_start;
    cin >> red_start >> black_start;
    vector<vector<int>> transitions(n,vector<int>()); //leave transitions[0] empty
    for (int i =0; i < m; i++){
        int temp_u, temp_v;
        cin >> temp_u >> temp_v;
        transitions[temp_u].push_back(temp_v);
    }

    vector<int> dp_min(n+1,-1);
    vector<int> dp_max(n+1,-1);

    int red_step = min_step(red_start,n,transitions,dp_min,dp_max);
    int black_step = min_step(black_start,n,transitions,dp_min,dp_max);

    int total_step_red, total_step_black;

    if (red_step%2==1){
        total_step_red = 1+(red_step/2)*4;
    }else{
        total_step_red = (red_step/2)*4;
    }

    if (black_step%2==1){
        total_step_black = 2 + (black_step/2)*4;
    }else{
        total_step_black = -1 + (black_step/2)*4;
    }

    if (total_step_red<total_step_black){
        cout << 0<<endl;
    }else{
        cout << 1<<endl;
    }

}


int main(){
    ios_base::sync_with_stdio(false);
    int t; 
    cin >> t;
    for(int i = 0; i < t; i++){
        runTest();
    }
}