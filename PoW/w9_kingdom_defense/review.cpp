#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
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
    int l, p; 
    cin >> l >> p;
    graph G(l+2);
    edge_adder adder(G);
    const int v_source = l;
    const int v_sink = l+1;
    vector<int> from_source(l);
    vector<int> to_sink(l);
    for(int i = 0; i< l; i++){
        cin >> from_source[i] >> to_sink[i];
    }

    for(int i = 0; i < p; i++){
        int u,v;
        int min_c, max_c;
        cin >> u >> v >> min_c >> max_c;
        adder.add_edge(u,v,max_c-min_c);
        from_source[v] += min_c;
        to_sink[u] += min_c;
    }
    long target = 0;
    for(int i = 0; i < l; i++){
        adder.add_edge(v_source,i,from_source[i]);
        adder.add_edge(i,v_sink,to_sink[i]);
        target += to_sink[i];
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    if(flow == target){
        cout << "yes" << endl;
    }else{
        cout << "no" << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}