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

// 叉积：(b - a) x (c - a)
int cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y)
         - (b.y - a.y) * (c.x - a.x);
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
    // 求下凸壳
    for (auto pt : p) {
        while (lower.size() >= 2 &&
               cross(lower[lower.size() - 2], lower.back(), pt) <= 0) {
            lower.pop_back();
        }
        lower.push_back(pt);
    }

    // 求上凸壳
    for (int i = n - 1; i >= 0; i--) {
        Point pt = p[i];

        while (upper.size() >= 2 &&
               cross(upper[upper.size() - 2], upper.back(), pt) <= 0) {
            upper.pop_back();
        }
        upper.push_back(pt);
    }

    // lower 和 upper 首尾会重复，需要删掉
    lower.pop_back();
    upper.pop_back();

    // 拼起来
    lower.insert(lower.end(), upper.begin(), upper.end());

    return lower;
}
