#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef std::size_t Index;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef K::Point_2 Point;
typedef std::pair<Point, Index> IPoint;

#include <boost/pending/disjoint_sets.hpp>

using namespace std;

int aux(long s, int n, vector<tuple<Index,Index,long>> edges, vector<pair<int, long> > nn_of_bones) {
    boost::disjoint_sets_with_storage<> uf(n);
    for (auto e : edges) {
        Index c1 = uf.find_set(get<0>(e));
        Index c2 = uf.find_set(get<1>(e));
        if (get<2>(e) <= s) {
            uf.link(c1, c2);
        }
    }

    map<int, int> counts;
    for (auto p : nn_of_bones) {
        if (4 * p.second <= s) {
            counts[uf.find_set(p.first)]++;
        }
    }

    int max_count = 0;
    for (auto p : counts) {
        if (p.second > max_count) {
            max_count = p.second;
        }
    }
    return max_count;
}


void runTest(){
    int n,m;
    long s;
    int k;
    cin >> n >> m >> s >> k;
    vector<IPoint> trees, bones;
    for (Index i = 0;i<n;i++){
        Point p;
        cin >> p;
        trees.emplace_back(p,i); 
    }
    for (Index i = 0;i<m;i++){
        Point p;
        cin >> p;
        bones.emplace_back(p,i+n); 
    }

    Triangulation t;
    t.insert(trees.begin(),trees.end());

    vector<tuple<Index,Index,long>> edges;
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Index i1 = e->first->vertex((e->second+1)%3)->info();
        Index i2 = e->first->vertex((e->second+2)%3)->info();
        edges.emplace_back(i1, i2, long(t.segment(e).squared_length()));
    }

    vector<pair<int, long> > nn_of_bones;
    for (int i = 0; i < m; i++) {
        Point curr = bones[i].first;
        auto nn_ver = t.nearest_vertex(curr);
        int nn_index = nn_ver->info();
        Point nn_pt = nn_ver->point();
        nn_of_bones.emplace_back(nn_index,CGAL::squared_distance(nn_pt,curr));
    }

    int a = aux(s, n, edges, nn_of_bones);

    //binary search on s
    long l = 0, r = LONG_MAX / 2; 
    while (l < r) {
        long mid = (l + r) / 2;
        if (aux(mid, n, edges, nn_of_bones) >= k) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    cout << a << " " << l << endl;
    

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i =0; i< t; i++){
        runTest();
    }
}