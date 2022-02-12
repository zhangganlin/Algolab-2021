#include<iostream>
#include<utility>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

int n, timer;

bool deactivate(int index, vector<bool>& is_deactivated, unordered_map<int,int> &time){
    if (is_deactivated[index]){return true;};
    if (index>=(n-1)/2){
        timer += 1;
        is_deactivated[index] = true;
        return time[index]>timer;
    }
    bool res = true;
    res &= deactivate(2*index+2,is_deactivated,time);
    res &= deactivate(2*index+1,is_deactivated,time);

    timer+=1;
    is_deactivated[index] = true;
    res &= time[index]>timer;
    return res;

}

void runTest(){
    cin >> n;
    timer = -1;
    unordered_map<int,int> time; //time[index]=t_i;
    vector<pair<int,int>> order_time(n);
    for (int i =0; i< n; i++){
        int ti; cin >> ti;
        time[i]=ti;
        order_time[i] = make_pair(ti,i);
    }
    sort(order_time.begin(),order_time.end());

    vector<bool>is_deactivated(n,false);

    bool success = true;
    for(int i =0; i<n;i++){
        int index = order_time[i].second;
        success &= deactivate(index,is_deactivated,time);
    }

    if(success){
        cout << "yes" <<endl;
    }else{
        cout << "no" << endl;
    }
}


int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >>t;
    for(int i = 0; i< t; i++){
        runTest();
    }
}