#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>                   Delaunay;
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void runTest(){
    int n,m,c; 
    cin >> n >> m >> c;

    vector<K::Point_2> points;

    vector<vector<int>> warehouses(n,vector<int>(4));
    for(int i = 0; i < n ;i++){
        cin >> warehouses[i][0] >> warehouses[i][1]
            >> warehouses[i][2] >> warehouses[i][3];
        points.push_back(K::Point_2(warehouses[i][0],warehouses[i][1]));
    }
    vector<vector<int>> stadiums(m,vector<int>(4));
    for(int i = 0; i < m ;i++){
        cin >> stadiums[i][0] >> stadiums[i][1]
            >> stadiums[i][2] >> stadiums[i][3];
        points.push_back(K::Point_2(stadiums[i][0],stadiums[i][1]));
    }

    vector<vector<int> > revenues(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> revenues[i][j];
        }
    }

    Delaunay t;
    t.insert(points.begin(), points.end());

    int num_tot_lines = 0;
    vector<vector<bool> > w_in_line(n);
    vector<vector<bool> > s_in_line(m);

    for (int i = 0; i < c; i++) {
        int x, y;
        K::FT r;
        cin >> x >> y >> r;
        K::FT sq_r = r * r;
        K::Point_2 center(x, y);
        K::Point_2 nn = t.nearest_vertex(center)->point();
        if (CGAL::squared_distance(nn, center) < sq_r) {
            num_tot_lines++;
            for (int j = 0; j < n; j++) {
                bool in_line = (CGAL::squared_distance(center, K::Point_2(warehouses[j][0],warehouses[j][1])) < sq_r);
                w_in_line[j].push_back(in_line);
            }
            for (int j = 0; j < m; j++) {
                bool in_line = (CGAL::squared_distance(center, K::Point_2(stadiums[j][0],stadiums[j][1])) < sq_r);
                s_in_line[j].push_back(in_line);
            }
        }
    }

    vector<vector<int> > num_crossing_lines(n, vector<int>(m, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < num_tot_lines; k++) {
                if (w_in_line[i][k] != s_in_line[j][k]) {
                    num_crossing_lines[i][j]++;
                }
            }
        }
    }

    Program lp (CGAL::SMALLER, true, 0, false, 0);

    int counter = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            lp.set_a(i * m + j, counter, 1);  
        }
        lp.set_b(counter, warehouses[i][2]);
        counter++;
    }
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            lp.set_a(i * m + j, counter, 1);
            lp.set_a(i * m + j, counter + 1, -1);
            lp.set_a(i * m + j, counter + 2, warehouses[i][3]);
        }
        lp.set_b(counter, stadiums[j][2]);
        lp.set_b(counter + 1, -stadiums[j][2]);
        lp.set_b(counter + 2, stadiums[j][3] * 100);
        counter += 3;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            lp.set_c(i * m + j, num_crossing_lines[i][j] - 100 * revenues[i][j]);
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        cout << "RIOT!" << endl;
    } else {
        double res = -(s.objective_value_numerator() / s.objective_value_denominator()).to_double() / 100.0;
        cout << (int)floor(res) << endl;
    }

}


int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i =0; i< t;i++){
        runTest();
    }
}