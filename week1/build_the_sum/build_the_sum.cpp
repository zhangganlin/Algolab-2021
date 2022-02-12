#include <iostream>

using namespace std;

int main(void){
    int t;
    cin >> t;
    int n;
    int sum;
    for (int i = 0; i < t; i++){
        cin >> n;
        sum = 0;
        int temp; 
        for (int j = 0; j < n; j++){
            cin >> temp;
            sum += temp;
        }
        cout << sum << endl;
    }
}