#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;
typedef K::Line_2 L;

using namespace std;

struct Biker{
    long y0;
    long x1;
    long y1;
    K::Direction_2 getAbsoluteDirection(){
        long y2=y1;
        if (y1-y0<0){
            y2 = y0 + y0-y1;
        }
        R ray(P(0,y0),P(x1,y2));
        K::Direction_2 d = ray.direction();
        return d;
    }
};

void runTest(){
    int n; cin >> n;
    vector<Biker> bikers(n);
    for(int i = 0; i< n; i++){
        long y0,x1,y1;
        cin >> y0 >> x1 >> y1;
        bikers[i]={y0,x1,y1};
    }
    sort(bikers.begin(),bikers.end(),[](Biker a, Biker b){return a.y0 < b.y0;});

    vector<Biker> runnings;
    for(int i = 0; i< n; i++){
        Biker curr = bikers[i];
        if(runnings.empty()){
            runnings.push_back(curr);
            continue;
        }
        while(!runnings.empty()){
            Biker last = runnings.back();
            runnings.pop_back();
            R ray_curr(P(0,curr.y0),P(curr.x1,curr.y1));
            R ray_last(P(0,last.y0),P(last.x1,last.y1));
            if(!CGAL::do_intersect(ray_curr,ray_last)){
                runnings.push_back(last);
                runnings.push_back(curr);
                break;
            }else{
                if(curr.getAbsoluteDirection()>last.getAbsoluteDirection() ||
                (curr.getAbsoluteDirection()==last.getAbsoluteDirection())){
                    runnings.push_back(last);
                    break;
                }
            }

        }
    }

    
}

int main(){
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--){
        runTest();
    }
}

int main_ref()
{
  P p[] = { P(0,0), P(2,0), P(1,0), P(3,0), P(.5,1), P(.5,-1) };
  S s[] = { S(p[0],p[1]), S(p[2],p[3]), S(p[4],p[5]) };
  for (int i = 0; i < 3; ++i)
    for (int j = i+1; j < 3; ++j)
        if (CGAL::do_intersect(s[i],s[j])) {
	        auto o = CGAL::intersection(s[i],s[j]);
            if (const P* op = boost::get<P>(&*o))
	            std::cout << "point: " << *op << "\n";
            else if (const S* os = boost::get<S>(&*o))
	            std::cout << "segment: " << os->source() << " " 
                    << os->target() << "\n";
            else // how could this be? -> error
                throw std::runtime_error("strange segment intersection");
      } else
	    std::cout << "no intersection\n";
}