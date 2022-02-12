#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

// 2D backpack problem on potion A, then just search on potion B

long dp(int i, int j, int k, vector<vector<vector<long>>>& mem, 
    vector<pair<long,int>>& potion_a ){
    if (j==0 && k == 0 ) return 0;
    if (j == 0) return -1;
    if (i<0) return -1;
    if (j<0) return -1;
    

    if(mem[i][j][k]!=-2){
        return mem[i][j][k];
    }
    long pi = potion_a[i].first;
    int hi = potion_a[i].second;
    long used = dp(i-1,j-1,max(0,k-hi),mem,potion_a);
    long not_used = dp(i-1,j,k,mem,potion_a);

    if(used!=-1){
        used += pi;
    }

    mem[i][j][k] = max(used,not_used);
    return mem[i][j][k];
}

void runTest(){
    int n,m; cin >> n >> m;
    long a,b; cin >> a >> b;
    long P,H,W; cin >> P >> H >> W;
    vector<pair<long,int>> potion_a;
    for(int i = 0; i < n; i++){
        long pi; int hi;
        cin >> pi >> hi;
        potion_a.emplace_back(pi,hi);
    }
    vector<long> potion_b;
    potion_b.reserve(m);
    for(int i = 0; i < m; i++){
        long wi; cin >> wi;
        potion_b.push_back(wi);
    }
    sort(potion_b.rbegin(),potion_b.rend());

    vector<vector<vector<long>>> mem(n,vector<vector<long>>(n+1,vector<long>(H+1,-2)));

    vector<long> max_power(n+1);
    for(int i = 0; i <=n; i++){
        max_power[i] = dp(n-1,i,H,mem,potion_a);
    }

    int min_num = INT32_MAX;
    for(int i = 0; i <=n; i++){
        int num_a = i;
        long w = -num_a*a;
        long p = max_power[i];
        if(p==-1){continue;}
        for(int j = 0; j < m; j++){
            int num_b = j+1;
            p -= b;
            w += potion_b[j];
            if(p < P){break;}
            if(w >= W){
                min_num = min(num_a+num_b,min_num);
            }
        }
    }

    if(min_num==INT32_MAX){
        cout << -1 << endl;
    }else{
        cout << min_num << endl;
    }

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >>t;
    while(t--){
        runTest();
    }
}