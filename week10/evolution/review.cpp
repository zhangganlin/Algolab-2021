#include<iostream>
#include<vector>
#include<algorithm>
#include<map>

using namespace std;

void dfs(int root, vector<pair<int,int>>& path, vector<int>& result, vector<vector<int>>& tree, vector<int>& ages, vector<vector<pair<int,int>>>& query){
    for(auto q:query[root]){
        result[q.second]= (*lower_bound(path.begin(),path.end(),q,[](auto a, auto b){return a.first > b.first;})).second;
    }

    for(auto child: tree[root]){
        path.push_back({ages[child],child});
        dfs(child,path,result,tree,ages,query);
    }
    
    path.pop_back();
}

void runTest(){
    int n,q;
    cin >> n >> q;
    map<string,int> spiece_to_idx;
    vector<string> spieces(n);
    vector<int> ages(n);

    for(int i = 0; i< n; i++){
        string name;
        int age;
        cin >> name >> age;
        spiece_to_idx[name] = i;
        spieces[i] = name;
        ages[i] = age;
    }

    int root = max_element(ages.begin(),ages.end()) - ages.begin();

    vector<vector<int>> tree(n);
    for(int i = 0; i< n-1; i++){
        string s,p;
        cin >> s >> p;
        tree[spiece_to_idx[p]].push_back(spiece_to_idx[s]);
    }
    
  
    
    
    vector<vector<pair<int,int>>> query(n);
    for(int i = 0; i< q; i++){
        string name; cin >> name;
        int b; cin >> b;
        query[spiece_to_idx[name]].push_back({b,i});
    }

    vector<pair<int,int>> path;
    path.push_back({ages[root],root});
    vector<int> result(q);

    dfs(root,path,result,tree,ages,query);

    for(int i  = 0; i< q; i++){
        cout << spieces[result[i]] << " ";
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