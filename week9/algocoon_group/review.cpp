#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

using namespace std;

// Custom edge adder class, highly recommended
class edge_adder {
    graph &G;

    public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void runTest(){
    int n,m;
    cin >> n >> m;
    graph G(n);
    edge_adder adder(G);
    for(int i = 0; i< m; i++){
        int a,b,c;
        cin >> a >> b >> c;
        adder.add_edge(a,b,c);
    }
    long min_cost = INT32_MAX;

    for(int i = 1; i< n; i++){
        long flow1 = boost::push_relabel_max_flow(G, 0, i);
        long flow2 = boost::push_relabel_max_flow(G, i, 0);
        min_cost = min(min_cost,flow1);
        min_cost = min(min_cost,flow2);
    }
    cout << min_cost<< endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >>t;
    while(t--){
        runTest();
    }
}