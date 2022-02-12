#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge{
    int v1;
    int v2;
    int weight;
};

int mst(int remove, vector<Edge>& edges, vector<int>& spanning_tree, int n){
    boost::disjoint_sets_with_storage<> uf(n);
    int edge_num = 0;
    int tot_weight = 0;
    for(int i = 0; i < (int)edges.size(); i++){
        if(i == remove){continue;}
        if(uf.find_set(edges[i].v1)!=uf.find_set(edges[i].v2)){
            edge_num ++;
            spanning_tree.push_back(i);
            uf.union_set(edges[i].v1,edges[i].v2);
            tot_weight += edges[i].weight;
            if(edge_num == n-1){
                break;
            }
        }
    }
    return tot_weight;
}

void runTest(){
    int n,start;
    cin >> n >> start;
    vector<Edge> edges;
    for(int j = 1; j <= n-1; j++){
        for(int k = 1; k <= n-j; k++){
            int w; cin >> w;
            edges.push_back({j-1,j+k-1,w});
        }
    }
    sort(edges.begin(),edges.end(),
        [](Edge& a,Edge& b){return a.weight < b.weight;});
    
    vector<int> minimum_spanning_tree;
    mst(-1,edges,minimum_spanning_tree,n);

    vector<int> temp;
    int min_cost = INT32_MAX;
    for(int i = 0; i < (int)minimum_spanning_tree.size(); i++){
        temp.clear();
        int remove = minimum_spanning_tree[i];
        int cost = mst(remove,edges,temp,n);
        min_cost = min(min_cost,cost);
    }

    cout << min_cost << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}