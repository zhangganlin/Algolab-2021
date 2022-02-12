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
    int m,n,k,c;
    cin>>m>>n>>k>>c; //node(i,j)=i*m+j
    graph G(m*n*2);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    for(int i =0; i<k; i++){
        int x,y;
        cin >>x>>y;
        adder.add_edge(v_source,(y*m+x)*2,1);
    }
    for(int i = 0; i< n;i++){
        for(int j=0;j<m;j++){
            int in_node = 2*(i*m+j);
            int out_node = 2*(i*m+j)+1;

            adder.add_edge(in_node,out_node,c);

            vertex_desc left_in;
            vertex_desc right_in;
            vertex_desc up_in;
            vertex_desc down_in;

            left_in = out_node-3;
            right_in = out_node+1;
            up_in = out_node-2*m-1;
            down_in = out_node+2*m-1;

            if(i==0){
                up_in = v_sink;
            }
            if(i==n-1){
                down_in = v_sink;
            }
            if(j==0){
                left_in = v_sink;
            }
            if(j==m-1){
                right_in = v_sink;
            }
            adder.add_edge(out_node,left_in,1);
            adder.add_edge(out_node,right_in,1);
            adder.add_edge(out_node,up_in,1);
            adder.add_edge(out_node,down_in,1);

        }
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    cout << flow <<endl;
}


int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i =0; i < t; i++){
        runTest();
    }
    
}