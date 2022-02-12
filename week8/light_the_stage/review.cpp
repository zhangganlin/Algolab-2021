#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

using namespace std;

void runTest(){
    int m,n; cin >> m >> n;
    vector<pair<P,long>> participants;
    for(int i = 0; i< m; i++){
        P pos; long r;
        cin >> pos >> r;
        participants.push_back({pos,r});
    }
    long h; cin >> h;
    vector<P> lamps(n);
    for(int i = 0; i < n; i++){
        cin >> lamps[i];
    }
    Triangulation t;
    t.insert(lamps.begin(),lamps.end());

    vector<int> winners;
    vector<int> die(m,-1);
    int last = -1;

    for(int i =0; i <m ; i++){
        P participant = participants[i].first;
        long radius = participants[i].second;
        if(CGAL::squared_distance(t.nearest_vertex(participant)->point(),participant) >= (radius+h)*(radius+h)){
            winners.push_back(i);
        }else{
            for(int j = 0; j < n; j++){
                if(CGAL::squared_distance(participant,lamps[j]) >= (radius+h)*(radius+h)){
                    die[i] = j;
                    last = max(last,j);
                }else{
                    break;
                }
            }
        }
    }

    if(!winners.empty()){
        for(auto win: winners){
            cout << win << " ";
        }
        cout << endl;
    }else{
        for(int j = 0; j< m; j++){
            if(die[j]==last){
                cout << j << " ";
            }
        }
        cout << endl;
    }

}


int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >>t;
    while(t--){
        runTest();
    }
}