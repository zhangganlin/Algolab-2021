#include <iostream>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

using namespace std;


#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>



void runTest(){
    int n,e,s,a,b;
    cin >> n >> e >> s >> a >> b;
    vector<weighted_graph> graphs(s,weighted_graph(n));

    for(int i = 0; i< e; i++){
        int t1,t2;
        cin >> t1 >> t2;
        for(int j=0; j< s; j++){
            int w; cin >> w;
            boost::add_edge(t1,t2,w,graphs[j]);
        }
    }

    typedef std::vector<vertex_desc> mst;
    vector<mst> preds(s,mst(n));

    for(int i = 0; i < s; i++){
        int hive; cin >> hive;
        boost::prim_minimum_spanning_tree(graphs[i],&preds[i][0],boost::root_vertex(hive));
    }

    weighted_graph G(n);
    weight_map weight_G = boost::get(boost::edge_weight, G);

    for(int i = 0; i < s; i++){
        weight_map weight = boost::get(boost::edge_weight, graphs[i]);
        for(int j = 0; j < n; j++){
            if(j == preds[i][j]){continue;}
            int k = preds[i][j];
            int w = weight[boost::edge(k,j,graphs[i]).first];
            auto edge = boost::edge(k,j,G);
            if(edge.second == false){
                boost::add_edge(k,j,w,G);
            }else if(weight_G[edge.first]>w){
                weight_G[edge.first] = w;
            }
        }
    }

    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, a,
        boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));

    cout << dist_map[b] << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}