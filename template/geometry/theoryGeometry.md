对于一个非退化严格凸多边形的边来说：2*max(边长长度) < 总周长

对于四边形来说固定四条边的情况下所能达到的最大面积为：
设定s = (a+b+c+d)/2  然后面积最大为sqrt((s-a)*(s-b)*(s-c)*(s-d))
# 二维计算几何学习记录
## 极角排序
### 使用 `atan2` 排序
最直观的做法，是计算每个点的极角，然后按角度排序。
```cpp
sort(points.begin(), points.end(),
    [&](const Point& a, const Point& b) {
        double angleA = atan2(a.y - O.y, a.x - O.x);
        double angleB = atan2(b.y - O.y, b.x - O.x);

        if (angleA != angleB) {
            return angleA < angleB;
        }

        // 极角相同时，距离近的排在前面
        double distA = (a.x - O.x) * (a.x - O.x)
                    + (a.y - O.y) * (a.y - O.y);
        double distB = (b.x - O.x) * (b.x - O.x)
                    + (b.y - O.y) * (b.y - O.y);

        return distA < distB;
    }
);
```

### `atan2` 排序的起点
需要注意，`atan2` 返回的是：$ [-\pi,\pi] $   
不是默认从 $x$ 轴正方向开始的 $[0,2\pi)$ 顺序。   
写的时候可以把角度转换一下：
```cpp
double getAngle(const Point& p, const Point& O) {
    double angle = atan2(p.y - O.y, p.x - O.x);
    if (angle < 0) {
        angle += 2 * acos(-1.0);
    }
    return angle;
}
```
这样排序就会从 \(x\) 轴正方向开始，逆时针旋转一周。

---

### 使用叉积进行极角排序

#### 叉积的几何意义

对于两个二维向量:
$ \vec a=(x_a,y_a),\qquad \vec b=(x_b,y_b) $   
叉积定义为:
$ \vec a\times\vec b=x_a y_b-y_a x_b $

其符号可以判断从 \(\vec a\) 转向 \(\vec b\) 的方向：

- $\vec a\times\vec b>0$：从 $\vec a$ 到 $ \vec b $ 是逆时针转动
- $\vec a\times\vec b<0$：从 $\vec a$ 到 $ \vec b $ 是顺时针转动
- $\vec a\times\vec b=0$：两个向量共线

因此，叉积可以比较两个向量的相对方向。

但仅使用：

```cpp
return cross(a, b) > 0;
```

不能完成整圆范围的极角排序。

---

## 3.2 为什么不能只看叉积

考虑三个向量：

\[
A=(1,0),\quad B=(0,1),\quad C=(-1,-1)
\]

可能出现：

- \(A\) 应排在 \(B\) 前
- \(B\) 应排在 \(C\) 前
- 但根据单纯叉积，\(C\) 又可能排在 \(A\) 前

这会导致比较关系形成环，不满足 `std::sort` 要求的严格弱序。

本质原因是：角度是一个圆周量，\(0^\circ\) 和 \(360^\circ\) 相邻，但普通排序需要把圆周切开成一条线。

因此，必须先选择一条起始射线，再把整个平面分成两部分。

---

# 4. 半平面划分加叉积

如果要求：

- 从 \(x\) 轴正方向开始
- 逆时针排序
- 角度范围为 \([0,2\pi)\)

可以先定义向量位于哪一个半平面：

```cpp
int half(const Point& p) {
    if (p.y > 0 || (p.y == 0 && p.x >= 0)) {
        return 0;
    }
    return 1;
}
```

划分结果：

- `half = 0`：上半平面以及 \(x\) 轴正方向，角度属于 \([0,\pi)\)
- `half = 1`：下半平面以及 \(x\) 轴负方向，角度属于 \([\pi,2\pi)\)

然后：

1. 半平面编号不同，编号小的在前
2. 位于同一半平面时，用叉积判断顺序
3. 同方向时，按照距离处理

