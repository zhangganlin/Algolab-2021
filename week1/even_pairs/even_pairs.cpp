#include <iostream>
#include <vector>

using namespace std;

void runTest(){
    int n; cin >> n;
    vector<int> A;
    int E = 0;
    int O = 0;
    int sum = 0;
    for(int i = 0; i < n; i++){
        int temp;
        cin >> temp;
        A.push_back(temp);
        sum += temp;
        if (sum%2==0){
            E += 1;
        }else{
            O += 1;
        }
    }

    int ans = E*(E-1)/2 + O*(O-1)/2 + E;
    cout << ans << endl;

}

int main(){
    int t; cin >> t;
    for (int i = 0; i < t; i++){
        runTest();
    }
}