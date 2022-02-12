#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <map>

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

struct Request{
    int start;
    int target;
    int start_time;
    int target_time;
    int profit;
};



void runTest(){
    int N,S;
    cin >> N >> S;
    vector<int> initial_cars(S);
    int total_car = 0;
    for(int i = 0; i < S; i++){
        cin >> initial_cars[i];
        total_car += initial_cars[i];
    }
    vector<map<int,int>> time_point(S);
    for(int i = 0; i < S; i++){
        time_point[i][0] = 0;
        time_point[i][100'000] = 0;
    }
    vector<Request> requests;
    for(int i = 0; i < N; i++){
        int s,t,d,a,p;
        cin >> s >> t >> d >> a >> p;
        requests.push_back({s-1,t-1,d,a,p});
        time_point[s-1][d] = 0;
        time_point[t-1][a] = 0;
    }

    int vertex_idx = 0;
    for(int i = 0; i < S; i++){
        for(auto& time: time_point[i]){
            time.second = vertex_idx;
            vertex_idx ++;
        }
    }

    graph G(vertex_idx+2);
    edge_adder adder(G); 
    const int v_source = vertex_idx;
    const int v_sink = vertex_idx+1;

    for(int i = 0; i < S; i++){
        adder.add_edge(v_source,time_point[i][0],initial_cars[i],0);
        adder.add_edge(time_point[i][100'000],v_sink,total_car,0);
        auto last_vertex = time_point[i].begin();
        for(auto vertex = time_point[i].begin(); vertex!=time_point[i].end(); vertex++){
            if (vertex == last_vertex){continue;}
            // imagine virtual depreciation for each car, proportional to time.
            adder.add_edge(last_vertex->second,
                           vertex->second,
                           total_car,
                           100*(vertex->first-last_vertex->first));
            last_vertex = vertex;
        }
    }

    for(auto& request: requests){
        adder.add_edge(time_point[request.start][request.start_time],
                       time_point[request.target][request.target_time],
                       1,
                       100*(request.target_time-request.start_time)-request.profit);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);

    int tot_profit = 100'000*100*total_car - cost;

    cout << tot_profit << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}