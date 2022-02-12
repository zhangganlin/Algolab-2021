#include <boost/config.hpp>
#include <vector>
#include <list>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iterator>
#include <iostream>

/*find biconnected components which only have one edge in them*/

namespace boost
{
struct edge_component_t
{
    enum
    {
        num = 555
    };
    typedef edge_property_tag kind;
} edge_component;
}

using namespace std;

void runTest(){
    int n,m;
    cin >> n >> m;
    using namespace boost;
    typedef adjacency_list< vecS, vecS, undirectedS, no_property,
        property< edge_component_t, std::size_t > >
        graph_t;
    typedef graph_traits< graph_t >::vertex_descriptor vertex_t;
    graph_t g(n);

    for(int i = 0; i < m; i++){
        int u,v;
        cin >> u >> v;
        add_edge(u,v,g);
    }

    property_map< graph_t, edge_component_t >::type component
        = get(edge_component, g);

    vector<pair<int,int>> outputs;
    std::size_t num_comps = biconnected_components(g, component);
    map<int,int> component_count;

    graph_traits< graph_t >::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){
        if(component_count.find(component[*ei]) == component_count.end()){
            component_count[component[*ei]] = 0;
        }
        component_count[component[*ei]] ++;
    }

    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){
        if(component_count[component[*ei]]==1){
            int u = ei->m_source;
            int v = ei->m_target;
            if (u>v){
                swap(u,v);
            }
            outputs.push_back({u,v});
        }
    }

    sort(outputs.begin(),outputs.end());
    cout << outputs.size()<< endl;
    for(auto p:outputs){
        cout << p.first << " " << p.second << endl;
    }

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}