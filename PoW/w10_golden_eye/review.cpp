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
    int from;
    int to;
    double square_length;
};

bool feasible_task1(long p, vector<Edge>& edges, vector<pair<P,P>>& missions, int n, int m,
 vector<double>& dist_u, vector<double>& dist_v, vector<int>& near_u, vector<int>& near_v){
    boost::disjoint_sets_with_storage<> uf_sub2(n);
    for(auto edge: edges){
        int v1 = edge.from;
        int v2 = edge.to;
        if(edge.square_length<=p){
            uf_sub2.union_set(v1,v2);
        }
    }

    for(int i  = 0; i< m; i++){
        if(dist_u[i] <= p &&
           dist_v[i] <= p &&
           uf_sub2.find_set(near_u[i]) == uf_sub2.find_set(near_v[i])){
            ;
        }else{
            return false;
        }
    }
    return true;
}

bool feasible_task2(long p, vector<Edge>& edges, vector<pair<P,P>>& missions, int n, int m, vector<bool>& success, 
 vector<double>& dist_u, vector<double>& dist_v, vector<int>& near_u, vector<int>& near_v){
    boost::disjoint_sets_with_storage<> uf_sub3(n);
    for(auto edge: edges){
        int v1 = edge.from;
        int v2 = edge.to;
        if(edge.square_length<=p){
            uf_sub3.union_set(v1,v2);
        }
    }

    for(int i  = 0; i< m; i++){
        if(success[i]==false){
            continue;
        }
        if(dist_u[i] <= p &&
           dist_v[i] <= p &&
           uf_sub3.find_set(near_u[i]) == uf_sub3.find_set(near_v[i])){
            ;
        }else{
            return false;
        }
    }
    return true;
}



void runTest(){
    int n,m;
    long p;
    cin >> n >> m >> p;
    vector<pair<P,int>> jammers(n);
    for(int i = 0; i < n; i++){
        P point;
        cin >> point;
        jammers[i] = {point,i};
    }
    vector<pair<P,P>> missions(m);
    for(int i = 0; i< m; i++){
        P from, to;
        cin >> from >> to;
        missions[i] = {from,to};
    }

    Triangulation t;
    t.insert(jammers.begin(),jammers.end());

    vector<bool> success(m,false);
    vector<double> dist_u(m);
    vector<double> dist_v(m);
    vector<int> near_u(m);
    vector<int> near_v(m);
    vector<Edge> edges;

    boost::disjoint_sets_with_storage<> uf_sub1(n);

    for(auto edge = t.finite_edges_begin(); edge != t.finite_edges_end(); edge++){
        auto face = edge->first;
        int v1 = face->vertex((edge->second + 1)%3)->info();
        int v2 = face->vertex((edge->second + 2)%3)->info();
        double square_length = t.segment(edge).squared_length();
        edges.push_back({v1,v2,square_length});
        if(square_length<=p){
            uf_sub1.union_set(v1,v2);
        }
    }

    for(int i  = 0; i< m; i++){
        P from = missions[i].first;
        P to = missions[i].second;
        auto v_from = t.nearest_vertex(from);
        auto v_to = t.nearest_vertex(to);
        dist_u[i] = CGAL::squared_distance(from,v_from->point())*4;
        dist_v[i] = CGAL::squared_distance(to  ,v_to  ->point())*4;
        near_u[i] = v_from->info();
        near_v[i] = v_to->info();
        if(dist_u[i] <= p &&
           dist_v[i] <= p &&
           uf_sub1.find_set(v_from->info()) == uf_sub1.find_set(v_to->info())){
            success[i] = true;
        }
    }

    for(int i = 0; i < m ; i++){
        if(success[i]){
            cout << "y";
        }else{
            cout << "n";
        }
    }
    cout << endl;

    long left = 0;
    long right = LONG_MAX;
    while(left < right){
        long mid = left + (right-left)/2;
        if(feasible_task1(mid,edges,missions,n,m,dist_u,dist_v,near_u,near_v)){
            right = mid;
        }else{
            left = mid+1;
        }
    }
    cout << left << endl;

    left = 0;
    right = p;
    while(left < right){
        long mid = left + (right-left)/2;
        if(feasible_task2(mid,edges,missions,n,m,success,dist_u,dist_v,near_u,near_v)){
            right = mid;
        }else{
            left = mid+1;
        }
    }
    cout << left << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}