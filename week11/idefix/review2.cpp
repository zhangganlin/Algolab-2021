#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;


typedef K::Point_2 P;


using namespace std;

struct Edge{
    int v1;
    int v2;
    double sqr_length;
};

int aux(long s, vector<Edge>& edges, vector<pair<double,int>>& bones, int n){
    boost::disjoint_sets_with_storage<> uf(n);
    for(auto edge:edges){
        if(edge.sqr_length<=s){
            uf.union_set(edge.v1,edge.v2);

        }
    }

    map<int,int> count;
    for(auto bone:bones){
        if(bone.first*4<=s){
            int cluster = uf.find_set(bone.second);
            // count[cluster] ++;
            if(count.find(cluster) == count.end()){
                count[cluster] = 1;
            }else{
                count[cluster] += 1;
            }
        }
    }

    int max_count = 0;
    for(auto p:count){
        if(p.second>max_count){
            max_count = p.second;
        }
    }

    return max_count;

    int max_num = 0;
    for(auto cluster: count){
        max_num = max(max_num,cluster.second);
    }

    return max_num;

}

void runTest(){
    int n,m,k;
    long s;
    cin >> n >> m >> s >> k;
    vector<pair<P,int>> oaks(n);
    for(int i = 0; i<n; i++){
        P pos; cin >> pos;
        oaks[i] = {pos,i};
    }

    Triangulation t;
    t.insert(oaks.begin(),oaks.end());

    vector<pair<double,int>> bones(m);
    for(int i = 0; i< m; i++){
        P pos; cin >> pos;
        auto vertex = t.nearest_vertex(pos);
        double sqr_length = CGAL::squared_distance(vertex->point(),pos);
        bones[i] = {sqr_length,vertex->info()};
    }


    
    vector<Edge> edges;
    for(auto edge = t.finite_edges_begin(); edge!=t.finite_edges_end();edge++){
        double d =t.segment(edge).squared_length();
        int v1 = edge->first->vertex((edge->second+1)%3)->info();
        int v2 = edge->first->vertex((edge->second+2)%3)->info();
        edges.push_back({v1,v2,d});
    }

    int a = aux(s,edges,bones,n);
    
    long left = 0;
    long right = INT64_MAX;

    while(left < right){
        long mid = left+(right-left)/2;
        if(aux(mid,edges,bones,n)>=k){
            right = mid;
        }else{
            left  = mid+1;
        }
    }
    cout << a <<" " << left << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}