// STL includes
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <string>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

typedef set<int> Student;

void runTest(){
    int n,c,f;
    cin >> n >> c >> f;
    unordered_map<string, int> character;
    int character_index = 0; 

    vector<Student> students(n);
    vector<vector<int>> common(n,vector<int>(n,0));


    for(int i = 0; i< n; i++){
        for(int j = 0; j < c; j++){
            string charac;
            cin >> charac;
            if(character.find(charac) == character.end()){
                character[charac] = character_index;
                character_index ++;
            }
            students[i].insert(character[charac]);
            for(int k = 0; k < i; k++){
                if(students[k].count(character[charac])>0){
                    common[i][k]++;
                }
            }
        }
    }

    graph G(n);
    for(int i = 0; i < n; i++){
        for(int k = 0; k < i; k++){
            if(common[i][k]>f){
                boost::add_edge(i,k,G);
            }
        }
    }

    std::vector<vertex_desc> mate_map(n);  // exterior property map
    boost::edmonds_maximum_cardinality_matching(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

    if(matching_size == n/2){
        cout << "not optimal"<<endl;
    }else{
        cout << "optimal"<< endl;
    }

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}
