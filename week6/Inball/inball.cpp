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

void runTest(int n){
    int d; cin >> d;
    vector<vector<int>> a(n,vector<int>(d,0));
    vector<int> b(n,0);

    for(int i = 0; i<n; i++){
        for(int j = 0; j<d;j++){
            int a_ij;
            cin >> a_ij;
            a[i][j] = a_ij;
        }
        cin >> b[i];
    }

    Program lp (CGAL::SMALLER, false, 0, false, 0);
    for(int i = 0; i<n;i++){
        int sum_square = 0;
        for(int j = 0; j<d;j++){
            lp.set_a(j,i,a[i][j]);
            sum_square += a[i][j]*a[i][j];
        }
        int norm = sqrt(sum_square);
        lp.set_a(d,i,norm);
        lp.set_b(i,b[i]);
    }
    lp.set_l(d,true,0);
    lp.set_c(d,-1);


    Solution s = CGAL::solve_linear_program(lp, ET());

    if(s.is_infeasible()){
        cout << "none" <<endl;
    }else if(s.is_optimal()){
        cout<<-s.objective_value().numerator() / s.objective_value().denominator()<<endl;
    }else if(s.is_unbounded()){
        cout << "inf"<<endl;
    }
}


int main(){
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n!=0){
        runTest(n);
        cin >> n;
    }
}