完整代码如下。

```cpp
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    long long x, y;
};

Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y};
}

long long cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

long long norm2(const Point& p) {
    return p.x * p.x + p.y * p.y;
}

// 从 x 轴正方向开始，逆时针划分
int half(const Point& p) {
    return (p.y > 0 || (p.y == 0 && p.x >= 0)) ? 0 : 1;
}

int main() {
    Point O{0, 0};

    vector<Point> points = {
        {0, -1},
        {1, 1},
        {-1, 0},
        {1, 0},
        {0, 1}
    };

    sort(points.begin(), points.end(),
         [&](const Point& A, const Point& B) {
             Point a = A - O;
             Point b = B - O;

             int ha = half(a);
             int hb = half(b);

             if (ha != hb) {
                 return ha < hb;
             }

             long long c = cross(a, b);
             if (c != 0) {
                 return c > 0;
             }

             // 同一方向时，距离近的在前
             return norm2(a) < norm2(b);
         });

    for (const Point& p : points) {
        cout << p.x << ' ' << p.y << '\n';
    }
}
```

结果为：

```text
1 0
1 1
0 1
-1 0
0 -1
```

对应角度：

\[
0^\circ,\ 45^\circ,\ 90^\circ,\ 180^\circ,\ 270^\circ
\]

---

# 5. 比较器的具体逻辑

核心部分是：

```cpp
int ha = half(a);
int hb = half(b);

if (ha != hb) {
    return ha < hb;
}
```

它先解决跨越 \(0^\circ/360^\circ\) 时的顺序问题。

接下来：

```cpp
long long c = cross(a, b);

if (c != 0) {
    return c > 0;
}
```

由于已经保证两个向量在同一个半平面中：

- `cross(a, b) > 0` 表示 \(a\) 的极角小于 \(b\)
- 因而 \(a\) 排在 \(b\) 前面

最后：

```cpp
return norm2(a) < norm2(b);
```

处理两个向量方向相同的情况。

例如：

\[
a=(1,1),\qquad b=(2,2)
\]

二者极角相同，叉积为零。这时可以按距离排序：

\[
|a|^2=2,\qquad |b|^2=8
\]

所以 \(a\) 在前。

这里比较平方距离，不需要开平方：

\[
|\vec a|^2=x_a^2+y_a^2
\]

---

# 6. 共线向量不一定同方向

叉积为零，只能说明向量共线，不一定说明同方向。

例如：

\[
a=(1,0),\qquad b=(-1,0)
\]

有：

\[
a\times b=0
\]

但二者方向相反，极角分别是 \(0^\circ\) 和 \(180^\circ\)。

半平面划分会先将它们分到不同组：

```cpp
half({1, 0})  == 0
half({-1, 0}) == 1
```

所以不会错误地按相同极角处理。

如果两个非零向量叉积为零，并且位于同一个上述半平面中，它们才是同方向。

---

# 7. 顺时针极角排序

如果要求从 \(x\) 轴正方向开始顺时针排序，可以修改半平面定义和叉积方向。

一种实现是：

```cpp
int halfClockwise(const Point& p) {
    return (p.y < 0 || (p.y == 0 && p.x >= 0)) ? 0 : 1;
}
```

比较器：

```cpp
sort(points.begin(), points.end(),
     [&](const Point& A, const Point& B) {
         Point a = A - O;
         Point b = B - O;

         int ha = halfClockwise(a);
         int hb = halfClockwise(b);

         if (ha != hb) {
             return ha < hb;
         }

         long long c = cross(a, b);
         if (c != 0) {
             return c < 0;
         }

         return norm2(a) < norm2(b);
     });
```

其顺序类似：

\[
0^\circ,\ 315^\circ,\ 270^\circ,\ 180^\circ,\ 90^\circ
\]

其中角度仍按通常的逆时针角度表示。

---

# 8. 从任意方向开始排序

