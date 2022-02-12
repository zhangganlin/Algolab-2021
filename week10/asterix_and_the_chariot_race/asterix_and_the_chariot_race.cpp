#include <iostream> 
#include <vector>
#include <climits>

using namespace std;


void dfs(int root, vector<vector<int> >&G,vector<int>&cost,vector<vector<int>>& DP){

    for(auto child: G[root]){
        if (DP[child][0]==-1){
            dfs(child,G,cost,DP);
        }
    }

    //type 0: min value if root selected
    //type 1: min value if root covered
    //type 2: min value if all child covered

    int selected_sum = 0;
    int covered_sum = 0;
    int not_covered_sum = 0;
    int min_diff_selected_covered = INT32_MAX;
    for(auto child: G[root]){
        selected_sum += DP[child][0];
        covered_sum += DP[child][1];
        not_covered_sum += DP[child][2];
        min_diff_selected_covered = min(min_diff_selected_covered, DP[child][0]-DP[child][1]);
    }

    DP[root][0] = cost[root] + not_covered_sum;
    DP[root][1] = min(DP[root][0], covered_sum+min_diff_selected_covered);
    DP[root][2] = min(DP[root][1],covered_sum);
}

void testcase() {
    int n; cin >> n;
    vector<vector<int> > G(n);
    for (int i = 0; i < n - 1; i++) {
        int from, to; cin >> from >> to;
        G[from].push_back(to);
    }
    vector<int> cost;
    for (int i = 0; i < n; i++) {
        int c; cin >> c;
        cost.push_back(c);
    }

    vector<vector<int> > DP(n, vector<int>(3,-1));
    dfs(0, G, cost, DP);

    cout << DP[0][1] << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}