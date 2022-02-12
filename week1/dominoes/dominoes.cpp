#include <iostream>
#include <vector>
#include <stack>
using namespace std;

void runTest(){
    int n; cin >> n;
    vector<int> A;
    vector<int> pos;
    for(int i=0; i<n; i++){
        int temp;
        cin >> temp;
        A.push_back(temp);
        pos.push_back(i+temp-1);
    }

    int temp_pos = 0;
    int temp_length = pos[0];

    while (temp_pos < temp_length){
        temp_pos++;
        if (temp_length>=n){
            cout << n << endl;
            return;
        }
        temp_length = max(pos[temp_pos],temp_length);
    } 

    cout << temp_length+1 << endl;

}

int main(){
    int t; cin >> t;
    for (int i = 0; i < t; i++){
        runTest();
    }
} 