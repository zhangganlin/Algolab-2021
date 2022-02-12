#include<algorithm>
#include<iostream>
#include<vector>

using namespace std;

struct Tunnel{
    int length;
    int chamber;
};
struct Chamber{
    long value;
    int total_child_length = 0;
    int total_child_number = 0;
    vector<Tunnel> tunnels;
};

void dfs(int root,vector<Chamber>& chambers){
    Chamber& chamber = chambers[root];

    for(int i = 0; i < chamber.tunnels.size(); i++){
        dfs(chamber.tunnels[i].chamber,chambers);
    }

    for(int i = 0; i < chamber.tunnels.size(); i++){
        chamber.total_child_length += chamber.tunnels[i].length+chambers[chamber.tunnels[i].chamber].total_child_length;
        chamber.total_child_number += 1+chambers[chamber.tunnels[i].chamber].total_child_number;
    }

    std::sort(chamber.tunnels.begin(), chamber.tunnels.end(),
    [&chambers](auto &t1, auto &t2) -> bool {
      const long c1 = chambers[t1.chamber].total_child_length + t1.length;
      const long n1 = chambers[t1.chamber].total_child_number + 1;
      const long c2 = chambers[t2.chamber].total_child_length + t2.length;
      const long n2 = chambers[t2.chamber].total_child_number + 1; 
      return c1*n2 < c2*n1; 
    });

}

long get_value(int root, long t, vector<Chamber>& chambers){
    long value = chambers[root].value - t;
    for(auto& tunnel: chambers[root].tunnels){
        value += get_value(tunnel.chamber, t+tunnel.length, chambers);
        t += 2*(tunnel.length + chambers[tunnel.chamber].total_child_length);
    }
    return value;
}

void runTest(){
    int n; cin >> n;
    vector<Chamber> chambers(n+1);
    for(int i = 1; i <= n; i++){
        long value; cin >> value;
        chambers[i].value = value;
    }
    for(int i = 0; i< n; i++){
        int u,v,l;
        cin >> u >> v >> l;
        chambers[u].tunnels.push_back({l,v});
    }
    dfs(0,chambers);
    cout << get_value(0,0,chambers) << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}