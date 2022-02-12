#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_vertices_iterator vertex_iterator;

void runTest(int n){
    std::vector<K::Point_2> positions;
    int l,b,r,t;
    cin >> l >> b >> r >> t;
    for(int i = 0; i < n; i++){
        K::Point_2 p;
        cin >> p;
        positions.push_back(p);
    }
    Triangulation triangulation;
    triangulation.insert(positions.begin(),positions.end());
    
    vertex_iterator v_iter;
    Triangulation::Vertex_circulator v_cir;

    vector<double> dists;

    for(v_iter = triangulation.finite_vertices_begin(); 
        v_iter != triangulation.finite_vertices_end();v_iter++){
        
        double min_dist = DBL_MAX;

        Triangulation::Vertex_circulator v_cir;
        v_cir = triangulation.incident_vertices(v_iter);
        if(v_cir !=0){
            do{
                if(!triangulation.is_infinite(v_cir)){
                    min_dist = min(sqrt(CGAL::squared_distance(v_iter->point(),v_cir->point()))/2.0,min_dist);
                }
                v_cir ++;
            }while(v_cir != triangulation.incident_vertices(v_iter));
        }

        min_dist = min(v_iter->point().x()-l,min_dist);
        min_dist = min(r-v_iter->point().x(),min_dist);
        min_dist = min(v_iter->point().y()-b,min_dist);
        min_dist = min(t-v_iter->point().y(),min_dist);

        dists.push_back(min_dist);
    }

    sort(dists.begin(),dists.end());
    int f = (int)ceil(sqrt(dists[0]-0.5));
    int m = (int)ceil(sqrt(dists[n/2]-0.5));
    int last = (int)ceil(sqrt(dists[n-1]-0.5));    

    cout << f << " " << m << " " << last << endl;
    
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