有时不是从 \(x\) 轴正方向开始，而是从给定向量：

\[
\vec s=(s_x,s_y)
\]

开始逆时针排序。

例如希望从正 \(y\) 轴方向 \((0,1)\) 开始：

\[
90^\circ,\ 180^\circ,\ 270^\circ,\ 0^\circ
\]

可以建立以 \(\vec s\) 为基准的局部坐标。对于待比较向量 \(\vec v\)，计算：

\[
X=\vec s\cdot\vec v
\]

\[
Y=\vec s\times\vec v
\]

其中：

\[
\vec s\cdot\vec v=s_xv_x+s_yv_y
\]

这里：

- \(X\) 是 \(\vec v\) 在起始方向上的投影
- \(Y\) 表示 \(\vec v\) 相对起始方向的逆时针偏转

于是可以把：

\[
(X,Y)
\]

当成变换后的新坐标，再按照从正 \(x\) 轴开始的方法排序。

```cpp
struct PolarCoord {
    long long x, y;
};

long long dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

PolarCoord relativeCoord(const Point& start, const Point& v) {
    return {
        dot(start, v),
        cross(start, v)
    };
}
```

更直接的方法是定义当前向量是否位于起始射线逆时针方向的前半圈：

```cpp
int relativeHalf(const Point& start, const Point& v) {
    long long c = cross(start, v);
    long long d = dot(start, v);

    // 相对角度为 [0, pi) 的部分
    return (c > 0 || (c == 0 && d >= 0)) ? 0 : 1;
}
```

完整比较：

```cpp
Point start{0, 1};  // 从正 y 轴开始

sort(points.begin(), points.end(),
     [&](const Point& A, const Point& B) {
         Point a = A - O;
         Point b = B - O;

         int ha = relativeHalf(start, a);
         int hb = relativeHalf(start, b);

         if (ha != hb) {
             return ha < hb;
         }

         long long c = cross(a, b);
         if (c != 0) {
             return c > 0;
         }

         return norm2(a) < norm2(b);
     });
```

---

# 9. 以某个点为中心排序

极角排序前一定要把点转换成相对于中心点的向量。

错误写法：

```cpp
cross(A, B)
```

当中心不是原点时，这比较的是 \(\overrightarrow{OA}\) 和 \(\overrightarrow{OB}\)，其中 \(O\) 是坐标原点，而不是题目给定的中心。

正确写法：

```cpp
Point a = A - center;
Point b = B - center;

cross(a, b);
```

等价地，可以定义三点叉积：

```cpp
long long cross(const Point& O,
                const Point& A,
                const Point& B) {
    return (A.x - O.x) * (B.y - O.y)
         - (A.y - O.y) * (B.x - O.x);
}
```

其含义是：

\[
\overrightarrow{OA}\times\overrightarrow{OB}
\]

---

# 10. 中心点本身怎么处理

如果某个待排序点正好等于中心点：

\[
P=O
\]

那么：

\[
\overrightarrow{OP}=(0,0)
\]

零向量没有极角。

因此必须明确处理策略。

## 方案一：提前删除

很多几何算法中，中心点不应该进入极角排序：

```cpp
points.erase(
    remove_if(points.begin(), points.end(),
              [&](const Point& p) {
                  return p.x == O.x && p.y == O.y;
              }),
    points.end()
);
```

## 方案二：固定排在最前面

```cpp
bool zeroA = (a.x == 0 && a.y == 0);
bool zeroB = (b.x == 0 && b.y == 0);

if (zeroA != zeroB) {
    return zeroA;
}
```

## 方案三：固定排在最后面

```cpp
if (zeroA != zeroB) {
    return !zeroA;
}
```

不能把零向量直接交给普通极角比较器，否则它与任意向量的叉积都是零，距离比较虽然可能给出结果，但“极角”含义并不成立。

---

# 11. 整数溢出问题

如果坐标使用 `long long`，叉积：

