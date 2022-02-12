#include<vector>
#include<iostream>
#include<map>
#include<string>
#include<set>
#include<boost/graph/adjacency_list.hpp>
#include<boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;


typedef adjacency_list<vecS,
                       vecS,
                       undirectedS> undirected_graph;
typedef graph_traits<undirected_graph>::vertex_descriptor vertex_desc;


void runTest(){
    int n,c,f;
    cin >> n>>c>>f;
    map<string,int> charac;
    int index_charac = -1;

    vector<set<int>> interests(n,set<int>());

    for(int i = 0; i<n;i++){
        for(int j = 0;j<c;j++){
            string interest;
            cin >> interest;
            if (charac.count(interest) == 0){
                index_charac ++;
                charac[interest] = index_charac;
            }   
            interests[i].insert(charac[interest]);
        }
    }

    undirected_graph G(n);

    for(int i =0;i<n-1;i++){
        for(int j = i+1;j<n;j++){
            int count = 0;
            for(auto interest:interests[i]){
                if (interests[j].count(interest)!=0){
                    count +=1;
                }
            }
            if (count>f){
                add_edge(i,j,G);
            }
        }
    }

    vector<vertex_desc> mate_map(n);

    boost::edmonds_maximum_cardinality_matching(G,
        make_iterator_property_map(mate_map.begin(),get(vertex_index,G)));
    
    int matching_size = boost::matching_size(G,
        make_iterator_property_map(mate_map.begin(),get(vertex_index,G)));

    if(matching_size == n/2){
        cout << "not optimal"<<endl;
    }else{
        cout <<"optimal"<<endl;
    }

}


int main(){
    ios_base::sync_with_stdio(false);
    int t; cin>> t;
    for (int i =0;i<t;i++){
        runTest();
    }
}