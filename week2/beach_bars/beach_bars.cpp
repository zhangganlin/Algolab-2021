///3
#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>

using namespace std;

void runTest(){
    int n; cin >> n;
    vector<int> parasols;
    for (int i =0; i<n; i++){
        int temp; cin >> temp;
        parasols.push_back(temp);
    }

    int l = 0, r = 0;
    sort(parasols.begin(),parasols.end());
    

    int max_num = 1;

    vector<vector<int>> positions;
    int single_dist = 300;
    while(r<n){
        int dist_temp = (parasols[r]-parasols[l]+1)/2;
        if(dist_temp > 100){
            l ++;
            continue;
        }
        int num = r-l+1;

        
        if(num > max_num){
            positions.clear();
            max_num = num;
            positions.push_back({parasols[r],parasols[l]});
            single_dist = dist_temp;
        }else if(num == max_num){
            if(dist_temp<single_dist){
                positions.clear();
                positions.push_back({parasols[r],parasols[l]});
                single_dist = dist_temp;
            }else if(dist_temp == single_dist){
                positions.push_back({parasols[r],parasols[l]});
            }
        }
        r++;

    }
    int num_pos = positions.size();
    vector<int> res_positions;
  
    for (int i =0; i<num_pos ;i++){
        int range = positions[i][0]-positions[i][1];
        if(range%2==0){
            res_positions.push_back((positions[i][0]+positions[i][1])/2);
        }else{
            res_positions.push_back(positions[i][1]+single_dist-1);
            res_positions.push_back(positions[i][1]+single_dist);
        }
    }
    
    cout << max_num << " " << single_dist<<endl;

    int pos_num = res_positions.size();
    for (int i =0; i<pos_num-1;i++){
        cout << res_positions[i] <<" ";
    }
    cout<<res_positions[pos_num-1]<<endl;

}

int main(){
    std::ios_base::sync_with_stdio(false);
    
    int t; cin >> t;
    for (int i =0; i<t;i++){
        runTest();
    }
}