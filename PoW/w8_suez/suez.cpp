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

double ceil_to_double(CGAL::Quotient<ET> const &x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a++;
    while (a - 1 >= x) a--;
    return a;
}

void runTest(){
    int n,m,h,w;
    cin >> n >> m >> h >> w;
    vector<pair<IT,IT>> new_posters(n);
    vector<pair<IT,IT>> old_posters(m);
    for(int i =0; i< n;i++){
        IT x,y; cin >> x>>y;
        new_posters[i] = make_pair(x,y);
    }
    for(int i =0; i< m; i++){
        IT x,y; cin >> x>>y;
        old_posters[i] = make_pair(x,y);
    }

    Program lp (CGAL::SMALLER, true, 1, false, 0);

    //objective function
    for(int ai = 0; ai < n; ai ++){
        lp.set_c(ai,-2*(h+w));
    }


    int constraint_counter = -1;

    //constraints with older posters
    for(int ai = 0; ai<n; ai++){
        IT xi,yi;
        xi = new_posters[ai].first;
        yi = new_posters[ai].second;
        IT min_temp_W = LONG_MAX;
        IT min_temp_H = LONG_MAX;
        for(int old_idx = 0; old_idx<m; old_idx++){
            IT x_old,y_old;
            
            x_old = old_posters[old_idx].first;
            y_old = old_posters[old_idx].second;

            if(abs(x_old-xi)*h>abs(y_old-yi)*w){
                min_temp_W = min(-w+2*abs(x_old-xi),min_temp_W);
            }else{
                min_temp_H = min(-h+2*abs(y_old-yi),min_temp_H);
            }
            
        }
        constraint_counter ++;
        lp.set_a(ai,constraint_counter,w);
        lp.set_b(constraint_counter,min_temp_W);

        constraint_counter ++;
        lp.set_a(ai,constraint_counter,h);
        lp.set_b(constraint_counter,min_temp_H);


    }

    //constraints between new posters
    for(int ai = 0; ai<n-1;ai++){
        IT xi,yi;
        xi = new_posters[ai].first;
        yi = new_posters[ai].second;
        for(int aj = ai+1; aj<n; aj++){
            IT xj,yj;
            xj = new_posters[aj].first;
            yj = new_posters[aj].second;

            if(abs(xj-xi)*h>abs(yj-yi)*w){
                constraint_counter ++;
                lp.set_a(ai,constraint_counter,w);
                lp.set_a(aj,constraint_counter,w);
                lp.set_b(constraint_counter,2*abs(xj-xi));
            }else{
                constraint_counter ++;
                lp.set_a(ai,constraint_counter,h);
                lp.set_a(aj,constraint_counter,h);
                lp.set_b(constraint_counter,2*abs(yj-yi));
            }
        }
    }

    Solution solution = CGAL::solve_linear_program(lp, ET());
    double res = ceil_to_double(-solution.objective_value());
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
