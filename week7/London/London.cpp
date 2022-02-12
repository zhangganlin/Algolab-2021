///5
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <string>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

using namespace std;

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
    int h,w;
    int n;
    cin >> h >>w;
    string query;
    cin >> query;
    vector<int> letter_count(26,0);
    vector<vector<int>> piece(26,vector<int>(26,0));
    
    n = query.size();
    for(int i = 0; i< n; i++){
        letter_count[query[i]-'A']++;
    }

    vector<vector<pair<int,int>>> paper(h,vector<pair<int,int>>(w,{-1,-1}));

    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            char letter;
            cin >> letter;
            paper[i][j].first = letter-'A';
        }
    }
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            char letter;
            cin >> letter;
            paper[i][w-1-j].second = letter-'A';
        }
    }

    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            int l1 = paper[i][j].first;
            int l2 = paper[i][j].second;
            piece[l1][l2]++;
        }
    }

    graph G(26*26+26+2);
    edge_adder adder(G);
    const int v_source = 26*26+26;
    const int v_sink = 26*26+26+1;

    for(int i = 0; i< 26; i++){
        for(int j =0; j< 26; j++){
            adder.add_edge(v_source,26*i+j,piece[i][j]);
            adder.add_edge(26*i+j,26*26+i,piece[i][j]);
            adder.add_edge(26*i+j,26*26+j,piece[i][j]);
        }
    }
    for(int i = 0; i< 26; i++){
        adder.add_edge(26*26+i,v_sink,letter_count[i]);
    }
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    if(flow == n){
        cout << "Yes" << endl;
    }else{
        cout << "No" << endl;
    }


}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}