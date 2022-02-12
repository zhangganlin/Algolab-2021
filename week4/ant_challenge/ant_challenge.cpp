
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       property<vertex_distance_t, int>,
                       property<edge_weight_t, int>
                       > weighted_graph;
            
typedef graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef property_map<weighted_graph, vertex_index_t>::type index_map;
typedef property_map<weighted_graph, edge_weight_t>::type edge_weight_map;
typedef property_map<weighted_graph, vertex_distance_t>::type dist_map;

void runTest(){
    int n,e,s,a,b;
    cin >>n>>e>>s>>a>>b;

    vector<weighted_graph> graph_spicies(s,weighted_graph(n));

    for(int i =0;i<e;i++){
        int t1,t2;
        cin >>t1>>t2;
        for(int j = 0;j<s;j++){
            int w;
            cin>>w;
            add_edge(t1,t2,w,graph_spicies[j]);
        }
    }

    vector<vector<vertex_desc>> pred_ver(s,vector<vertex_desc>(n));
    for(int i =0;i<s;i++){
        int h; cin >>h;
        prim_minimum_spanning_tree(graph_spicies[i],&pred_ver[i][0],root_vertex(h));
    }

    weighted_graph G_combined(n);
    edge_weight_map final_weights = get(edge_weight,G_combined);

    for(int i=0;i<s;i++){
        edge_weight_map weights = get(edge_weight, graph_spicies[i]);
        for(int j = 0;j<n;j++){
            int p = pred_ver[i][j];
            if (p!=j){
                int weight = weights[edge(p,j,graph_spicies[i]).first];
                if(!edge(p,j,G_combined).second){
                    add_edge(p,j,weight,G_combined);
                }else if(weight<final_weights[edge(p, j, G_combined).first]){
                    final_weights[edge(p, j, G_combined).first]=weight;
                }
            }
        }
    }

    vector<int> final_dist_map(n);
    dijkstra_shortest_paths(G_combined, a, distance_map(make_iterator_property_map(
                          final_dist_map.begin(), get(vertex_index, G_combined))));
                          
    cout << final_dist_map[b] << endl;

}



int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >>t;
    for (int i =0; i<t;i++){
        runTest();
    }
}