\[
x_1y_2-y_1x_2
\]

可能溢出。

例如坐标量级是 \(10^{12}\)，乘积量级为：

\[
10^{24}
\]

已经超过 64 位整数范围。

可以使用 `__int128`：

```cpp
using i128 = __int128_t;

i128 cross(const Point& a, const Point& b) {
    return static_cast<i128>(a.x) * b.y
         - static_cast<i128>(a.y) * b.x;
}

i128 norm2(const Point& p) {
    return static_cast<i128>(p.x) * p.x
         + static_cast<i128>(p.y) * p.y;
}
```

比较器中：

```cpp
i128 c = cross(a, b);

if (c != 0) {
    return c > 0;
}
```

如果坐标范围是 \(10^9\)，乘积在 \(10^{18}\) 量级，虽然单个乘积通常接近 `long long` 范围，但两个乘积相减仍可能溢出，因此使用 `__int128` 更稳妥。

---

# 12. 浮点坐标的处理

若坐标是 `double`，不能简单使用：

```cpp
cross(a, b) == 0
```

一般要使用误差阈值：

```cpp
constexpr double EPS = 1e-10;

double c = cross(a, b);

if (fabs(c) > EPS) {
    return c > 0;
}
```

但是在 `std::sort` 比较器中随意使用 EPS 可能破坏严格弱序。例如：

- \(a\) 和 \(b\) 被认为相等
- \(b\) 和 \(c\) 被认为相等
- \(a\) 和 \(c\) 却被认为不相等

因此可能产生不传递关系。

对于浮点极角排序，更稳妥的策略包括：

- 数据本质是整数时，始终使用整数坐标和整数叉积
- 如果必须使用浮点数，预先计算 `atan2` 作为排序键
- 或对坐标按明确精度量化后再使用整数比较
- 保证最终有稳定且一致的次级排序键

例如预计算角度：

```cpp
struct Node {
    Point p;
    double angle;
    double dist2;
};

sort(nodes.begin(), nodes.end(),
     [](const Node& a, const Node& b) {
         if (a.angle != b.angle) {
             return a.angle < b.angle;
         }
         return a.dist2 < b.dist2;
     });
```

---

# 13. 同角度时距离顺序怎么选

这取决于具体算法，不是固定规则。

## 距离近的在前

常规极角排序经常这样写：

```cpp
return norm2(a) < norm2(b);
```

适合：

- 输出所有点的极角顺序
- 射线上从近到远处理
- Graham 扫描的部分实现

## 距离远的在前

```cpp
return norm2(a) > norm2(b);
```

适合某些需要优先保留最远点的场景。

## 同一射线只保留最远点

例如凸包预处理：

1. 先按角度、距离升序排列
2. 对相同方向的一组点，只留下最后一个

判断同方向：

```cpp
cross(a, b) == 0 && dot(a, b) > 0
```

如果已经通过半平面确保在同一半区，通常判断叉积为零就足够。

---

# 14. 极角排序与 Graham 凸包

Graham 扫描通常会：

1. 选择一个基准点
2. 其他点相对基准点做极角排序
3. 使用栈维护凸包

基准点一般取：

- \(y\) 最小
- 如果 \(y\) 相同，则 \(x\) 最小

由于基准点是最下方的点，其他点相对它通常都处于上半平面，因此可以直接使用叉积排序：

```cpp
sort(points.begin() + 1, points.end(),
     [&](const Point& a, const Point& b) {
         long long c = cross(points[0], a, b);

         if (c != 0) {
             return c > 0;
         }

         return dist2(points[0], a) < dist2(points[0], b);
     });
```

这里不一定需要完整的半平面划分，是因为基准点的选择已经限制了其他向量的角度范围，一般不会跨越完整圆周。

然后扫描：

