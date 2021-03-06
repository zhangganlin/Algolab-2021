#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;
                
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

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

void solve() {
  int e, w, m, d, p, l;
  std::cin >> e >> w >> m >> d >> p >> l;
  
  graph G(2*(e+w));
  edge_adder adder(G);
  
  const auto v_source = boost::add_vertex(G);
  const auto v_target = boost::add_vertex(G);
  
  const auto v_free_e = boost::add_vertex(G);
  const auto v_free_w = boost::add_vertex(G);
  
  const int free_matches_e = p - l*e;
  const int free_matches_w = p - l*w;
  
  
  
  adder.add_edge(v_source, v_free_e, free_matches_e, 0);
  
  for (int i = 0; i < e; i++) {
    adder.add_edge(v_source, 2*i, l, 0);
    adder.add_edge(v_free_e, 2*i, free_matches_e, 0);
    adder.add_edge(v_free_e, 2*i+1, free_matches_e, 0);
  }
  
  for (int i = 0; i < m; i++) {
    int u, v, r;
    std::cin >> u >> v >> r;
    adder.add_edge(2*u, 2*(e+v), 1, r);
  }
  
  for (int i = 0; i < d; i++) {
    int u, v, r;
    std::cin >> u >> v >> r;
    adder.add_edge(2*u+1, 2*(e+v)+1, 1, r);
  }
  
  for (int i = e; i < e+w; i++) {
    adder.add_edge(2*i, v_target, l, 0);
    adder.add_edge(2*i, v_free_w, free_matches_w, 0);
    adder.add_edge(2*i+1, v_free_w, free_matches_w, 0);
  }
  
  adder.add_edge(v_free_w, v_target, free_matches_w, 0);
  
  if (free_matches_e < 0 || free_matches_w < 0){
    std::cout << "No schedule!" << std::endl;
    return;
  }
  
  
  if (boost::push_relabel_max_flow(G, v_source, v_target) < p) {
    std::cout << "No schedule!" << std::endl;
  } else {
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    std::cout << boost::find_flow_cost(G) << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t;
  std::cin >> t;
  
  while (t--) {
    solve();
  }
}