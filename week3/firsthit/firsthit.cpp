#include<iostream>
#include<CGAL/Exact_predicates_exact_constructions_kernel.h>
#include<random>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

using namespace std;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void runTest(int n){
    long x,y,a,b;
    cin >> x>>y>>a>>b;
    P begin(x,y);
    R ray(begin,P(a,b));

    bool flag = false;
    vector<S> segs(n);
    for (int i = n-1; i >=0; i--) {
      long p1, p2, p3, p4;
      cin >> p1 >> p2 >> p3 >> p4;
      segs[i] = S(P(p1, p2), P(p3, p4));
      // segs.push_back(S(P(p1, p2), P(p3, p4)));
    }
    

    
    S curr_seg;


    for (int i =0; i<n; i++){
        S seg = segs[i];

        boost::optional<boost::variant<P,S>> o;
        if (flag ==false){
            bool intersect = CGAL::do_intersect(ray,seg);
            if (intersect){
                flag = true;
                o = CGAL::intersection(ray,seg);
            }else{continue;}
        }
        else if (flag==true){
            bool intersect = CGAL::do_intersect(seg,curr_seg);
            if (intersect){
                o = CGAL::intersection(seg,curr_seg);
            }else{continue;}
        }

        if(const P* op = boost::get<P>(&*o)){
            curr_seg = S(begin,*op);
        }else if(const S* os = boost::get<S>(&*o)){
            if (CGAL::collinear_are_ordered_along_line(begin,os->source(),os->target())){
                curr_seg = S(begin,os->source());
            }else{
                curr_seg = S(begin,os->target());
            }
        }

    }

    if (flag){
        cout << floor_to_double( curr_seg.target().x()) << " " << floor_to_double(curr_seg.target().y())<<endl;
    }else{
        cout << "no" <<endl;
    }

}


int main(){
    ios_base::sync_with_stdio(false);
    cout<<setprecision(0) << fixed;
    int n; cin>>n;
    while (n!=0){
        runTest(n);
        cin >> n;
    }
}