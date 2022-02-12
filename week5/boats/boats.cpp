#include<vector>
#include<iostream>
#include<algorithm>

using namespace std;

void runTest(){
    int n; cin>>n;
    vector<pair<int,int>> boats(n);
    for(int i = 0; i< n; i++){
        int length, position;
        cin >> length >> position;
        boats[i] = {position,length};
    }
    sort(boats.begin(),boats.end());
    int result = 0;

    int last_head_pos = INT32_MIN;
    int last_tail_pos = INT32_MIN;
    for(int i = 0; i< n; i++){
        auto boat = boats[i];
        if(last_tail_pos <= boat.first-boat.second){
            last_tail_pos = boat.first;
            last_head_pos = boat.first-boat.second;
            result ++;
        }else if(last_tail_pos<=boat.first){
            last_head_pos = last_tail_pos;
            last_tail_pos = last_tail_pos+boat.second;
            result++;
        }else if(last_tail_pos > boat.first){
            int temp_tail = max(last_head_pos+boat.second, boat.first);
            if(temp_tail<=last_tail_pos){
                last_tail_pos = temp_tail;
                last_head_pos = last_tail_pos-boat.second;
            }
        }
    }
    cout << result << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}