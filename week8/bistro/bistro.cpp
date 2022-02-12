#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

void runTest(int n){
    std::vector<K::Point_2> existings;
    for(int i =0; i< n; i++){
        int x,y;
        cin >> x >> y;
        existings.push_back(K::Point_2(x,y));
    }
    Triangulation t;
    t.insert(existings.begin(),existings.end());

    int m; cin >> m;
    std::vector<K::Point_2> new_positions;
    for(int i =0; i< m; i++){
        int x,y;
        cin >> x >> y;
        new_positions.push_back(K::Point_2(x,y));
    }

    for(int i = 0; i<m;i++){
        K::Point_2 p = new_positions[i];
        K::Point_2 v = t.nearest_vertex(p)->point();
        cout<< CGAL::squared_distance(p,v)<<endl;;
    }
}


int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n!=0){
        runTest(n);
        cin >> n;
    }
}