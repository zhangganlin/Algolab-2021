#include<iostream>
#include<vector>

using namespace std;

int subProblem(int start, int end, vector<vector<int>>& mem, vector<int>& coins, int m, int n){
    if(start == end){
        return coins[start];
    }
    if(end < 0){
      return 0;
    }
    if(start >= n){
      return 0;
    }
    if(start > end){
        return 0;
    }

    if(mem[start][end]!=-1){
        return mem[start][end];
    }

    int sub1 = INT32_MAX;
    for(int i = 0; i < m; i++){
        sub1 = min(sub1,subProblem(start+1+i,end-m+1+i,mem,coins,m,n));
    }

    int sub2 = INT32_MAX;
    for(int i = 0; i < m; i++){
        sub2 = min(sub2,subProblem(start+i,end-m+i,mem,coins,m,n));
    }
    
    mem[start][end] = max(coins[start]+sub1, coins[end]+sub2);
    return mem[start][end];
}

void runTest(){
    int n,m,k;
    cin >> n >> m >> k;
    vector<int> coins(n);
    for(int i = 0; i< n; i++){
        cin >> coins[i];
    }

    vector<vector<int>> mem(n,vector<int>(n,-1)); //dp[i][j] from i to j

    int result = INT32_MAX;
    for(int i =0; i<= k; i++){
        int start = i;
        int end = n-k+i-1;
        result = min(result,subProblem(start,end,mem,coins,m,n));
    }
    cout << result << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}