#include <bits/stdc++.h>
using namespace std;
#define int long long
const int MOD = 1e9 + 7;

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