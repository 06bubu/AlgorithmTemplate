#include <bits/stdc++.h>
using namespace std;
#define int long long
const int MOD = 1e9 + 7;
const int N = 5e5 + 10;
const int MAXN = 1e6 + 5;

int fac[N], invfac[N];


int qpow(int a, int b) {
    int r=1;
    while (b) {
        if (b&1) r=r*a%MOD;
        a=a*a%MOD;
        b>>=1;
    }
    return r;
}

void init(int n) {
    fac[0] = 1;
    for (int i=1;i<=n;i++) fac[i] = fac[i-1]*i%MOD;
    invfac[n]=qpow(fac[n],MOD-2);
    for (int i=n-1;i>=0;i--) invfac[i]=invfac[i+1]*(i+1)%MOD;
}

int C(int a, int b) {
    if (b < 0 || b > a) return 0;
    return fac[a] * invfac[b] % MOD * invfac[a - b] % MOD;
}

/**
 * Erdős–Szekeres定理：
 * 任意长度为 (r-1)(s-1)+1 的序列，要么存在长度为 r 的非递减子序列，要么存在长度为 s 的严格递减子序列
 * 
 * Dilworth定理:
 * 将一个序列剖成若干个单调不升子序列的最小个数等于该序列最长上升子序列的个数
 */

/***
 * 错位排列
 * f[0] = 1;
 * f[1] = 0;
 * f[i] = (i-1)*(f[i-1]+f[-2])
 */
int derangement(int n) {
    if (n == 0) return 1;
    if (n == 1) return 0;

    int dp0 = 1; // !0
    int dp1 = 0; // !1

    for (int i = 2; i <= n; i++) {
        int cur = (i - 1) * ((dp1 + dp0) % MOD) % MOD;
        dp0 = dp1;
        dp1 = cur;
    }

    return dp1;
}

/**
 * 无编号球放到有编号盒子
 * 隔板法：
 * 盒子不允许为空 : C(n-1,k-1)
 * 盒子允许为空   : C(n+k-1,k-1) 
 */

/**
 * 第二类斯特林 : 
 * n个有标号的球放到k个无标号的盒子中,盒子不为空
 * DP；加入已有集合中or新开一个集合
 * dp[n][k] = dp[n-1][k-1] + k*dp[n-1][k]
 * init : dp[0][0] = 1
 */
struct ppp {
    int S[10][10];
    void initS2()
    {//预处理斯特林数
        int n = 1e6;
        S[0][0] = 1;
        int R = n - 5;
        for(int i = 1; i <= n; i ++ )
            for(int j = 1; j <= n; j ++)
                S[i][j] = (S[i - 1][j - 1] + S[i - 1][j] * j % MOD) % MOD;
    }
};

/***
 * 后续补充同一行/列的第二类斯特林数的计算
 */

/**
 * 第一类斯特林 : 
 * n个有标号的球放到k个无标号的盒子中,盒子可以为空
 * 我们可以借助第二类斯特林数，由于盒子是相同的，我们只需要枚举有几个空盒子就可以了
 * ans[n][k] = sum({n,k-i}) (i=[0,k-1])
 */
struct ppp {
    int S[10][10];
    void initS2()
    {
        //预处理第二类斯特林数
        int n = 1e6;
        S[0][0] = 1;
        int R = n - 5;
        for(int i = 1; i <= n; i ++ )
            for(int j = 1; j <= n; j ++)
                S[i][j] = (S[i - 1][j - 1] + S[i - 1][j] * j % MOD) % MOD;
    }
    int get_first_Strling(int n,int k) {
        int res = 0;
        for(int i=0;i<=k-1;i++) 
            res = (res + S[n][k-i])%MOD;
        return res;
    }
};

/***
 * 后续补充同一行/列的第一类斯特林数的计算
 */


/**
 * 贝尔数
 */


/**
 * 卡特兰数
 */
int catalan(int n) {
    int comb = C(2*n,n);
    return comb * qpow(n + 1, MOD - 2) % MOD;
}


/**
 * 分拆数
 */



/**
  * 欧拉数
  */


  
/**
 * 多项式与生成函数
 */
//快速傅里叶变换
using cd = complex<double>;//虚数
const double PI = acos(-1.0);

