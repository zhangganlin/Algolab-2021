#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<iostream>
#include<vector>
#include<algorithm>
#include<stack>
typedef CGAL::Exact_predicates_inexact_constructions_kernel kernel;
typedef kernel::Ray_2 R;
typedef kernel::Point_2 P;
using namespace std;

struct biker{
    int index;
    long y0;
    long x1;
    long y1;
    kernel::Direction_2 getAbsoluteDirection(){
        long y2=y1;
        if (y1-y0<0){
            y2 = y0 + y0-y1;
        }
        R ray(P(0,y0),P(x1,y2));
        kernel::Direction_2 d = ray.direction();
        return d;
    }
    R getRay(){
        return R(P(0,y0),P(x1,y1));
    }
};

bool compare(biker& b1, biker&b2){
    return b1.y0 > b2.y0;
};




void runTest(){
    int n; cin >> n;
    vector<biker> bikers;
    for (int i =0; i<n;i++){
        long y0,x1,y1;
        cin >> y0>>x1>>y1;
        bikers.push_back({i,y0,x1,y1});
    }

    sort(bikers.begin(),bikers.end(),compare);

    vector<biker> running_bikers;
    
    for (int i =0;i<n;i++){
        bool flag = true;
        while (flag){
            if(running_bikers.empty()){
                running_bikers.push_back(bikers[i]);
                flag = false;
            }
            else{
                biker nearest_biker = running_bikers.back();
                kernel::Direction_2 di = bikers[i].getAbsoluteDirection();
                kernel::Direction_2 dn = nearest_biker.getAbsoluteDirection();

                if (CGAL::do_intersect(bikers[i].getRay(),nearest_biker.getRay())){
                    if ((dn>di)||(dn==di && compare(nearest_biker,bikers[i]))){
                        running_bikers.pop_back();
                    }
                    else{flag = false;}
                }else{
                    running_bikers.push_back(bikers[i]);
                    flag = false;
                }
            }
        }
    }
    vector<int> indeces;
    for(auto biker:running_bikers){
        indeces.push_back(biker.index);
    }
    sort(indeces.begin(),indeces.end());
    
    for (auto idx: indeces){
        cout << idx << " ";
    }
    cout << endl;

}


int main(){
    ios_base::sync_with_stdio(false);
    int t; cin>>t;
    for (int i =0; i<t;i++){
        runTest();
    }
}