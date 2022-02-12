#include<iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <vector>
using namespace std;

typedef int IT;
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void runTest(int n, int m){
    vector<int> mins(n,0);
    vector<int> maxs(n,0);
    vector<vector<int>> C(m,vector<int>(n,0));
    vector<int> price(m,0);

    for(int i =0; i<n;i++){
        int min_i,max_i;
        cin >> min_i >> max_i;
        mins[i] = min_i;
        maxs[i] = max_i;
    }

    for(int i =0; i<m;i++){
        int p_i; cin >>p_i;
        price[i] = p_i;
        for(int j = 0; j<n;j++){
            cin >> C[i][j];
        }
    }

    Program lp (CGAL::SMALLER, true, 0, false, 0);
    for(int i = 0; i<n;i++){
        for(int j = 0; j<m;j++){
            lp.set_a(j,i,C[j][i]);
            lp.set_a(j,i+n,-C[j][i]);
        }
        lp.set_b(i,maxs[i]);
        lp.set_b(i+n,-mins[i]);
    }


    for(int j = 0;j<m;j++){
        lp.set_c(j,price[j]);
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());

    if(s.is_infeasible()){
        cout << "No such diet." <<endl;
    }else{
        cout<<s.objective_value().numerator() / s.objective_value().denominator()<<endl;
    }
}


int main(){
    ios_base::sync_with_stdio(false);
    int n,m; cin >> n>>m;
    while((n!=0)&&(m!=0)){
        runTest(n,m);
        cin >> n>>m;
    }
}