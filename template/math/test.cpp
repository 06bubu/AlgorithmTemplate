#include <bits/stdc++.h>
using namespace std;
const int MOD = 2008;

//素数的一些理论
/***
 * 素数间隔平均间隔其实就30左右 基本上都是左
 * 10的13次方以内最多最大的也就777
 * 
 * 哥德巴赫猜想：
 * 1.任何一个大于2的偶数都可写成两个质数之和
 * 2.任何一个大于5的奇数都能被表示成三个奇质数的和
 * ---> 给定一个sum判断能不能转化到n个素数
 */
//关于斐波那契数列的性质



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

int inv[10000];
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
 * 费马小定理--a的逆元   (a,p)=1
 */
int inv_1(int a, int p) {
    return qpow(a, p - 2);
}

/***
 * 线性递推求解1-n逆元
 */
void build(int LIMIT, int p)
{
    inv[1] = 1;
    for (int i = 2; i <= LIMIT; i++)
        inv[i] = (p - inv[p % i] * (p / i) % p);
}

int gcd(int a, int b)
{
    return b == 0 ? a : gcd(b, a % b);
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

/**
 * 欧拉函数
 */
int euler_phi(int n)
{ 
    int ans = n;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
        {
            ans = ans / i * (i - 1);
            while (n % i == 0)
                n /= i;
        }
    if (n > 1)
        ans = ans / n * (n - 1);
    return ans;
}

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



