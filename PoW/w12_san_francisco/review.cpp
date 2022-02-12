#include <iostream>
#include <vector>
#include <map>

using namespace std;

long dp(int start, int step, vector<vector<pair<int,long>>>& path, vector<vector<long>>& mem){
    if(mem[start][step]!=-1){
        return mem[start][step];
    }

    if(path[start].empty()){
        mem[start][step] = dp(0,step,path,mem);
        return mem[start][step];
    }

    long max_value = 0;
    for(auto p: path[start]){
        int to = p.first;
        long value = p.second;
        max_value = max(max_value,dp(to,step-1,path,mem)+value);
    }
    mem[start][step] = max_value;
    return mem[start][step];
}

void runTest(){
    int n,m,k;
    long x;
    cin >> n >>m >>x >>k;
    vector<vector<pair<int,long>>> path(n);  //{idx,value}
    vector<vector<long>> mem(n,vector<long>(k+1,-1));

    for(int i = 0; i < m; i++){
        int u,v;
        long p;
        cin >> u >> v >>p;
        path[u].push_back({v,p});
    }

    for (int i = 0; i < n; ++i) {
        mem[i][0] = 0;
    }

    for(int i = 0; i <=k; i++ ){
        long value = dp(0,i,path,mem);
        if(value >=x){
            cout << i << endl;
            return;
        }
    }

    cout << "Impossible" << endl;


}

int main(){
    int t;
    cin >> t;
    while(t--){
        runTest();
    }
}