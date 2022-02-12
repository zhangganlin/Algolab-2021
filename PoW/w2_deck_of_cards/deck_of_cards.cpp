#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

void runTest(){
    int n,k;
    cin >> n >>k;
    vector<int> cards(n);
    int min_idx;
    int min_value = INT32_MAX;
    for(int i = 0; i< n; i++){
        cin >> cards[i];
        if(cards[i]<min_value){
            min_value = cards[i];
            min_idx = i;
        }
    }

    if(k==0){
        cout << min_idx << " " << min_idx << endl;
        return;
    }

    int right = 0;
    int left = 0;
    int sum = cards[0];

    int min_dist = INT32_MAX;
    int res_l,res_r;

    while(right < n){
        if (abs(sum-k)<min_dist){
            min_dist = abs(sum-k);
            res_l = left;
            res_r = right;
        }
        if(sum<k){
            right++;
            if(right==n)break;
            sum += cards[right];
        }else{
            sum -= cards[left];
            left ++;
        }
    }
    cout << res_l << " " << res_r << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}