// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include<iostream>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

double floor_to_double(const CGAL::Quotient<ET> &x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) {
        a -= 1;
    }
    while (a+1 <= x) {
        a += 1;
    }
    return a;
}

void runTest(){
    long xs,ys,n;
    cin >> xs >> ys >> n;

    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp (CGAL::SMALLER, false, 0, false, 0); 

    // set the coefficients
    const int X = 0; 
    const int Y = 1;
    const int T = 2;

    for(int i = 0; i < n; i++){
        long ai,bi;
        int ci,vi;
        cin >> ai >> bi >> ci >> vi;
        int sqrt_a2_b2 = (int)sqrt(ai*ai+bi*bi);

        if(ai*xs+bi*ys+ci>0){
            lp.set_a(X,i+n,-ai); lp.set_a(Y,i+n,-bi); lp.set_a(T,i+n,sqrt_a2_b2*vi); lp.set_b(i+n,ci);
        }else{
            lp.set_a(X,i+n,ai); lp.set_a(Y,i+n,bi); lp.set_a(T,i+n,sqrt_a2_b2*vi); lp.set_b(i+n,-ci);
        }
    }

    lp.set_l(T,true,0);
    lp.set_c(T,-1);

    // solve the program, using ET as the exact type
    Solution solution = CGAL::solve_linear_program(lp, ET());

    double res = floor_to_double(-solution.objective_value());
    std::cout << (int)res <<endl;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++){
        runTest();
    }
}