#include<iostream>
#include <vector>

using namespace std;

void dfs(int root, vector<vector<int>>& edges, vector<int>& costs, vector<vector<int>>& mem){
    //state 0: self included
    //state 1: valid
    //state 2: child vaild
    
    for(auto child: edges[root]){
        if(mem[child][0] == -1)
            dfs(child,edges,costs,mem);
    }

    int selected_sum = 0;
    int covered_sum = 0;
    int child_covered_sum = 0;
    int min_diff  = INT32_MAX;
    for(auto child:edges[root]){
        selected_sum += mem[child][0];
        covered_sum += mem[child][1];
        child_covered_sum += mem[child][2];
        min_diff = min(min_diff,mem[child][0]-mem[child][1]);
    }

    int selected = costs[root] + child_covered_sum;
    int covered = min(selected,covered_sum + min_diff);
    int child_covered = min(covered,covered_sum);

    mem[root][0] = selected;
    mem[root][1] = covered;
    mem[root][2] = child_covered;

}

void runTest(){
    int n; cin >> n;
    vector<vector<int>> edges(n);
    for(int i = 0; i<n-1; i++){
        int u,v; cin >> u >> v;
        edges[u].push_back(v);
    }
    vector<int> costs(n);
    for(int i  = 0; i< n; i++){
        cin >> costs[i];
    }

    vector<vector<int>> mem(n,vector<int>(3,-1));
    dfs(0,edges,costs,mem);

    cout << mem[0][1] << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}