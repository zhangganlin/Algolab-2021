#include<vector>
#include<iostream>
#include <map>

using namespace::std;

void runTest(){
    int n,k,w;
    cin >> n >> k >> w;
    vector<int> weights(n);
    for(int i = 0; i < n; i++){
        cin >> weights[i];
    }

    vector<vector<int>> waterways(w);
    for(int i = 0; i < w; i++){
        int l; cin >> l;
        for(int j = 0; j < l; j++){
            int rj; cin >> rj;
            waterways[i].push_back(rj);
        }
    }

    int max_plan = 0;

    for(auto& waterway: waterways){
        int temp_sum = weights[waterway[0]];
        int left = 0;
        int right = 0;
        while(right < (int)waterway.size()){
            if(temp_sum < k){
                right ++;
                if(right == (int)waterway.size()) break;
                temp_sum += weights[waterway[right]];
            }else if(temp_sum > k){
                temp_sum -= weights[waterway[left]];
                left ++;
            }else if(temp_sum == k){
                max_plan = max(max_plan, right-left+1);
                temp_sum -= weights[waterway[left]];
                left ++;
            }
        }
    }

    map<int,int> number_of_weight;
    for(auto& waterway: waterways){
        vector<int> partial_sum;
        int temp_sum = 0;
        for(int i = 0; i < waterway.size(); i++){
            temp_sum += weights[waterway[i]];
            if(temp_sum > k) break;
            partial_sum.push_back(temp_sum);
        }

        for(int i = 0; i < partial_sum.size(); i++){
            auto it = number_of_weight.find(k-partial_sum[i]+weights[waterway[0]]);
            if(it!=number_of_weight.end()){
                max_plan = max(max_plan, it->second+i+1);
            }
        }

        for(int i = 0; i < partial_sum.size(); i++){
            auto it = number_of_weight.find(partial_sum[i]);
            if(it!=number_of_weight.end()){
                it->second = max(it->second,i);
            }else{
                number_of_weight[partial_sum[i]] = i;
            }
            
        }

    }

    cout << max_plan << endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t ; cin >> t;
    while(t--){
        runTest();
    }
}