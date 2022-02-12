#include <iostream>
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
    int l,p;
    cin>>l>>p; //node(i,j)=i*m+j
    graph G(l);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    vector<int> source_to_v(l,0);
    vector<int> v_to_sink(l,0);
    long total_to_sink = 0;
    for(int i =0; i<l; i++){
        int g,d;
        cin >>g>>d;
        source_to_v[i] = g;
        v_to_sink[i] = d;
        
    }
    for(int i = 0; i< p;i++){
        int v1,v2,path_min,path_max;
        cin >> v1 >> v2 >> path_min >> path_max;
        adder.add_edge(v1,v2,path_max-path_min); 
        v_to_sink[v1] += path_min;
        source_to_v[v2] += path_min;
    }
    
    for(int i = 0; i < l ;i++){
        adder.add_edge(v_source,i,source_to_v[i]);
        adder.add_edge(i,v_sink,v_to_sink[i]);
        total_to_sink += v_to_sink[i];
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    if (total_to_sink==flow){
        std::cout << "yes" << endl;
    }else{
        std::cout << "no" << endl;
    }
}


int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i =0; i < t; i++){
        runTest();
    }   
}
