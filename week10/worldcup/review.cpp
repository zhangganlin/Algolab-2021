#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 P;

using namespace std;
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


struct warehouse{
    P pos;
    int supply;
    int alcohol;
};
struct stadium{
    P pos;
    int demand;
    int uplimit;
};


void runTest(){
    int n,m,c;
    cin >> n >> m >> c;
    vector<warehouse> warehouses(n);
    vector<stadium> stadiums(m);
    vector<P> points;
    for(int i = 0; i < n; i++){
        P pos; int s,a;
        cin >> pos >> s >> a;
        warehouses[i] = {pos,s,a};
        points.push_back(pos);
    }
    for(int i = 0; i < m; i++){
        P pos; int d,u;
        cin >> pos >> d >> u;
        stadiums[i] = {pos,d,u};
        points.push_back(pos);
    }
    vector<vector<int>> revenues(n,vector<int>(m));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> revenues[i][j];
        }
    }

    Triangulation t;
    t.insert(points.begin(),points.end());
    vector<vector<bool>> w_inline(n);
    vector<vector<bool>> s_inline(m);

    int num_tot_lines = 0;
    for(int i = 0; i < c; i++){
        P pos; long r;
        cin >> pos >> r;
        if(r*r > CGAL::squared_distance(pos,t.nearest_vertex(pos)->point())){
            num_tot_lines ++;
            for(int j = 0; j < n; j++){
                bool in_line = CGAL::squared_distance(warehouses[j].pos,pos) < r*r;
                w_inline[j].push_back(in_line);
            }
            for(int j = 0; j < m; j++){
                bool in_line = CGAL::squared_distance(stadiums[j].pos,pos) < r*r;
                s_inline[j].push_back(in_line);
            }
        }
    }

    vector<vector<int>> num_crossing_line(n,vector<int>(m,0));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            for(int k = 0; k < num_tot_lines; k++){
                if(w_inline[i][k]!=s_inline[j][k]){
                    num_crossing_line[i][j]++;
                }
            }
        }
    }

    auto variable = [m](int i, int j){return i*m+j;};

    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    int counter = 0;
    for(int i = 0; i< n; i++){
        for(int j = 0; j < m; j++){
            lp.set_a(variable(i,j), counter,  1);
        }
        lp.set_b(counter,warehouses[i].supply);
        counter ++;
    }
    for(int j = 0; j < m; j++){
        for(int i = 0; i < n; i++){
            lp.set_a(variable(i,j),counter, 1);
            lp.set_a(variable(i,j),counter+1, -1);
            lp.set_a(variable(i,j),counter+2, warehouses[i].alcohol);
        }
        lp.set_b(counter,   stadiums[j].demand);
        lp.set_b(counter+1,-stadiums[j].demand);
        lp.set_b(counter+2,stadiums[j].uplimit*100);
        counter+=3;
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            lp.set_c(variable(i,j),num_crossing_line[i][j]-100*revenues[i][j]);
        }
    }

    Solution s = CGAL::solve_linear_program(lp,ET());
    if (s.is_infeasible()) {
        cout << "RIOT!" << endl;
    } else {
        double res = -CGAL::to_double(s.objective_value()) / 100.0;
        cout << (int)floor(res) << endl;
    }

}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}