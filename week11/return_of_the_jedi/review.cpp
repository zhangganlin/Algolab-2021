///1
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <iostream>

using namespace std;

int mst(vector<pair<int,pair<int,int>>>& edges, int edge_removed, int n, vector<int>& result){
    boost::disjoint_sets_with_storage<> ds(n);
    int num_edge = 0;
    for(int i = 0; i < int(edges.size()); i++){
        auto edge = edges[i];
        if(i == edge_removed){continue;}
        if(ds.find_set(edge.second.first)!=ds.find_set(edge.second.second)){
            num_edge++;
            result.push_back(i);
            ds.link(edge.second.first,edge.second.second);
        }
        if(num_edge == n-1){break;}
    }

    int tot_cost = 0;
    for(int i : result){
        tot_cost += edges[i].first;
    }

    return tot_cost;
}

void runTest(){
    int n, start;
    cin >> n >> start;
    vector<pair<int,pair<int,int>>> edges;
    for(int j = 0; j < n-1; j++){
        for(int k = 1; k <= n-1-j; k++){
            int cost;
            cin >> cost;
            edges.push_back({cost,{j,j+k}});
        }
    }
    sort(edges.begin(),edges.end());

    vector<int> mst_edges;
    mst(edges,-1,n,mst_edges);

    int sub_cost = INT32_MAX;

    
    for(int i: mst_edges){
        vector<int> none;
        int temp_sub_cost = mst(edges,i,n,none);
        sub_cost = min(sub_cost,temp_sub_cost);
    }

    cout << sub_cost << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}