#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Finite_vertices_iterator vertex_iterator;

void runTest(){
    int m,n; cin >> m >> n;
    vector<pair<K::Point_2,long>> participants;
    for (int i = 0; i < m; i++){
        long x,y;
        long r;
        cin >> x >> y >> r;
        K::Point_2 p(x,y);
        participants.push_back(make_pair(p,r));
    }

    long h; cin >> h;
    vector<K::Point_2> lamps;
    for(int i =0; i< n; i++){
        K::Point_2 lamp;
        cin >> lamp;
        lamps.push_back(lamp);
    }

    Triangulation t;
    t.insert(lamps.begin(),lamps.end());


    vector<int> winner;
    
    int last = -1;
    vector<int> first_hit(m,-1);

    for(int i = 0; i < m; i++){
        auto position = participants[i].first;
        auto radius = participants[i].second;

        if(CGAL::squared_distance(position,t.nearest_vertex(position)->point())>=(h+radius)*(h+radius)){
            winner.push_back(i);
        }else{
            for(int j = 0; j < n; j++){
                if((h+radius)*(h+radius) > CGAL::squared_distance(lamps[j], position)) {
                    first_hit[i]  = j;
                    last = std::max(last, j);
                    break;
                }
            }
        }
    }

    if(winner.size()==0){
        for(int i = 0; i < m; i++){
            if(first_hit[i]==last){
                winner.push_back(i);
            }
        }
    }

    for(int i:winner){
        cout << i << " ";
    }

    cout << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i =0; i<t; i++){
        runTest();
    }
    

}