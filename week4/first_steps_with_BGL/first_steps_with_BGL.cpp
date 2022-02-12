#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <iostream>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
                              weighted_graph;

typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

void runTest(){
    int n,m;
    std::cin>>n>>m;
    weighted_graph G(n);
    for (int i = 0;i < m; i++){
        int v1,v2, w;
        std::cin>>v1>>v2>>w;
        boost::add_edge(v1,v2,w,G);
    }

    std::vector<edge_desc> mst; //vector to store MST edges
    boost::kruskal_minimum_spanning_tree(G,std::back_inserter(mst));


    boost::property_map < weighted_graph,
                      boost::edge_weight_t >::type EdgeWeightMap = get(boost::edge_weight, G);
    int total_weight_in_mst = 0;
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); it++){
        total_weight_in_mst += EdgeWeightMap[*it];
    }


    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, 0, 
        boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),boost::get(boost::vertex_index,G)))
    );
    int max_dist = *std::max_element(dist_map.begin(), dist_map.end());
    std::cout << total_weight_in_mst << " " << max_dist<<std::endl;
}


int main(){
    std::ios_base::sync_with_stdio(false);
    int t; std::cin>>t;
    for (int i=0;i<t;i++){
        runTest();
    }
}