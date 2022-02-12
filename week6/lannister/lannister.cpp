#include<iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <vector>
using namespace std;

typedef long IT;
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(const CGAL::Quotient<ET>& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a++;
  while (a-1 >= x) a--;
  return a;
}

void runTest(){
    int n,m;
    long s;
    cin >> n >> m >> s;
    vector<vector<long>> nobles(n,vector<long>(2,0));
    vector<vector<long>> commons(m,vector<long>(2,0));
    for(int i =0; i< n; i++){
        cin>>nobles[i][0]>>nobles[i][1];
    }
    for(int i =0; i< m; i++){
        cin>>commons[i][0]>>commons[i][1];
    }

    int const a = 0;
    int const b = 1;
    int const c1 = 2;
    int const c2 = 3;
    int const min_len = 4;

    Program lp (CGAL::SMALLER, false, 0, false, 0);

    lp.set_l(a,true,1);
    lp.set_u(a,true,1);

    lp.set_c(min_len,1);

    int constrain_counter = 0;

    //Cersei's constrain
    for(int i = 0; i<n;i++){
        lp.set_a(a,constrain_counter,nobles[i][0]);
        lp.set_a(b,constrain_counter,nobles[i][1]);
        lp.set_a(c1,constrain_counter,1);
        constrain_counter ++;
    }
    for(int i =0; i<m;i++){
        lp.set_a(a,constrain_counter,-commons[i][0]);
        lp.set_a(b,constrain_counter,-commons[i][1]);
        lp.set_a(c1,constrain_counter,-1);
        constrain_counter ++;
    }

    //Jaime's constrain
    //-min*a <= ay-bx+c2 <= min*a
    for(int i =0; i<n;i++){
        // 1) ay-bx+c2-min*a <= 0
        lp.set_a(a, constrain_counter, nobles[i][1]);
        lp.set_a(b, constrain_counter, -nobles[i][0]);
        lp.set_a(c2, constrain_counter, 1);
        lp.set_a(min_len, constrain_counter, -1);
        constrain_counter++;
        // 2)-ay+bx-c2-min*a <= 0
        lp.set_a(a, constrain_counter, -nobles[i][1]);
        lp.set_a(b, constrain_counter, nobles[i][0]);
        lp.set_a(c2, constrain_counter, -1);
        lp.set_a(min_len, constrain_counter, -1);
        constrain_counter++;
    }
    for(int i =0; i<m;i++){
        // 1) ay-bx+c2-min*a <= 0
        lp.set_a(a, constrain_counter, commons[i][1]);
        lp.set_a(b, constrain_counter, -commons[i][0]);
        lp.set_a(c2, constrain_counter, 1);
        lp.set_a(min_len, constrain_counter, -1);
        constrain_counter++;
        // 2)-ay+bx-c2-min*a <= 0
        lp.set_a(a, constrain_counter, -commons[i][1]);
        lp.set_a(b, constrain_counter, commons[i][0]);
        lp.set_a(c2, constrain_counter, -1);
        lp.set_a(min_len, constrain_counter, -1);
        constrain_counter++;
    }

    Solution solution = CGAL::solve_linear_program(lp, ET());
    if(solution.is_infeasible()){
        cout<<"Yuck!"<<endl;
        return;
    }

    long x_sum = 0;
    long y_sum = 0;
    long const_sum = 0;

    for(int i =0; i<n;i++){
        x_sum -= nobles[i][0];
        y_sum -= nobles[i][1];
        const_sum -= 1;
    }
    for(int i =0; i<m;i++){
        x_sum += commons[i][0];
        y_sum += commons[i][1];
        const_sum += 1;
    }

    if(s>=0){
        lp.set_a(a,constrain_counter,x_sum);
        lp.set_a(b,constrain_counter,y_sum);
        lp.set_a(c1,constrain_counter,const_sum);
        lp.set_b(constrain_counter,s);

        solution = CGAL::solve_linear_program(lp, ET());
        if (solution.is_infeasible()) {
            std::cout << "Bankrupt!" << std::endl;
            return;
        }
    }

    double res = ceil_to_double(solution.objective_value());
    std::cout << res << std::endl;

}


int main(){
    ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0); //!!!!!!!!!!!!!!!!!!!!
    int t; cin >>t;
    for(int i = 0; i< t; i++){
        runTest();
    }
}

