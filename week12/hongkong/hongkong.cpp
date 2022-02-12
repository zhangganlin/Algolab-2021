#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <queue>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Face_handle Face;
typedef K::Point_2 P;

using namespace std;

const K::FT ft_max = 4*(K::FT)(1l<<51)*(K::FT)(1l<<51);
void fill_triangulation(Triangulation& t){
    auto cmp = [](Face& a, Face& b){return a->info()<b->info();};
    priority_queue<Face,vector<Face>,decltype(cmp)> Q(cmp);
    for(auto f = t.all_faces_begin(); f!=t.all_faces_end();f++){
        if(t.is_infinite(f)){
            f->info() = ft_max;
            Q.push(f);
        }else{
            f->info() = CGAL::squared_radius(f->vertex(0)->point(),
                                             f->vertex(1)->point(),
                                             f->vertex(2)->point());
            Q.push(f);
        }
    }

    while(!Q.empty()){
        auto f = Q.top(); Q.pop();
        K::FT max_length = f->info();
        for(int i = 0; i < 3; i++){
            auto neighbor = f->neighbor(i);
            auto v1 = f->vertex((i+1)%3);
            auto v2 = f->vertex((i+2)%3);
            K::FT possible_length = min(max_length,
                    CGAL::squared_distance(v1->point(),v2->point()));
            if(neighbor->info() < possible_length){
                neighbor->info() = possible_length;
                Q.push(neighbor);
            }
        }
    }
}

void runTest(){
    int n,m;
    long r;
    cin >> n >> m >> r;
    vector<P> trees;
    for(int i = 0; i < n; i++){
        long x,y; cin >> x >> y;
        trees.emplace_back(x,y);
    }
    vector<pair<P,long>> ballons;
    for(int i = 0; i < m; i++){
        long x, y, s; cin >> x >> y >> s;
        ballons.emplace_back(P(x,y),s);
    }

    Triangulation t;
    t.insert(trees.begin(),trees.end());
    fill_triangulation(t);

    for(auto& ballon: ballons){
        P pos = ballon.first;
        K::FT length = K::FT(ballon.second+r)*K::FT(ballon.second+r) ;
        if(CGAL::squared_distance(t.nearest_vertex(pos)->point(),pos)<length){
            cout << "n";
            continue;
        }

        auto f = t.locate(pos);
        if(f->info() >= length*4){
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
