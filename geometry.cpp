#include <bits/stdc++.h>
#ifdef ONLINE_JUDGE
#pragma GCC optimize("O2")
#endif
#define ll long long
using namespace std;
const double pi = acos(-1);
const double eps = 1e-6;
const double inf = LLONG_MAX>>1;
const double lim = 1e9 + 1;
class point{
    ll x, y;
public :
    point(){
        x = y = 0;
    }
    point (ll a,ll b){
        assert(abs(a)<inf && abs(b)<inf);
        x = a;
        y = b;
    }
    point (const point & a){
        x = a.x;
        y = a.y;
    }
    ll getx ()const{return x;}
    ll gety ()const{return y;}
    point operator+ (const point& a)const{
        return point(x+a.x,y+a.y);
    }
    point operator-() const{
        return point(-x,-y);
    }
    point operator-(const point &a) const{
        return (*this)+(-a);
    }

    ll dot(const point &a)const {
        return x*a.x + y*a.y;
    }
    double dist(const point &a)const{
        return hypot(x-a.x,y-a.y);
    }
    ll operator*(const point &a)  const{ 
        return x * a.y - y * a.x;
    }
    double norm() const{
        return hypot(x,y);
    }
    double angle(const point &a)const{ 
        // returns polar angle { OX, (this,a)}
        point u = a - (*this);
        double m = atan2(u.y,u.x);
        if(m < 0) m = fmod(m+2*pi,2*pi);
        return m; 
    }
    bool operator<(const point& b){
        if(y != b.y) return y<b.y;
        return x < b.x;
    }
    void disp(){
        cout<<x<<" "<<y;
    }

};
bool collinear(const point &a, const point &b, const point &c){
    // returns true if the three are collinear
    ll o = (b-a)*(c-a);
    return o == 0;
}
class segment{
    point first;
    point last;
public :
    segment(){
        first = last = point();
    }
    segment(ll a,ll b, ll c, ll d){
        if(a>c) swap(a,c),swap(b,d);
        if(a==c && b>d) swap(b,d); 
        first = point(a,b);
        last = point(c,d);
    }
    segment(point a,point b){
        first = a;
        last = b;
    }
    double slope() const {
        // y = slope * x + b;
        return (last.gety() - first.gety()) * (1.0/(last.getx() - first.getx()));
    }
    double len() const{
        return first.dist(last);
    }
    double dist(const point &a)const{
        // returns shortest distance between a and each point on the segment *this
        if(((a-first).dot(first-last))>-eps) return last.dist(a);
        if((last-first).dot(a-last)>-eps) return first.dist(a);
        if(this->len() < eps) first.dist(a);
        return abs((a-first)*(last-first)/(this->len()));
    }
    double dist(const segment &s) const{
        if((this->intersect(s))) return 0;
        return min(s.dist(first),s.dist(last));
    }
    bool intersect (const segment &a)const{
        if(fabs(a.slope() - this->slope())<eps){
            //parallel
            if(a.dist(last)>eps && a.dist(first)>eps)return false;
            return true;
        }
        point A = first, B = last, C = a.first, D = a.last;
        if((B-A)*(C-A)*((B-A)*(D-A))>0) return false;
        swap(A,C);swap(D,B);
        if((B-A)*(C-A)*((B-A)*(D-A))>0) return false;
        return true;
        

    }
};
class polygon{
protected : 
    vector<point> v;
public :
    polygon(){}
    polygon(vector<point> arr){
        v = arr;
    }
    double area(){
        double ans = 0;
        point prev = v.back();
        for(auto pt : v){
            ans+= (pt.getx() - prev.getx())*((pt.gety() + prev.gety()))*0.5;
            prev = pt;
        }
        return ans;
    }
    bool is_inside(const point &a) const{
        // Ray casting algorithm to check if a point is inside a polygon
        point b(a.getx() + lim,a.gety() + 1);
        segment A(a,b);
        int x = 0;
        point prev = v.back();
        for(int i = 1;i<v.size();i++){
            segment B(prev,v[i]);
            if(B.dist(a) < eps) return true;
            if(A.intersect(B)) x++;
        }
        return x&1;
    }
    void disp(){
        for(auto x:v){
            x.disp();cout<<"\n";
        }
    }
};
class convex_polygon : public polygon{
public :
    convex_polygon(vector<point> arr){
        // constructs convex hull 
        if(arr.size()<=2){ 
            v = arr; 
            return ;
        }
        sort(arr.begin(),arr.end());
        sort(arr.begin()+1,arr.end(), [=](point a, point b)->bool{
            return (arr[0].angle(a) != arr[0].angle(b) ? arr[0].angle(a)<arr[0].angle(b) : arr[0].dist(a)>arr[0].dist(b));
        });
        v.resize(arr.size());
        v[0] = arr[0];
        v[1] = arr[1];
        int n = 2;
        for(int i = 2;i<arr.size();n++,i++){
            while(n>1 && (v[n-1] - v[n-2])*(arr[i] - v[n-2])<=0)n--;
            v[n] = arr[i];
        }
        v.resize(n);
    }
};
int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int n;cin>>n;
    vector<point> v;
    for(int i=0;i<n;i++){
        int x,y;
        cin>>x>>y;
        v.push_back(point(x,y));
    }
    convex_polygon p(v);
    p.disp();
    cout<<p.is_inside(point(0,0))<<endl;
}
