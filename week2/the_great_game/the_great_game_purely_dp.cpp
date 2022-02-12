#include<iostream>
#include<vector>
#include<fstream>
#include <limits>

using namespace std;

int subProblem(int curr_pos, vector<int>&dp, vector<vector<int>>& transitions){ 
    if(dp[curr_pos]!=-1){
        return dp[curr_pos];
    }
    int n = dp.size()-1;
    vector<int> potential_results_this;
    int min_this_step = INT32_MAX;
    for(auto this_step:transitions[curr_pos]){
        if (this_step==n){return 1;}
        int max_next_step = -1;
        for(auto next_step:transitions[this_step]){
            if (dp[next_step]==-1){
                dp[next_step]=subProblem(next_step,dp,transitions);
            }
            if (dp[next_step]+1>max_next_step){
                max_next_step = dp[next_step]+1;
            }
        }
        if(max_next_step+1<min_this_step){
            min_this_step = max_next_step+1;
        }

    }
    dp[curr_pos] = min_this_step;
    return dp[curr_pos];
} //return the steps from position currpos


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

    vector<int> dp_red(n+1,-1);
    vector<int> dp_black(n+1,-1);
    dp_red[n]=0; dp_black[n]=0;

    int red_step = subProblem(red_start,dp_red,transitions);
    int black_step = subProblem(black_start,dp_black,transitions);

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
    int t; 
    ios_base::sync_with_stdio(false);
    cin >> t;
    for(int i = 0; i < t; i++){
        runTest();
    }
}
