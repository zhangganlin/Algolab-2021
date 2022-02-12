#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle  Vertex;

using namespace std;

// s = 4*r*r;
int aux(long s, vector<pair<double,int>>& bones,
        int n, vector<tuple<int,int,double>>& edges){
    boost::disjoint_sets_with_storage<> ds(n);
    map<int,int> count;

    for(auto e:edges){
        if(get<2>(e)<=s){
            ds.link(get<0>(e),get<1>(e));
        }
    }

    for(auto bone:bones){
        if(bone.first * 4 <= s){
            count[ds.find_set(bone.second)]++;
        }
    }

    int max_count = 0;
    for(auto p:count){
        if(p.second>max_count){
            max_count = p.second;
        }
    }

    return max_count;
}


void runTest(){
    int n,m,k;
    long s;
    cin >> n >> m >> s >> k;
    std::vector<pair<K::Point_2,int>> trees(n);
    for(int i = 0; i< n; i++){
        K::Point_2 tree;
        cin >> tree;
        trees[i] = {tree,i};
    }
    Triangulation t;
    t.insert(trees.begin(),trees.end());
    
    std::vector<pair<double,int>> bones(m);
    for(int i = 0; i< m; i++){
        K::Point_2 pos;
        cin >> pos;
        double squared_dist = CGAL::squared_distance(t.nearest_vertex(pos)->point(),pos);
        int index_of_vertex = t.nearest_vertex(pos)->info();
        bones[i] = {squared_dist,index_of_vertex};
    }
    
    vector<tuple<int,int,double>> edges;
    for(auto e = t.finite_edges_begin(); e!=t.finite_edges_end();e++){
        Vertex v1 = e->first->vertex((e->second+1)%3);
        Vertex v2 = e->first->vertex((e->second+2)%3);
        edges.push_back(make_tuple(v1->info(),v2->info(),t.segment(e).squared_length()));
    }


    int a = aux(s,bones,n,edges);

    long l = 0, r = LONG_MAX;
    
    while (l<r){
        long mid = (r+l)/2;
        if(aux(mid,bones,n,edges)>=k){
            r = mid;
        }else{
            l = mid+1;
        }
    }
    cout << a << " " << r << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}