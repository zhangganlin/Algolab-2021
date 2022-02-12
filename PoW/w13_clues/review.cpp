#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::Point_2 P;
using namespace std;

#include <boost/graph/adjacency_list.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>  graph;
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

/*only need to search the first and second nearest vertex to detect whether the graph
 is bipartite. 
 How to find second nearest vertex: 
   https://stackoverflow.com/questions/34724197/cgal-delaunay-triangulation-2nd-closest-neighbour*/
void build_graph(graph& G, Triangulation& t, long r_sqr){
    for(auto v = t.finite_vertices_begin(); v!=t.finite_vertices_end(); v++){
        auto v1 = t.incident_vertices(v);
        do{
            if(!t.is_infinite(v1) && CGAL::squared_distance(v1->point(),v->point()) <= r_sqr){
                boost::add_edge(v1->info(), v->info(), G);
            
                auto v2 = t.incident_vertices(v1);
                do{
                    if(!t.is_infinite(v2) && v->info()!=v2->info() && CGAL::squared_distance(v2->point(),v->point()) <= r_sqr){
                        boost::add_edge(v2->info(), v->info(), G);
                    }
                }while(++v2 != t.incident_vertices(v1));
            }
        }while(++v1 != t.incident_vertices(v));
    }
}

bool in_range(const K::Point_2& a, const K::Point_2& b, long r) {
  return (CGAL::squared_distance(a, b) <= CGAL::square(r));
}

void runTest(){
    int n,m;
    long r;
    cin >> n >> m >> r;
    long r_sqr = r*r;
    vector<pair<P,int>> stations(n);
    vector<P> Holmes(m);
    vector<P> Watson(m);
    for(int i = 0; i < n; i++){
        P pos;
        cin >> pos;
        stations[i] = {pos,i};
    }
    for(int i = 0; i < m; i++){
        cin >> Holmes[i];
        cin >> Watson[i];
    }
    Triangulation t;
    t.insert(stations.begin(),stations.end());
    graph network(n);


    build_graph(network,t,r_sqr);



    bool is_bipartite = boost::is_bipartite(network);
    if(!is_bipartite){
        for(int i = 0; i < m; i++){
            cout << "n";
        }
        cout << endl;
        return;
    }

    std::vector<int> component_map(n);
    int ncc = boost::connected_components(network, 
        boost::make_iterator_property_map(component_map.begin(), 
                                          boost::get(boost::vertex_index, network)));    
    

    for(int i = 0; i < m; i++){
        if (CGAL::squared_distance(Holmes[i], Watson[i]) <= r_sqr) {
            cout << 'y';
            continue;
        }
        auto start = t.nearest_vertex(Holmes[i]);
        auto end = t.nearest_vertex(Watson[i]);
        double dist1 = CGAL::squared_distance(start->point(),Holmes[i]);
        double dist2 = CGAL::squared_distance(end->point(),Watson[i]);
        if(dist1<=r_sqr && 
           dist2<=r_sqr && 
           component_map[start->info()]==component_map[end->info()]){
            cout << "y";
        }else{
            cout << "n";
        }
    }
    cout << endl;


}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}
