#include<set>
#include<vector>
#include<algorithm>
#include<iostream>

using namespace std;

void runTest(){
    int n,m;
    cin >> n >>m;
    vector<int> strengths;
    multiset<int,std::greater<int>> books;
    for(int i = 0; i< n; i++){
        int strength; cin >> strength;
        strengths.push_back(strength);
    }
    for(int i = 0; i< m; i++){
        int book;
        cin >> book;
        books.insert(book);
    }
    sort(strengths.rbegin(),strengths.rend());

    if (strengths[0]< *(books.begin())){
        cout << "impossible" << endl;
        return;
    }

    int turns = 0;
    while(!books.empty()){
        turns+=1;
        for(int i = 0; i< n; i++){
            int strength = strengths[i];
            auto max_book = books.lower_bound(strength);
            if(max_book == books.end()){
                break;
            }else{
                books.erase(max_book);
            }
        }
    }
    cout << turns*3-1 << endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}