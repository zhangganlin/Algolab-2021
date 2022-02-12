#include<iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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
    int n,m,s;
    cin >> n>>m>>s;
    graph G(n+s);
    edge_adder adder(G);
    vector<int> stores(s,0);
    for(int i =0; i< s; i++){
        cin >> stores[i];
    }

    const vertex_desc v_source = 0;
    const vertex_desc v_sink = boost::add_vertex(G);

    for(int i =0; i< m;i++){
        int v1,v2;
        cin >> v1>>v2;
        if(v1==v_source){
            adder.add_edge(v1,v2,1);
        }else if(v2==v_source){
            adder.add_edge(v2,v1,1);
        }else{
            adder.add_edge(v1,v2,1);
            adder.add_edge(v2,v1,1);
        }
    }

    for(int i = 0; i<s ;i++){
        adder.add_edge(stores[i],n+i,1);
        adder.add_edge(n+i,v_sink,1);
    }
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    if(flow==long(stores.size())){
        cout << "yes" << endl;
    }else{
        cout << "no" << endl;
    }
}


int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i =0; i < t; i++){
        runTest();
    }
    
}