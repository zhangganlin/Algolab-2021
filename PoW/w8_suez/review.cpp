///1
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(CGAL::Quotient<ET> const &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a++;
    while (a - 1 >= x) a--;
    return a;
}

void runTest(){
    int new_num, old_num,h,w;
    cin >> new_num >> old_num >> h >> w;
    vector<pair<IT,IT>> new_posters(new_num);
    vector<pair<IT,IT>> old_posters(old_num);
    for(int i = 0; i< new_num ; i++){
        IT x, y;
        cin >> x >> y;
        new_posters[i]={x,y};
    }
    for(int i = 0; i< old_num ; i++){
        IT x, y;
        cin >> x >> y;
        old_posters[i]={x,y};
    }

    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    int equ_num = 0;

    for(int ai = 0; ai < new_num; ai++ ){
        IT min_h = INT32_MAX;
        IT min_w = INT32_MAX;
        IT xi = new_posters[ai].first;
        IT yi = new_posters[ai].second;
        for(int i  = 0; i <  old_num; i++){
            IT x = old_posters[i].first;
            IT y = old_posters[i].second;
            if(abs(xi-x)*h < abs(yi-y)*w){
                min_h = min(min_h, 2*abs(yi-y)-h);
            }else{
                min_w = min(min_w, 2*abs(xi-x)-w);
            }
        }
        lp.set_a(ai, equ_num, h); lp.set_b(equ_num, min_h);
        equ_num++;
        lp.set_a(ai, equ_num, w); lp.set_b(equ_num, min_w);
        equ_num++;
    }

    for(int ai = 0; ai < new_num-1; ai++){
        IT xi = new_posters[ai].first;
        IT yi = new_posters[ai].second;
        for(int aj = ai+1; aj < new_num; aj++){
            IT xj = new_posters[aj].first;
            IT yj = new_posters[aj].second;
            if(abs(xj-xi)*h>abs(yj-yi)*w){
                lp.set_a(ai, equ_num, w);lp.set_a(aj, equ_num, w); lp.set_b(equ_num, 2*abs(xi-xj));
                equ_num++;
            }else{
                lp.set_a(ai, equ_num, h);lp.set_a(aj, equ_num, h); lp.set_b(equ_num, 2*abs(yi-yj));
                equ_num++;
            }
        }
    }

    for(int ai = 0; ai< new_num; ai++){
        lp.set_l(ai, true, 1); 
    }

    // objective function
    for(int ai = 0; ai < new_num; ai++){
        lp.set_c(ai, -2*(h+w));
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    double res = ceil_to_double(-s.objective_value());
    cout <<long(res)<<endl;

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--){
        runTest();
    }
    
}   