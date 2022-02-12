#include <iostream>
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


// using min cost max flow, the begin and end compacity are 2 to represent go and back (equal to 
// find two routes to the destination).
void runTest(){
    int n; cin >> n;
    graph G(n*n*2+2);
    const int v_source = n*n*2;
    const int v_sink = n*n*2+1;
    edge_adder adder(G);  
    int offset = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int score; cin >> score;
            if( (i==0 && j == 0) || (i==n-1 && j == n-1) ){
                offset += score;
                adder.add_edge(i*n+j,i*n+j+n*n,2,100-score);
            }
            else{
                adder.add_edge(i*n+j,i*n+j+n*n,1,100-score);
            }
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i == n-1 && j == n-1){
                adder.add_edge(i*n+j+n*n,v_sink,2,0);
            }else if(i == n-1){
                adder.add_edge(i*n+j+n*n,i*n+j+1,1,0);
            }else if(j == n-1){
                adder.add_edge(i*n+j+n*n,(i+1)*n+j,1,0);
            }else{
                adder.add_edge(i*n+j+n*n,i*n+j+1,1,0);
                adder.add_edge(i*n+j+n*n,(i+1)*n+j,1,0);
            }
        }
    }
    adder.add_edge(v_source,0,2,0);

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);
    int result = (4*n-2)*100 - cost -offset;
    cout << result << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}