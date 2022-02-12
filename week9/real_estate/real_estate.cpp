// ALGOLAB BGL Tutorial 3
// Code demonstrating 
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Includes
// ========
#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

void runTest(){
    int N,M,S;
    cin >> N >> M >> S;

    vector<int> num_can_be_sold(S+1,-1);
    for (int i = 1; i < S+1; i++){
        cin >> num_can_be_sold[i];
    }

    vector<int> state_of_site(M+1,-1);
    for (int i = 1; i < M+1; i++){
        cin >> state_of_site[i];
    }

    vector<vector<int>> buyer_i_site_j_price(N+1,vector<int>(M+1,-1));
    for(int i = 1; i < N+1; i++){
        for(int j = 1; j < M+1; j++){
            cin >> buyer_i_site_j_price[i][j];
        }
    }

    graph G(N+M+S+2);
    edge_adder adder(G);
    const int v_source = 0;
    const int v_target = N+M+S+1;
    
    //buyers 1~N, sites N+1~N+M, states N+M+1~N+M+S

    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    for (int buyer = 1; buyer <= N; buyer ++){
        adder.add_edge(v_source, buyer, 1, 0);
        for(int site = N+1; site <= N+M; site++ ){
            adder.add_edge(buyer,site, 1, 100-buyer_i_site_j_price[buyer][site-N]);
        }
    }

    for( int site = N+1; site <= N+M; site++){
        adder.add_edge(site,state_of_site[site-N]+M+N,1,0);
    }

    for(int state = N+M+1; state <= N+M+S; state ++){
        adder.add_edge(state,v_target,num_can_be_sold[state-N-M],0);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);

    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }

    cost = 100*s_flow - cost;

    cout << s_flow << " " << cost << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; i++){
        runTest();
    }
}