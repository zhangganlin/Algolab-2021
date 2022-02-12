#include<iostream>
#include<CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/number_utils.h>
#include <cmath>
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt   K;
typedef  K::Point_2                                          P;
typedef  CGAL::Min_circle_2_traits_2<K>   Traits;
typedef  CGAL::Min_circle_2<Traits>           Min_circle;

using namespace std;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}


void runTest(int n){
    P points[n];
    for (int i = 0; i < n; i++){
        long a,b;
        cin >> a >> b;
        points[i]=P(a,b);
    }
    Min_circle mc(points,points+n,true);
    cout << ceil_to_double(CGAL::sqrt(mc.circle().squared_radius()))<<endl;
}


int main()
{   
    ios_base::sync_with_stdio(false);
    cout<<setprecision(0) << fixed;
    int n; cin >> n;
    while(n!=0){
        runTest(n);
        cin >> n;
    }
}