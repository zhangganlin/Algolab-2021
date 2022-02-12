#include<iostream>
#include<vector>

using namespace std;

int subResult(int start, int end, vector<int> &coins, vector<vector<int>> &memo);

void runTest(){
    int n; cin >> n;
    vector<int> coins (n,0);
    for(int i =0; i<n;i++){
        cin >> coins[i];
    }

    vector<vector<int>> memo(n,vector<int>(n,-1));

    int res = subResult(0,n-1,coins,memo);
    cout << res << endl;
}

int subResult(int start, int end, vector<int> &coins, vector<vector<int>> &memo){
    if(start == end){
        memo[start][end]=coins[start];
        return memo[start][end];
    }
    if (start == end - 1){
        memo[start][end]=max(coins[start],coins[end]);
        return memo[start][end];
    }

    if (memo[start+2][end]==-1){
        memo[start+2][end]=subResult(start+2,end,coins,memo);
    }
    if (memo[start+1][end-1]==-1){
        memo[start+1][end-1]=subResult(start+1,end-1,coins,memo);
    }
    if (memo[start][end-2]==-1){
        memo[start][end-2]=subResult(start,end-2,coins,memo);
    }
    

    int res1 = coins[start]+min(memo[start+2][end],memo[start+1][end-1]);
    int res2 = coins[end]+min(memo[start][end-2],memo[start+1][end-1]);

    return max(res1,res2);
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i =0; i<t;i++){
        runTest();
    }
}