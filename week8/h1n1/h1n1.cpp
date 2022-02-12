#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<double,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Face_handle Face;

typedef K::Point_2 P;

using namespace std;

void fill_triangulation(Triangulation& t){
    auto cmp = [](Face& a, Face& b) { return a->info() < b->info();};
    priority_queue<Face,vector<Face>,decltype(cmp)> Q(cmp);
    for(auto face = t.all_faces_begin(); face!=t.all_faces_end(); face++){
        if(t.is_infinite(face)){
            face->info() = DBL_MAX;
            Q.push(face);
        }else{
            face->info() = 0;
        }
    }

    while(!Q.empty()){
        auto f = Q.top(); Q.pop();
        double max_length = f->info();
        for(int i = 0; i< 3; i++){
            auto v1 = f->vertex((i+1)%3);
            auto v2 = f->vertex((i+2)%3);
            auto neighbor = f->neighbor(i);
            
            double possibale_length = 
                min(CGAL::squared_distance(v1->point(),v2->point()),
                    max_length);
            if(possibale_length > neighbor->info()){
                neighbor->info() = possibale_length;
                Q.push(neighbor);
            }
        }
    }

}

void runTest(int n){
    vector<P> infected(n);
    for(int i  =0; i<n; i++){
        cin >> infected[i];
    }
    int m;
    cin >> m;
    vector<pair<P,long>> query(m);
    for(int i = 0; i < m; i++){
        P pos; long d;
        cin >> pos >> d;
        query[i] = {pos,d};
    }

    Triangulation t;
    t.insert(infected.begin(),infected.end());
    fill_triangulation(t);

    for(int i = 0; i < m; i++){
        P pos = query[i].first;
        long d = query[i].second;
        if(CGAL::squared_distance(pos,t.nearest_vertex(pos)->point())<d){
            cout << 'n';
            continue;
        }
        Face f = t.locate(pos);
        if(f->info() >= d*4){
            cout << "y";
        }else{
            cout << "n";
        }
    }
    cout << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n!=0){
        runTest(n);
        cin >> n;
    }
}