#include <bits/stdc++.h>
using namespace std;
#define i128 __int128
#define int long long
const int MOD = 2008;

int qpow(int a, int b) {
    int ans = 1 % MOD;
    while (b)
    {
        if (b & 1)
            ans = ans * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return ans % MOD;
}

/**
 * 三分模板  
 */
void ternarySearch(int l, int r) {
    while (r - l > 5) {
        int mid1 = l + (r - l) / 3;
        int mid2 = r - (r - l) / 3;

        // if (f(mid1) <= f(mid2)) {
        //     // 最大值在右侧
        //     l = mid1 + 1;
        // } else {
        //     // 最大值在左侧
        //     r = mid2 - 1;
        // }
    }

    for (int x = l + 1; x <= r; ++x) {
        // if (f(x) > f(ans)) {
        //     ans = x;
        // }
    }
}

/**
 * 高斯消元
 * 矩阵 -----> 上三角矩阵
 */
int det(vector<vector<int>> a, int n) {
    int ans = 1;

    for (int i = 0; i < n; i++) {
        int pivot = -1;

        // 找第 i 列的主元
        for (int j = i; j < n; j++) {
            if (a[j][i] != 0) {
                pivot = j;
                break;
            }
        }

        // 没有主元，行列式为 0
        if (pivot == -1) {
            return 0;
        }

        // 如果主元不在当前行，就交换行
        // 交换两行会让行列式变号
        if (pivot != i) {
            swap(a[pivot], a[i]);
            ans = (MOD - ans) % MOD;
        }

        // 当前主元的逆元
        int inv = qpow(a[i][i], MOD - 2);

        // 用第 i 行消掉下面所有行的第 i 列
        for (int j = i + 1; j < n; j++) {
            if (a[j][i] == 0) {
                continue;
            }

            int factor = a[j][i] * inv % MOD;

            for (int k = i; k < n; k++) {
                a[j][k] = (a[j][k] - factor * a[i][k]) % MOD;

                if (a[j][k] < 0) {
                    a[j][k] += MOD;
                }
            }
        }

        // 乘上当前对角线元素
        ans = ans * a[i][i] % MOD;
    }
    return ans;
}

int exgcd(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int gcd = exgcd(b, a % b, x, y);
    int tmp = x;
    x = y;
    y = tmp - a / b * y;
    return gcd;
}

/***
 * CRT定理
 * 扩展CRT定理
 * 下标均为从[1,n]
 */
int normalize(i128 x, int mod) {
    x %= mod;
    if (x < 0) x += mod;
    return (int)x;
}
int CRT(int n, vector<int> a,vector<int> m) {
    int M = 1, ans = 0;
    for (int i = 1; i <= n; i++) M = M * m[i];
    for (int i = 1; i <= n; i++) {
        int cnt = M / m[i];
        int inv, y;
        exgcd(cnt, m[i], inv, y);//求解cn关于m[i]的逆元
        ans = (ans + a[i] * cnt * inv % M) % M;
    }
    return (ans % n + n) % n;
}
int exCRT(int n,vector<int> &a,vector<int> &m,int &lcm) {
    if (n == 0) {
        lcm = 1;
        return 0;
    }

    int ans = normalize(a[1], m[1]);
    int M = m[1];

    for (int i = 2; i <= n; i++) {
        int x, y;
        int g = exgcd(M, m[i], x, y);

        i128 diff128 = (i128)a[i] - ans;

        if (diff128 % g != 0) {
            lcm = -1;
            return -1;
        }

        int mod = m[i] / g;

        int k = normalize((diff128 / g) * x, mod);

        int newM = (int)((i128)M / g * m[i]);

        ans = normalize((i128)ans + (i128)M * k, newM);
        M = newM;
    }

    lcm = M;
    return ans;
}
 
/**
 * 异或线性基
 */
struct lineBasis {
    int d[61],p[61];
    int cnt;
    lineBasis() {
        memset(d,0,sizeof(d));
        memset(p,0,sizeof(p));
        cnt = 0;
    }
    bool insert(int x)
    {
        for (int i=60;i>=0;i--) {
            if (x & (1LL<<i)) {
                if (!d[i]) {
                    d[i] = x;
                    return true;
                } else { 
                    x ^= d[i];
                }
            }
        }
        return false;
    }
    int query_max()
    {
        int res = 0;
        for (int i = 60;i>=0;i--)
            if ((res^d[i]) > res)
                res ^= d[i];
        return res;
    }
    int query_min()
    {
        for (int i=0;i<=60;i++)
            if (d[i])
                return d[i];
        return 0;
    }
    void rebuild()
    {
        for (int i=60;i>=0;i--)
            for (int j=i-1;j>=0;j--)
                if (d[i]&(1LL<<j))
                    d[i]^=d[j];
        for (int i=0;i<=60;i++)
            if (d[i])
                p[cnt++]=d[i];
    }
    int kthquery(int k)
    {
        int ret=0;
        if (k>=(1LL<<cnt))
            return -1;
        for (int i=60;i>=0;i--)
            if (k&(1LL<<i))
                ret^=p[i];
        return ret;
    }
};


/**
 * 高精度
 * 加法  乘法  最值
 * O(len)
 */
struct BigInteger {
    static const int BASE = 10;   // 如果想更快，可以改成 1e8，但输出要处理前导零
    vector<int> d;                // 低位在前
 
    BigInteger() { d = {0}; }
 
    BigInteger(long long num) { *this = num; }
 
    BigInteger(const string &s) { *this = s; }
 
    BigInteger& operator = (long long num) {
        d.clear();
        if (num == 0) d.push_back(0);
        while (num) {
            d.push_back(num % BASE);
            num /= BASE;
        }
        return *this;
    }
 
    BigInteger& operator = (const string &s) {
        d.clear();
        for (int i = (int)s.size() - 1; i >= 0; --i) {
            d.push_back(s[i] - '0');
        }
        trim();
        return *this;
    }
 
    void trim() {
        while (d.size() > 1 && d.back() == 0) d.pop_back();
    }
 
    string toString() const {
        string s;
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            s += char(d[i] + '0');
        }
        return s;
    }
 
    // 比较大小
    bool operator < (const BigInteger &b) const {
        if (d.size() != b.d.size()) return d.size() < b.d.size();
        for (int i = (int)d.size() - 1; i >= 0; --i) {
            if (d[i] != b.d[i]) return d[i] < b.d[i];
        }
        return false;
    }
 
    bool operator > (const BigInteger &b) const {
        return b < *this;
    }
 
    bool operator == (const BigInteger &b) const {
        return d == b.d;
    }
 
    // 加法
    BigInteger operator + (const BigInteger &b) const {
        BigInteger c;
        c.d.clear();
        int carry = 0;
        int n = d.size(), m = b.d.size();
        int len = max(n, m);
        for (int i = 0; i < len; ++i) {
            int x = carry;
            if (i < n) x += d[i];
            if (i < m) x += b.d[i];
            c.d.push_back(x % BASE);
            carry = x / BASE;
        }
        if (carry) c.d.push_back(carry);
        return c;
    }
 
    // 乘法
    BigInteger operator * (const BigInteger &b) const {
        BigInteger c;
        int n = d.size(), m = b.d.size();
        c.d.assign(n + m, 0);
 
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                c.d[i + j] += d[i] * b.d[j];
            }
        }
 
        for (int i = 0; i < (int)c.d.size() - 1; ++i) {
            c.d[i + 1] += c.d[i] / BASE;
            c.d[i] %= BASE;
        }
 
        c.trim();
        return c;
    }
};
// 取最大值
BigInteger bigMax(const BigInteger &a, const BigInteger &b) {
    return (a < b ? b : a);
}

//判断浮点数是不是一个整数 fabsl(x1-roundl(x1)) < 1e-12L
//roundl 往最近的整数取整
//floorl    向下取整，返回不大于 x 的最大整数
//ceill(x)	向上取整，返回不小于 x 的最小整数
