///4
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <boost/pending/disjoint_sets.hpp>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef K::Point_2 P;

struct Edge{
    int u;
    int v;
    long slen;
};

using namespace std;

void runTest(){
    int n,k,f0; long s0;
    cin >> n >> k >> f0 >> s0;
    vector<pair<P,int>> tents;
    tents.reserve(n);
    for(int i = 0; i < n; i++){
        P pos; cin >> pos;
        tents.emplace_back(pos,i);
    }
    Triangulation t(tents.begin(),tents.end());
    vector<Edge> edges;
    for(auto e = t.finite_edges_begin(); e!=t.finite_edges_end(); e++){
        int v1 = e->first->vertex((e->second+1)%3)->info();
        int v2 = e->first->vertex((e->second+2)%3)->info();
        long slen = t.segment(e).squared_length();
        edges.push_back({v1,v2,slen});
    }

    sort(edges.begin(),edges.end(),
        [](Edge& e1, Edge& e2){return e1.slen < e2.slen;});

    boost::disjoint_sets_with_storage<> uf(n);
    vector<int> num_tent_in_cluster(n,1);
    vector<int> num_of_cluster_with_size(k+1,0);

    num_of_cluster_with_size[1] = n;

    long max_s = 0;
    int max_f = 1;

    int f = num_of_cluster_with_size[k];
    int cluster_num = n; 
    for(auto& edge: edges){
        int set1 = uf.find_set(edge.u);
        int set2 = uf.find_set(edge.v);
        if(set1 == set2) continue;
        long s = edge.slen;

        if(s >= s0) max_f = max(f,max_f);
        if(f >= f0) max_s = max(s,max_s);
        
        uf.union_set(set1,set2);
        int new_set = uf.find_set(edge.u);
        int old_set = new_set==set1? set2: set1;
        num_of_cluster_with_size[min(k,num_tent_in_cluster[set1])] --;
        num_of_cluster_with_size[min(k,num_tent_in_cluster[set2])] --;
        num_of_cluster_with_size[min(k,num_tent_in_cluster[set1]+num_tent_in_cluster[set2])] ++;

        num_tent_in_cluster[new_set] += num_tent_in_cluster[old_set];
        num_tent_in_cluster[old_set] = 0;
        
        f = num_of_cluster_with_size[k];
        if(k==2){
            f += num_of_cluster_with_size[1]/2;
        }else if(k==3){
            if(num_of_cluster_with_size[1]<num_of_cluster_with_size[2]){
                f += num_of_cluster_with_size[1];
                f += (num_of_cluster_with_size[2]-num_of_cluster_with_size[1])/2;
            }else{
                f += num_of_cluster_with_size[2];
                f += (num_of_cluster_with_size[1]-num_of_cluster_with_size[2])/3;
            }
        }
        else if(k==4){
            int one = num_of_cluster_with_size[1];
            int two = num_of_cluster_with_size[2];
            int three = num_of_cluster_with_size[3];
            f += two/2; two = two%2; //2+2
            // 1+3
            if(one< three){
                f += one;
                three = three -one;
                f += (three+two)/2;
            }else{
                f += three;
                one =  one -three;
                if(two==1 && one >=2){
                    f += 1;
                    one -= 2;
                }
                f += one/4;
            }
        }
        cluster_num --;
        if(cluster_num == 1){break;}
    }
    cout << max_s << " " << max_f << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}