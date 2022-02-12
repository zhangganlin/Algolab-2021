#include <iostream>
#include <vector>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
            boost::no_property,
            boost::property<boost::edge_weight_t, double>>   Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor   Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor     Edge;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

const int sec_in_micro = 1000000;

void testcase() {
    // Num planets (vertices), num edges, num warehouses, num teleportation hubs
    int n, m, k, t; std::cin >> n >> m >> k >> t;

    // Init reverse universe graph
    Graph G(n);
    std::vector<int> tele_network;

    // Read in teleportation network
    for(int i = 0; i < t; i++) {
        int t_i; std::cin >> t_i; 
        tele_network.push_back(t_i);
    }

    // Read in edges
    for(int i = 0; i < m; i++) {
        int u, v, c; std::cin >> u >> v >> c;
        // Insert reverse edge
        add_edge(v, u, 2*c, G);
    }

    // Compute connected components
    std::vector<int> scc(n);
    int num_scc = boost::strong_components(G,
        boost::make_iterator_property_map(scc.begin(), boost::get(boost::vertex_index, G)));

    // Compute map cc -> list of vertices s.t. all vertices are in the teleportation network
    std::map<int, std::vector<int>> cc_verts;
    for(auto t_i : tele_network) {
        cc_verts[scc[t_i]].push_back(t_i);
    }

    for(int cc = 0; cc < num_scc; cc++) {
        if(cc_verts[cc].size() > 0) {
            std::vector<int> & reachable = cc_verts[cc];
            for(long unsigned int i = 0; i < reachable.size(); i++) {
                int u = reachable[i];
                boost::add_edge(u, n + cc,reachable.size() - 1, G);
                boost::add_edge(n + cc, u,reachable.size() - 1, G);
            }
        }
    }

    std::vector<int> dist_map(n + num_scc);       // Dijkstra distance map
    Vertex start = n - 1;
    boost::dijkstra_shortest_paths(G, start,
                    boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));

    int min_dist = INT32_MAX;
    for(int i = 0; i < k; i++) {
        int dist_i = dist_map[i]/2;
        min_dist = std::min(dist_i,min_dist);
    }

    if (min_dist > sec_in_micro) {
        std::cout << "no" << std::endl;
    } else {
        std::cout << min_dist << std::endl;
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i =0; i< t;i++){
    testcase();
    }
    return 0;
}