#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int timer;

bool deactivate(int idx, int n, vector<int>& time, vector<bool>& is_active){
    if(is_active[idx]==false){
        return true;
    }
    if(idx >= (n-1)/2){
        timer += 1;
        is_active[idx] = false;
        return timer<=time[idx];
    }

    bool support1 = deactivate(2*idx+1,n,time,is_active);
    bool support2 = deactivate(2*idx+2,n,time,is_active);

    timer += 1;
    is_active[idx] = false;

    bool success = support1&support2;

    return (timer<=time[idx])&success;
    
}

void runTest(){
    int n; cin >> n;
    vector<pair<int,int>> bombs(n);
    vector<int> time(n);
    timer = 0;
    for(int i = 0; i< n; i++){
        int t;
        cin >> t;
        bombs[i] = {t,i};
        time[i] = t;
    }

    sort(bombs.begin(),bombs.end());
    vector<bool> is_active(n,true);

    bool success = true;
    for(auto bomb:bombs){
        success &= deactivate(bomb.second,n,time,is_active);
    }

    if(success){
        cout << "yes" << endl;
    }else{
        cout << "no" << endl;
    }


}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}