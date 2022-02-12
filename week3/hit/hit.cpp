#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <random>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Ray_2 R;
typedef K::Point_2 P;
typedef K::Segment_2 S;

using namespace std;

void testcase(int num_segments) {
    // Read source and point through which ray passes
    long x, y, a, b;
    std::cin >> x >> y >> a >> b;

    // Define the ray
    R ppRay(P(x, y), P(a, b));

    // Read segments, test for intersection
    bool hit = false;
    for (int i = 0; i < num_segments; ++i) {
        std::cin >> x >> y >> a >> b;

        // Only test if not hit
        if (!hit) {
            S obstacle(P(x, y), P(a, b));

            if (CGAL::do_intersect(ppRay, obstacle)) {
                hit = true;
            }
        }

    }
    
    // Print result
    if (hit) {
        std::cout << "yes" << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }
    return;

}

int main() {
    ios_base::sync_with_stdio(false);
    int n; std::cin >> n;
    while (n != 0) {
        testcase(n);
        std::cin >> n;
    }
    return 0;
}