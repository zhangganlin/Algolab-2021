///5
#include<vector>
#include<iostream>

using namespace std;

void runTest(){
    int n,m; long k;
    cin >> n >> m >> k;
    vector<int> defenders(n);
    for(int i = 0; i < n; i++){
        cin >> defenders[i];
    }
    vector<int> length_end_in(n,0);
    int left = 0;
    int right = 0;
    long sum = defenders[0];

    while(right < n){
        if(sum < k){
            right++;
            if(right == n){break;}
            sum += defenders[right];
        }else if(sum == k){
            length_end_in[right] = right-left+1;
            sum -= defenders[left];
            left ++;
        }else if(sum > k){
            sum -= defenders[left];
            left ++;
        }
    }

    vector<vector<int>> mem(n,vector<int>(m+1,-1));
    for(int j = 1; j<= m; j++){
        mem[0][j] = -1;
    }

    for(int i = 0; i< n; i++){
        mem[i][0] = 0;
    }

    for(int i = 1; i < n; i++){
        for(int j = 1; j <= m; j++){
            int length = length_end_in[i];
            if(i-length < 0 && j == 1){
                mem[i][j] = length;
            }
            else if(i-length<0){ 
                mem[i][j] = mem[i-1][j];
            }
            else if(length==0 || mem[i-length][j-1]==-1){
                mem[i][j] = mem[i-1][j];
            }else{
                mem[i][j] = max(mem[i-1][j], mem[i-length][j-1]+length);
            }
        }
    }

    if(mem[n-1][m] == -1){
        cout << "fail" << endl;
    }else{
        cout << mem[n-1][m] << endl;
    }

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}