// ALGOLAB BGL Tutorial 3
// Flow example demonstrating
// - breadth first search (BFS) on the residual graph

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_residual_bfs.cpp -o bgl_residual_bfs ./bgl_residual_bfs
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_residual_bfs.cpp -o bgl_residual_bfs; ./bgl_residual_bfs

// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;
// Interior Property Maps
typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

using namespace std;

void runTest(){
    int n; cin >> n;
    vector<vector<bool>> chessboard(n,vector<bool>(n,false));
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            bool isValid;
            cin >> isValid;
            chessboard[i][j] = isValid;
        }
    }

    graph G(n*n+2);
    edge_adder adder(G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    const int src = n*n;
    const int sink = n*n+1;

    const int S = 0;
    const int T = 1;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int idx = i*n+j;
            if (!chessboard[i][j]){
                continue;
            }
            if ((i+j)%2==S){
                adder.add_edge(src,idx,1);
            }else{
                adder.add_edge(idx,sink,1);
            }
        }
    }


    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(!chessboard[i][j]){
                continue;
            }

            int idx = i*n+j;
            if((i+j)%2==T){
                continue;
            }

            if(i>=1 && j>=2 && chessboard[i-1][j-2]){
                adder.add_edge(idx,(i-1)*n+j-2,1);
            }
            if(i>=1 && j<=n-3 && chessboard[i-1][j+2]){
                adder.add_edge(idx,(i-1)*n+j+2,1);
            }
            if(i>=2 && j>=1 && chessboard[i-2][j-1]){
                adder.add_edge(idx,(i-2)*n+j-1,1);
            }
            if(i>=2 && j<=n-2 && chessboard[i-2][j+1]){
                adder.add_edge(idx,(i-2)*n+j+1,1);
            }
            if(i<=n-2 && j>=2 && chessboard[i+1][j-2]){
                adder.add_edge(idx,(i+1)*n+j-2,1);
            }
            if(i<=n-2 && j<=n-3 && chessboard[i+1][j+2]){
                adder.add_edge(idx,(i+1)*n+j+2,1);
            }
            if(i<=n-3 && j>=1 && chessboard[i+2][j-1]){
                adder.add_edge(idx,(i+2)*n+j-1,1);
            }
            if(i<=n-3 && j<=n-2 && chessboard[i+2][j+1]){
                adder.add_edge(idx,(i+2)*n+j+1,1);
            }

        }
    }

    int flow = boost::push_relabel_max_flow(G, src, sink);

  std::vector<int> vis(n*n+2, false); // visited flags
  std::queue<int> Q; // BFS queue (from std:: not boost::)
  vis[src] = true; // Mark the source as visited
  Q.push(src);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // Only follow edges with spare capacity
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }
  }  

    int count = 0;
    
    
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          
          // int debug_flag = false;
          
            if ((i+j) % 2 == S) {
                if (chessboard[i][j] && vis[i*n+j]) {
                    count++;
                    // debug_flag = true;
                }
            } else {
                if (chessboard[i][j] && !vis[i*n+j]) {
                    count++;
                    // debug_flag = true;
                }
            }
            
            // if(debug_flag) cout <<"x ";
            // else cout <<"o ";
        }
        // cout << endl;
    }

    cout << count << endl;

  

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++){
        runTest();
    }
}