void fft(vector<cd>& a, bool invert) {
    int n = (int)a.size();
    // 位逆序置换
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2.0 * PI / len * (invert ? -1.0 : 1.0);
        cd wlen(cos(angle), sin(angle));

        for (int i = 0; i < n; i += len) {
            cd w(1.0);
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;

                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd& x : a) x /= n;
    }
}

vector<int> convolution(vector<int>& a, vector<int>& b) {
    if (a.empty() || b.empty()) return {};

    int resultSize = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < resultSize) n <<= 1;

    vector<cd> fa(a.begin(), a.end());
    vector<cd> fb(b.begin(), b.end());
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);

    for (int i = 0; i < n; ++i) {
        fa[i] *= fb[i];
    }

    fft(fa, true);

    vector<int> result(resultSize);
    for (int i = 0; i < resultSize; ++i) {
        result[i] = llround(fa[i].real());
    }
    return result;
}

//快速数论变换
//只能在模998244353使用
const int G = 3;
void ntt(vector<int>& a, bool invert) {
    int n = (int)a.size();

    // 位逆序置换
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;

        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }

        j ^= bit;

        if (i < j) {
            swap(a[i], a[j]);
        }
    }

    for (int len = 2; len <= n; len <<= 1) {
        // len 次单位根
        int wlen = qpow(G, (MOD - 1) / len);

        if (invert) {
            wlen = qpow(wlen, MOD - 2);
        }

        for (int i = 0; i < n; i += len) {
            int w = 1;

            for (int j = 0; j < len / 2; ++j) {
                int u = a[i + j];
                int v = a[i + j + len / 2] * w % MOD;

                a[i + j] = u + v;
                if (a[i + j] >= MOD) {
                    a[i + j] -= MOD;
                }

                a[i + j + len / 2] = u - v;
                if (a[i + j + len / 2] < 0) {
                    a[i + j + len / 2] += MOD;
                }

                w = w * wlen % MOD;
            }
        }
    }

    if (invert) {
        int inv_n = qpow(n, MOD - 2);

        for (int& x : a) {
            x = x * inv_n % MOD;
        }
    }
}

vector<int> convolution(vector<int> a, vector<int> b) {
    if (a.empty() || b.empty()) {
        return {};
    }

    int result_size = (int)a.size() + (int)b.size() - 1;
    int n = 1;

    while (n < result_size) {
        n <<= 1;
    }

    // 998244353 最大支持 2^23 点NTT
    if (n > (1 << 23)) {
        throw runtime_error("NTT length exceeds 2^23");
    }

    a.resize(n);
    b.resize(n);

    ntt(a, false);
    ntt(b, false);

    for (int i = 0; i < n; ++i) {
        a[i] = a[i] * b[i] % MOD;
    }

    ntt(a, true);
    a.resize(result_size);

    return a;
}

//快速沃尔什变换


//矩阵类
struct Matrix {
    int n;
    int a[2][2];

    Matrix(bool identity = false) {
        memset(a,0,sizeof(a));
        //单位矩阵
        if (identity) {
            for(int i=0;i<n;i++)
                a[i][i] = 1;
        }
    }

    Matrix operator*(const Matrix& oth) const {
        Matrix res;
        for(int i=0;i<n;i++) {
            for(int j=0;j<n;j++) {
                for(int k=0;k<n;k++) {
                    res.a[i][j] += a[i][k] * oth.a[k][j]%MOD;
                    res.a[i][j] %= MOD;
                }
            }
        }
        return res;
    }
};
Matrix matrixpow(Matrix a,int b) {
    Matrix res(1);
    while(b) {
        if(b&1) res = res*a;
        a = a*a;
        b >>= 1;
    }
    return res;
}


/***
 * 拉格朗日插值
 * 给定 n 个点 (xs[i], ys[i])，求 f(k)
 * O(n^2)
 */
int lagIner(vector<int>& xs,vector<int>& ys,int k) {
    int n = xs.size();
    k %= MOD;

    int answer = 0;

    for (int i = 0; i < n; ++i) {
        int numerator = 1;
        int denominator = 1;

        for (int j = 0; j < n; ++j) {
            if (i == j) continue;

            numerator = numerator*(k - xs[j])%MOD;
            denominator = denominator*(xs[i] - xs[j])%MOD;
        }

        int term = ys[i] % MOD;
        term = term* numerator%MOD;
        term = term* qpow(denominator,MOD-2)%MOD;

        answer += term;
        if (answer >= MOD) answer -= MOD;
    }

    return answer;
}

