#include <bits/stdc++.h>
using namespace std;
#define int long long
#define ld long double
#define pll pair<int, int>
#define pdd pair<double, double>
#define tlll tuple<int, int, int>
#define lowbit(x) x & (-x)
const int N = 2e5 + 10;
const int M = 1e3 + 10;
const int mx = 1e18;
const int LOGN = 20;
const int MOD = 987654321;
int LOG2[LOGN];
int n, m, q;

/////////////////////
//如果坐标是long long范围  可能会超long long
//使用__int128进行处理
////////////////////

struct Point {
    int x, y;

    bool operator < (const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator == (const Point& other) const {
        return x == other.x && y == other.y;
    }
};
Point operator- (const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y};
}
Point operator+ (const Point& a, const Point& b) {
    return {a.x + b.x, a.y + b.y};
}

int cross(Point& a, Point& b,Point c) {
    return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}

//判断点位于哪一个半平面
int half(const Point& p) {
    if (p.y>0 || (p.y==0 && p.x>=0)) return 0;
    return 1;
}

int d(Point p,Point q) {
    return sqrt((p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y));
}
int d2(const Point& p) {
    return p.x * p.x + p.y * p.y;
}

//同向的点的化简
Point normalRay(Point p) {
    int g = gcd(abs(p.x),abs(p.y));
    p.x /= g;
    p.y /= g;
    return p;
}

//判断点P是否在线段AB上
bool onSegment(Point A,Point B,Point P) {
    if(cross(A,B,P) != 0) return false;

    return min(A.x,B.x) <= P.x && P.x <= max(A.x,B.x) && 
           min(A.y,B.y) <= P.y && P.y <= max(A.y,B.y);
}

//判断两条线段是否相交
bool segmentIntersect(Point A,Point B,Point C,Point D) {
    int c1 = cross(A,B,C);
    int c2 = cross(A,B,D);
    int c3 = cross(C,D,A);
    int c4 = cross(C,D,B);

    //此时是严格相交
    if(c1*c2<0 && c3*c4<0) return true;

    {
        // 以下处理端点接触、共线重叠
        if (c1 == 0 && onSegment(A, B, C)) return true;
        if (c2 == 0 && onSegment(A, B, D)) return true;
        if (c3 == 0 && onSegment(C, D, A)) return true;
        if (c4 == 0 && onSegment(C, D, B)) return true;
    }

    return false;
}

//点到直线的距离

//点到线段的距离

/**
 * 极角排序  叉乘实现  高精度
 * @param 点集
 * Andrew  n*logn
 * @return 从x轴逆时针排序
 * 注意处理一下零向量
 */
void angleSort(vector<Point>& points) {
    Point O{0, 0};
    sort(points.begin(), points.end(),
        [&](const Point& A, const Point& B) {
            Point a = A - O;
            Point b = B - O;

            int ha = half(a);
            int hb = half(b);

            if (ha != hb) return ha < hb;
            
            int c = cross(O, a, b);
            if (c != 0) return c > 0;
            // 同一方向时，距离近的在前
            return d2(a) < d2(b);
        }
    );
}


/***
 * 求凸包 
 * @param 点集
 * Andrew  n*logn
 * @return 返回逆时针顺序的凸包顶点
 */
vector<Point> convexHull(vector<Point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());

    int n = p.size();
    if (n <= 1) return p;
    vector<Point> lower, upper;
    for (auto pt : p) {
        while (lower.size() >= 2 &&
               cross(lower[lower.size() - 2], lower.back(), pt) <= 0) {
            lower.pop_back();
        }
        lower.push_back(pt);
    }

    for (int i = n - 1; i >= 0; i--) {
        Point pt = p[i];

        while (upper.size() >= 2 &&
               cross(upper[upper.size() - 2], upper.back(), pt) <= 0) {
            upper.pop_back();
        }
        upper.push_back(pt);
    }

    lower.pop_back();
    upper.pop_back();

    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}


//三角剖分
