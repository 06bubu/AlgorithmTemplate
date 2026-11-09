#include <bits/stdc++.h>
using namespace std;
#define int long long
#define ld long double

const double EPS = 1e-10;

struct Vec3 {
    double x, y, z;

    Vec3 operator+(const Vec3& v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    Vec3 operator-(const Vec3& v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    Vec3 operator*(double k) const {
        return {x * k, y * k, z * k};
    }

    Vec3 operator/(double k) const {
        return {x / k, y / k, z / k};
    }
};

//两个向量点积
double dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

//两个向量叉积--->平面法向量
Vec3 cross(const Vec3& a, const Vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

//向量的长度
double length(const Vec3& v) {
    return sqrt(dot(v, v));
}

// 返回两个单位向量之间的夹角，范围 [0, pi]
ld angle(const Vec3& a, const Vec3& b) {
    return atan2l(length(cross(a, b)), dot(a, b));
}

//单位向量
Vec3 normalize(const Vec3& v) {
    return v / length(v);
}

// 三个不共线点确定的平面的法向量
Vec3 planeNormal(const Vec3& A, const Vec3& B, const Vec3& C) {
    Vec3 n = cross(B - A, C - A);
    if (length(n) < EPS) {
        throw runtime_error("三个点共线，不能唯一确定平面");
    }
    return n;
}

// 点 P 到平面 ABC 的距离
double pointPlaneDistance(const Vec3& P,const Vec3& A,const Vec3& B,const Vec3& C ) {
    Vec3 n = planeNormal(A, B, C);
    return abs(dot(n, P - A)) / length(n);
}

// 点 P 在平面 ABC 上的投影
Vec3 projectToPlane(
    const Vec3& P,
    const Vec3& A,
    const Vec3& B,
    const Vec3& C
) {
    Vec3 n = planeNormal(A, B, C);
    double t = dot(n, P - A) / dot(n, n);
    return P - n * t;
}

// 两个平面的通常夹角，返回弧度，范围 [0, pi/2]
double planeAngle(Vec3 n1, Vec3 n2) {
    double c = dot(n1, n2) / (length(n1) * length(n2));
    c = clamp(abs(c), 0.0, 1.0);
    return acos(c);
}

// 求直线 OP 与平面 OAB 的线面角。
// 返回弧度，范围 [0, pi / 2]。
double linePlaneAngle(
    const Vec3& P,
    const Vec3& O,
    const Vec3& A,
    const Vec3& B
) {
    Vec3 direction = P - O;
    Vec3 normal = cross(A - O, B - O);

    double directionLength = length(direction);
    double normalLength = length(normal);

    if (directionLength < EPS) {
        throw runtime_error("P 与 O 重合，无法确定直线 OP");
    }

    if (normalLength < EPS) {
        throw runtime_error("O、A、B 共线，无法确定平面");
    }

    double sinAngle =
        abs(dot(direction, normal)) /
        (directionLength * normalLength);

    sinAngle = clamp(sinAngle, 0.0, 1.0);
    return asin(sinAngle);
}

// Rodrigues：向量 v 绕单位轴 axis 旋转 angle 弧度
Vec3 rotateVector(Vec3 v, Vec3 axis, double angle) {
    axis = normalize(axis);
    double c = cos(angle);
    double s = sin(angle);

    return v * c
         + cross(axis, v) * s
         + axis * (dot(axis, v) * (1.0 - c));
}

// 点 P 绕经过 O、方向为 axis 的直线旋转
Vec3 rotatePoint(Vec3 P, Vec3 O, Vec3 axis, double angle) {
    return O + rotateVector(P - O, axis, angle);
}