```cpp
vector<Point> hull;

for (const Point& p : points) {
    while (hull.size() >= 2) {
        Point a = hull[hull.size() - 2];
        Point b = hull[hull.size() - 1];

        if (cross(a, b, p) > 0) {
            break;
        }

        hull.pop_back();
    }

    hull.push_back(p);
}
```

其中：

- `cross(a, b, p) > 0`：左转
- `cross(a, b, p) < 0`：右转
- `cross(a, b, p) == 0`：共线

是否弹出共线点，决定凸包是否保留边界上的所有点。

---

# 15. 一个通用模板

下面是整数点、任意中心、从正 \(x\) 轴开始、逆时针排序的稳妥模板：

```cpp
#include <algorithm>
#include <cstdint>
#include <vector>
using namespace std;

using i64 = long long;
using i128 = __int128_t;

struct Point {
    i64 x, y;
};

Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y};
}

i128 cross(const Point& a, const Point& b) {
    return static_cast<i128>(a.x) * b.y
         - static_cast<i128>(a.y) * b.x;
}

i128 norm2(const Point& p) {
    return static_cast<i128>(p.x) * p.x
         + static_cast<i128>(p.y) * p.y;
}

bool isZero(const Point& p) {
    return p.x == 0 && p.y == 0;
}

// [0, pi) 为 0，[pi, 2pi) 为 1
int half(const Point& p) {
    return (p.y > 0 || (p.y == 0 && p.x >= 0)) ? 0 : 1;
}

void polarSort(vector<Point>& points, const Point& center) {
    sort(points.begin(), points.end(),
         [&](const Point& A, const Point& B) {
             Point a = A - center;
             Point b = B - center;

             bool za = isZero(a);
             bool zb = isZero(b);

             // 中心点固定放在最前
             if (za != zb) {
                 return za;
             }

             // 两个都是中心点，视为等价
             if (za && zb) {
                 return false;
             }

             int ha = half(a);
             int hb = half(b);

             if (ha != hb) {
                 return ha < hb;
             }

             i128 c = cross(a, b);
             if (c != 0) {
                 return c > 0;
             }

             i128 da = norm2(a);
             i128 db = norm2(b);

             if (da != db) {
                 return da < db;
             }

             // 完全重合的点视为等价
             return false;
         });
}
```

复杂度为：

\[
O(n\log n)
\]

每次比较只涉及整数加减乘法，通常比反复调用 `atan2` 更快，也不会产生浮点角度误差。

---

# 16. 常见错误总结

### 错误一：只写叉积

```cpp
sort(v.begin(), v.end(), [](Point a, Point b) {
    return cross(a, b) > 0;
});
```

对于整圆排序，这可能不满足传递性，属于错误比较器。

### 错误二：忘记减中心点

```cpp
cross(A, B)
```

应改为：

```cpp
cross(A - O, B - O)
```

### 错误三：叉积为零时没有次级排序

这不一定导致排序失败，但会让同方向点的顺序不确定，可能影响后续算法。

### 错误四：把反方向误认为同角度

叉积为零只表示共线。完整极角排序必须先进行半平面划分。

### 错误五：直接放入零向量

中心点没有极角，应提前删除或单独规定位置。

### 错误六：整数乘法溢出

即使函数返回 `long long` 或 `__int128`，如果先用 `long long` 相乘再转换，仍然会溢出。

错误：

```cpp
return (__int128)(a.x * b.y - a.y * b.x);
```

正确：

```cpp
return (__int128)a.x * b.y - (__int128)a.y * b.x;
```

必须在乘法发生前进行类型提升。

---

**核心记忆**

完整圆周上的极角排序可以概括为三步：

```text
先减中心点
再划分半平面
同一半平面内用叉积排序
```

即：

```cpp
if (half(a) != half(b))
    return half(a) < half(b);

if (cross(a, b) != 0)
    return cross(a, b) > 0;

return norm2(a) < norm2(b);
```

其中半平面划分解决“圆从哪里切开”的问题，叉积解决同一半圆内部的角度先后问题，距离则处理同方向向量。