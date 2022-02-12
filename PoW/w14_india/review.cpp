#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

using namespace std;
    int c,g,k,a;
    long b;

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

bool feasible(long expected_flow, graph& G){
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    auto edge = boost::edge(c,k,G).first;
    c_map[edge] = expected_flow;
    boost::successive_shortest_path_nonnegative_weights(G, c, a);
    int cost = boost::find_flow_cost(G);

    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(c,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }

    if(s_flow == expected_flow && cost <= b){
        return true;
    }else{
        return false;
    }
}

void runTest(){
    cin >> c >> g >> b >> k >> a;
    graph G(c+1);
    edge_adder adder(G);  
    const int v_source = c;
    

    long max_result = 0;

    for(int i = 0; i < g; i++){
        int x,y,d,e;
        cin >> x >> y >> d >> e;
        adder.add_edge(x,y,e,d);
        if(y == a){
            max_result += e;
        }
    }
    adder.add_edge(v_source,k,1,0);

    long left = 0;
    long right = max_result;

    while(left<right){
        long mid = left + (right-left+1)/2;
        if(feasible(mid,G)){
            left = mid;
        }else{
            right = mid-1;
        }
    }

    cout << left << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}