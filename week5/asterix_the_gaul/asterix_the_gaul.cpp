#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

long D,T;
int n,m;

//cannot use dp because the complexity, using split and list instead.

bool feasible(vector<pair<long,long>>& first_half,vector<pair<long,long>>& second_half, long p){
    int n1 = first_half.size();
    int n2 = second_half.size();

    vector<pair<long,long>> sub1(1<<n1,{0,0});
    vector<pair<long,long>> sub2(1<<n2,{0,0});

    for(int i = 0; i < 1<<n1; i++){
        int select = 1;
        for(int j = 0; j < n1; j++){
            if((select&i) != 0){
                sub1[i].first += first_half[j].first+p;
                sub1[i].second += first_half[j].second;
            }
            select <<= 1;
        }
    } 

    for(int i = 0; i < 1<<n2; i++){
        int select = 1;
        for(int j = 0; j < n2; j++){
            if((select&i) != 0){
                sub2[i].first += second_half[j].first+p;
                sub2[i].second += second_half[j].second;
            }
            select <<= 1;
        }
    } 

    sort(sub1.begin(),sub1.end(),[](pair<long,long>a,pair<long,long>b){
        if (a.second != b.second){
            return a.second < b.second;
        }else{
            return a.first > b.first;
        }
    });
    sort(sub2.begin(),sub2.end(),[](pair<long,long>a,pair<long,long>b){
        if (a.second != b.second){
            return a.second < b.second;
        }else{
            return a.first > b.first;
        }
    });

    long curr_max = -1;
    for(auto& p: sub1){
        curr_max = max(curr_max,p.first);
        p.first = curr_max;
    }

    curr_max = -1;
    for(auto& p: sub2){
        curr_max = max(curr_max,p.first);
        p.first = curr_max;
    }

    int point1 = 0;
    int point2 = sub2.size()-1;

    while(point1<sub1.size() && point2>=0){
        if(sub1[point1].second + sub2[point2].second >=T){
            point2--;
        }else if(sub1[point1].first + sub2[point2].first <D){
            point1++;
        }else{
            return true;
        }
    }
    return false;

}

void runTest(){
    cin >> n >> m >> D >> T;
    vector<pair<long,long>> first_half;
    vector<pair<long,long>> second_half;
    for(int i = 0; i < n/2; i++){
        long d,t;
        cin >> d >> t;
        first_half.push_back({d,t});
    }
    for(int i = n/2; i < n; i++){
        long d,t;
        cin >> d >> t;
        second_half.push_back({d,t});
    }
    vector<long> potion;
    potion.push_back(0);
    for(int i = 0 ; i < m; i++){
        long s;
        cin >> s;
        potion.push_back(s);
    }



    int left = 0;
    int right = m;

    if (feasible(first_half,second_half,0)) {
            cout << 0 << endl;
            return;
    } 

    if (m>0){
        if (!feasible(first_half,second_half,potion[m])) {
            cout << "Panoramix captured" << endl;
            return;
        } 
    }else{
        if(!feasible(first_half,second_half,0)){
            cout << "Panoramix captured" << endl;
            return;
        }
    }
    

    while(left<right){
        int mid = (left+right)/2;
        if(feasible(first_half,second_half,potion[mid])){
            right = mid;
        }else{
            left = mid+1;
        }
    }
    cout << left << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}