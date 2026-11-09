# 回文专题：回文子串与回文子序列

> 本文整理算法竞赛中常见的回文相关问题，包括回文子序列、回文子串、区间 DP、LCS、Manacher、字符串哈希和回文自动机等方法。

---

## 目录

- [1. 基本概念](#1-基本概念)
- [2. 最长回文子序列：区间 DP](#2-最长回文子序列区间-dp)
- [3. 最长回文子序列：转化为 LCS](#3-最长回文子序列转化为-lcs)
- [4. 构造最长回文子序列](#4-构造最长回文子序列)
- [5. 最少删除或插入字符形成回文串](#5-最少删除或插入字符形成回文串)
- [6. 判断是否存在指定长度的回文子序列](#6-判断是否存在指定长度的回文子序列)
- [7. 统计回文子序列数量](#7-统计回文子序列数量)
- [8. 统计本质不同的回文子序列](#8-统计本质不同的回文子序列)
- [9. 按长度统计回文子序列](#9-按长度统计回文子序列)
- [10. 回文子序列的大规模优化](#10-回文子序列的大规模优化)
- [11. 回文子串常见算法](#11-回文子串常见算法)
- [12. 方法总结](#12-方法总结)
- [13. 常见易错点](#13-常见易错点)

---

## 1. 基本概念

### 1.1 子串

**子串（substring）**必须由原字符串中连续的一段字符组成。

例如对于：

```text
abcde
```

下面是它的子串：

```text
abc
bcd
de
```

而 `ace` 不是子串，因为它在原串中不连续。

---

### 1.2 子序列

**子序列（subsequence）**是从原字符串中选取若干个位置，并保持这些位置的相对顺序不变。

字符不要求连续。

例如对于：

```text
abcde
```

下面都是它的子序列：

```text
ace
abd
bcde
```

---

### 1.3 回文串

正序和逆序完全相同的字符串称为回文串。

例如：

```text
a
aa
aba
abba
abcba
```

---

### 1.4 回文子串与回文子序列

- **回文子串**：连续选择一段字符，且结果是回文串。
- **回文子序列**：选择若干个位置，保持相对顺序不变，且结果是回文串。

例如：

```text
s = "abcba"
```

- `"bcb"` 是回文子串。
- `"aca"` 是回文子序列，但不是子串。

如果题目描述为：

> 选取若干个位置，保持相对顺序不变。

那么求的是**回文子序列**，而不是回文子串。

---

# 回文子序列

## 2. 最长回文子序列：区间 DP

### 2.1 状态定义

设：

```cpp
dp[l][r]
```

表示字符串区间 `[l,r]` 中最长回文子序列的长度。

---

### 2.2 状态转移

如果两端字符相同：

```cpp
s[l] == s[r]
```

则可以同时选择两端：

```cpp
dp[l][r] = dp[l + 1][r - 1] + 2;
```

如果两端字符不同，则至少舍弃其中一端：

```cpp
dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
```

边界：

```cpp
dp[i][i] = 1;
```

空区间的最长回文子序列长度为 `0`。

---

### 2.3 完整代码

```cpp
#include <bits/stdc++.h>
using namespace std;

int longestPalindromeSubsequence(const string& s) {
    int n = s.size();

    if (n == 0) {
        return 0;
    }

    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;

            if (s[l] == s[r]) {
                if (len == 2) {
                    dp[l][r] = 2;
                } else {
                    dp[l][r] = dp[l + 1][r - 1] + 2;
                }
            } else {
                dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
            }
        }
    }

    return dp[0][n - 1];
}
```

复杂度：

- 时间复杂度：`O(n²)`
- 空间复杂度：`O(n²)`

---

### 2.4 一维空间优化

如果只需要最长长度，不需要恢复具体方案，可以把空间优化到 `O(n)`。

```cpp
#include <bits/stdc++.h>
using namespace std;

int longestPalindromeSubsequence(const string& s) {
    int n = s.size();
    vector<int> dp(n, 0);

    for (int l = n - 1; l >= 0; l--) {
        dp[l] = 1;

        int diagonal = 0;

        for (int r = l + 1; r < n; r++) {
            int old = dp[r];

            if (s[l] == s[r]) {
                dp[r] = diagonal + 2;
            } else {
                dp[r] = max(dp[r], dp[r - 1]);
            }

            diagonal = old;
        }
    }

    return n == 0 ? 0 : dp[n - 1];
}
```

其中：

- 更新前的 `dp[r]` 表示 `dp[l+1][r]`
- 更新后的 `dp[r-1]` 表示 `dp[l][r-1]`
- `diagonal` 表示 `dp[l+1][r-1]`

复杂度：

- 时间复杂度：`O(n²)`
- 空间复杂度：`O(n)`

---

## 3. 最长回文子序列：转化为 LCS

令：

```text
t = reverse(s)
```

则有：

```text
LPS(s) = LCS(s, reverse(s))
```

其中：

- `LPS`：最长回文子序列
- `LCS`：最长公共子序列

---

### 3.1 二维 LCS

设：

```cpp
dp[i][j]
```

表示 `s` 的前 `i` 个字符与 `t` 的前 `j` 个字符的最长公共子序列长度。

转移：

```cpp
if (s[i - 1] == t[j - 1])
    dp[i][j] = dp[i - 1][j - 1] + 1;
else
    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
```

代码：

```cpp
#include <bits/stdc++.h>
using namespace std;

int longestPalindromeSubsequence(string s) {
    string t = s;
    reverse(t.begin(), t.end());

    int n = s.size();
    vector<vector<int>> dp(
        n + 1,
        vector<int>(n + 1, 0)
    );

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (s[i - 1] == t[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(
                    dp[i - 1][j],
                    dp[i][j - 1]
                );
            }
        }
    }

    return dp[n][n];
}
```

复杂度：

- 时间复杂度：`O(n²)`
- 空间复杂度：`O(n²)`

---

### 3.2 滚动数组优化

```cpp
#include <bits/stdc++.h>
using namespace std;

int longestPalindromeSubsequence(string s) {
    string t = s;
    reverse(t.begin(), t.end());

    int n = s.size();
    vector<int> dp(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        int diagonal = 0;

        for (int j = 1; j <= n; j++) {
            int old = dp[j];

            if (s[i - 1] == t[j - 1]) {
                dp[j] = diagonal + 1;
            } else {
                dp[j] = max(dp[j], dp[j - 1]);
            }

            diagonal = old;
        }
    }

    return dp[n];
}
```

复杂度：

- 时间复杂度：`O(n²)`
- 空间复杂度：`O(n)`

---

### 3.3 区间 DP 与 LCS 的选择

| 情况 | 推荐方法 |
|---|---|
| 想直接分析回文结构 | 区间 DP |
| 已有 LCS 模板 | 转化为 LCS |
| 需要恢复具体方案 | 区间 DP 更直观 |
| 只求长度且空间有限 | 一维 DP |
| 需要 bitset 优化 | 转化为 LCS |

---

## 4. 构造最长回文子序列

如果不仅要求最长长度，还要求输出具体的最长回文子序列，可以在完成区间 DP 后倒推。

### 4.1 倒推思路

从区间 `[0,n-1]` 开始：

1. 如果两端字符相同，并且选取两端能够得到最优解，则同时选取两端。
2. 否则移动到答案更大的子区间。
3. 如果最后剩下单个字符，则把它作为回文中心。

---

### 4.2 完整代码

```cpp
#include <bits/stdc++.h>
using namespace std;

string buildLongestPalindromeSubsequence(const string& s) {
    int n = s.size();

    if (n == 0) {
        return "";
    }

    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;

            if (s[l] == s[r]) {
                int inner = (len == 2 ? 0 : dp[l + 1][r - 1]);
                dp[l][r] = inner + 2;
            } else {
                dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
            }
        }
    }

    string leftPart;
    string rightPart;

    int l = 0;
    int r = n - 1;

    while (l < r) {
        int inner = (l + 1 > r - 1 ? 0 : dp[l + 1][r - 1]);

        if (s[l] == s[r] &&
            dp[l][r] == inner + 2) {
            leftPart.push_back(s[l]);
            rightPart.push_back(s[r]);

            l++;
            r--;
        } else if (dp[l + 1][r] >= dp[l][r - 1]) {
            l++;
        } else {
            r--;
        }
    }

    if (l == r) {
        leftPart.push_back(s[l]);
    }

    reverse(rightPart.begin(), rightPart.end());

    return leftPart + rightPart;
}
```

如果最长回文子序列不唯一，这段代码会返回其中任意一个。

---

## 5. 最少删除或插入字符形成回文串

### 5.1 最少删除字符

如果只能删除字符，要使剩余字符串成为回文串，就应该保留一个最长回文子序列。

因此：

```text
最少删除次数 = n - 最长回文子序列长度
```

代码：

```cpp
int minimumDeletionsToPalindrome(const string& s) {
    int lps = longestPalindromeSubsequence(s);
    return static_cast<int>(s.size()) - lps;
}
```

例如：

```text
s = "abcda"
```

最长回文子序列可以是：

```text
aca
ada
```

长度为 `3`，因此最少删除次数为：

```text
5 - 3 = 2
```

---

### 5.2 最少插入字符

要将字符串变成回文串，最少插入次数同样满足：

```text
最少插入次数 = n - 最长回文子序列长度
```

也可以直接使用区间 DP。

设：

```cpp
dp[l][r]
```

表示将区间 `[l,r]` 变成回文串所需的最少插入次数。

转移：

```cpp
if (s[l] == s[r])
    dp[l][r] = dp[l + 1][r - 1];
else
    dp[l][r] = min(dp[l + 1][r], dp[l][r - 1]) + 1;
```

代码：

```cpp
#include <bits/stdc++.h>
using namespace std;

int minimumInsertionsToPalindrome(const string& s) {
    int n = s.size();

    if (n == 0) {
        return 0;
    }

    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;

            if (s[l] == s[r]) {
                dp[l][r] =
                    (len == 2 ? 0 : dp[l + 1][r - 1]);
            } else {
                dp[l][r] =
                    min(dp[l + 1][r], dp[l][r - 1]) + 1;
            }
        }
    }

    return dp[0][n - 1];
}
```

---

## 6. 判断是否存在指定长度的回文子序列

### 6.1 一般情况

如果要求判断是否存在长度至少为 `k` 的回文子序列，可以先求最长回文子序列长度 `L`，然后判断：

```cpp
L >= k
```

复杂度为 `O(n²)`。

---

### 6.2 长度为 1

只要字符串非空，就存在长度为 `1` 的回文子序列。

---

### 6.3 长度为 2

只要某个字符出现至少两次，就存在长度为 `2` 的回文子序列。

```cpp
bool hasPalindromeSubsequenceLength2(const string& s) {
    vector<int> count(256, 0);

    for (unsigned char c : s) {
        count[c]++;

        if (count[c] >= 2) {
            return true;
        }
    }

    return false;
}
```

时间复杂度：`O(n)`。

---

### 6.4 长度为 3

长度为 `3` 的回文子序列形如：

```text
a ? a
```

因此只需要判断是否存在两个相同字符，并且它们之间至少有一个位置。

```cpp
bool hasPalindromeSubsequenceLength3(const string& s) {
    vector<int> first(256, -1);

    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        unsigned char c = s[i];

        if (first[c] != -1 && i - first[c] >= 2) {
            return true;
        }

        if (first[c] == -1) {
            first[c] = i;
        }
    }

    return false;
}
```

时间复杂度：`O(n)`。

---

## 7. 统计回文子序列数量

需要特别区分两种计数方式：

1. 按选择的位置集合计数。
2. 按最终得到的字符串计数，即统计本质不同的回文子序列。

本节讨论第一种。

---

### 7.1 状态定义

设：

```cpp
dp[l][r]
```

表示区间 `[l,r]` 中非空回文子序列的数量。

如果选择的位置集合不同，就认为是不同方案。

---

### 7.2 两端字符不同

如果：

```cpp
s[l] != s[r]
```

则使用容斥：

```cpp
dp[l][r]
    = dp[l + 1][r]
    + dp[l][r - 1]
    - dp[l + 1][r - 1];
```

原因是：

- `dp[l+1][r]`：不选左端。
- `dp[l][r-1]`：不选右端。
- 两者都包含 `dp[l+1][r-1]`，因此需要减去一次。

---

### 7.3 两端字符相同

如果：

```cpp
s[l] == s[r]
```

则：

```cpp
dp[l][r]
    = dp[l + 1][r]
    + dp[l][r - 1]
    + 1;
```

新增方案包括：

- 单独同时选择两端；
- 在内部任意回文子序列两侧加上相同字符。

化简后即可得到上述转移。

---

### 7.4 完整代码

```cpp
#include <bits/stdc++.h>
using namespace std;

long long countPalindromeSubsequences(
    const string& s,
    long long mod
) {
    int n = s.size();

    if (n == 0) {
        return 0;
    }

    vector<vector<long long>> dp(
        n,
        vector<long long>(n, 0)
    );

    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;

            if (s[l] == s[r]) {
                dp[l][r] = (
                    dp[l + 1][r]
                    + dp[l][r - 1]
                    + 1
                ) % mod;
            } else {
                long long inner =
                    (len == 2 ? 0 : dp[l + 1][r - 1]);

                dp[l][r] = (
                    dp[l + 1][r]
                    + dp[l][r - 1]
                    - inner
                ) % mod;

                if (dp[l][r] < 0) {
                    dp[l][r] += mod;
                }
            }
        }
    }

    return dp[0][n - 1];
}
```

复杂度：

- 时间复杂度：`O(n²)`
- 空间复杂度：`O(n²)`

---

### 7.5 示例

对于：

```text
s = "aaa"
```

按位置计数：

- 长度为 `1`：3 种
- 长度为 `2`：3 种
- 长度为 `3`：1 种

总数：

```text
3 + 3 + 1 = 7
```

虽然最终得到的不同字符串只有：

```text
"a"
"aa"
"aaa"
```

但位置选择不同仍然算不同方案。

---

## 8. 统计本质不同的回文子序列

本质不同指：

> 如果最终得到的字符序列相同，就只计算一次。

例如：

```text
s = "aaa"
```

本质不同的非空回文子序列只有：

```text
"a"
"aa"
"aaa"
```

答案为 `3`。

---

### 8.1 状态定义

设：

```cpp
dp[l][r]
```

表示区间 `[l,r]` 中本质不同的非空回文子序列数量。

---

### 8.2 两端字符不同

如果：

```cpp
s[l] != s[r]
```

则仍然使用容斥：

```cpp
dp[l][r]
    = dp[l + 1][r]
    + dp[l][r - 1]
    - dp[l + 1][r - 1];
```

---

### 8.3 两端字符相同

设：

```cpp
s[l] == s[r]
```

在开区间 `(l,r)` 中寻找与两端字符相同的位置：

- `low`：从左向右第一个满足 `s[low] == s[l]` 的位置。
- `high`：从右向左第一个满足 `s[high] == s[l]` 的位置。

根据中间相同字符的数量分三种情况。

#### 情况一：中间没有相同字符

即：

```text
low > high
```

转移：

```cpp
dp[l][r]
    = 2 * dp[l + 1][r - 1] + 2;
```

额外新增：

```text
s[l]
s[l]s[r]
```

---

#### 情况二：中间恰好有一个相同字符

即：

```text
low == high
```

转移：

```cpp
dp[l][r]
    = 2 * dp[l + 1][r - 1] + 1;
```

---

#### 情况三：中间至少有两个相同字符

即：

```text
low < high
```

转移：

```cpp
dp[l][r]
    = 2 * dp[l + 1][r - 1]
    - dp[low + 1][high - 1];
```

减去的部分是重复计算的回文子序列。

---

### 8.4 复杂度

如果每次都在线性扫描寻找 `low` 和 `high`，复杂度会达到：

```text
O(n³)
```

通常需要预处理：

- 每个位置右侧第一个相同字符；
- 每个位置左侧第一个相同字符。

从而将总复杂度优化为：

- 时间复杂度：`O(n²)`
- 空间复杂度：`O(n²)`

---

### 8.5 完整代码

```cpp
#include <bits/stdc++.h>
using namespace std;

long long countDistinctPalindromeSubsequences(
    const string& s,
    long long mod
) {
    int n = s.size();

    if (n == 0) {
        return 0;
    }

    vector<int> nextSame(n, n);
    vector<int> previousSame(n, -1);

    vector<int> last(256, -1);

    for (int i = 0; i < n; i++) {
        unsigned char c = s[i];
        previousSame[i] = last[c];
        last[c] = i;
    }

    fill(last.begin(), last.end(), n);

    for (int i = n - 1; i >= 0; i--) {
        unsigned char c = s[i];
        nextSame[i] = last[c];
        last[c] = i;
    }

    vector<vector<long long>> dp(
        n,
        vector<long long>(n, 0)
    );

    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    auto get = [&](int l, int r) -> long long {
        if (l > r) {
            return 0;
        }

        return dp[l][r];
    };

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;

            if (s[l] != s[r]) {
                dp[l][r] = (
                    dp[l + 1][r]
                    + dp[l][r - 1]
                    - get(l + 1, r - 1)
                ) % mod;
            } else {
                int low = nextSame[l];
                int high = previousSame[r];

                if (low > high) {
                    dp[l][r] = (
                        2 * get(l + 1, r - 1) + 2
                    ) % mod;
                } else if (low == high) {
                    dp[l][r] = (
                        2 * get(l + 1, r - 1) + 1
                    ) % mod;
                } else {
                    dp[l][r] = (
                        2 * get(l + 1, r - 1)
                        - get(low + 1, high - 1)
                    ) % mod;
                }
            }

            dp[l][r] %= mod;

            if (dp[l][r] < 0) {
                dp[l][r] += mod;
            }
        }
    }

    return dp[0][n - 1];
}
```

---

## 9. 按长度统计回文子序列

如果题目要求：

> 统计长度恰好为 `k` 的回文子序列数量。

可以增加一维状态。

---

### 9.1 状态定义

设：

```cpp
dp[l][r][k]
```

表示区间 `[l,r]` 中长度恰好为 `k` 的回文子序列数量。

这里通常按选择的位置集合计数。

---

### 9.2 状态转移

先通过容斥统计不要求同时选择两端的方案：

```cpp
dp[l][r][k]
    = dp[l + 1][r][k]
    + dp[l][r - 1][k]
    - dp[l + 1][r - 1][k];
```

如果两端字符相同：

```cpp
s[l] == s[r]
```

则还可以同时选择两端：

```cpp
dp[l][r][k] += dp[l + 1][r - 1][k - 2];
```

为了统一处理长度为 `2` 的情况，可以定义：

```text
空区间中长度为 0 的子序列数量为 1
```

即：

```text
dp[空区间][0] = 1
```

---

### 9.3 复杂度

- 时间复杂度：`O(n²k)`
- 空间复杂度：`O(n²k)`

只适用于 `n` 和 `k` 都较小的情况。

如果只依赖相邻区间，可以根据具体转移尝试进行空间优化。

---

## 10. 回文子序列的大规模优化

普通最长回文子序列需要 `O(n²)` 时间，一般适合 `n` 在几千以内的情况。

---

### 10.1 Bitset 优化 LCS

最长回文子序列可以转化为：

```text
LCS(s, reverse(s))
```

因此可以使用 bitset 优化 LCS。

复杂度大致为：

```text
O(n² / word_size)
```

在 64 位机器上，可以理解为约：

```text
O(n² / 64)
```

适用于：

- `n` 达到几万；
- 字符集较小；
- 只需要求长度。

---

### 10.2 稀疏匹配优化

LCS 存在 Hunt–Szymanski 等稀疏匹配算法。

如果两个字符串之间的字符匹配对很少，可以获得较好的性能。

但回文子序列中比较的是：

```text
s 与 reverse(s)
```

如果字符集很小，字符匹配对通常很多，因此稀疏优化不一定有效。

---

### 10.3 超大数据范围

对于一般字符串，精确求最长回文子序列没有简单的线性或 `O(n log n)` 通用算法。

如果：

```text
n ≈ 2 × 10^5
```

题目通常还会提供特殊条件，例如：

- 字符集或字符出现次数有限；
- 只判断是否存在较短的回文子序列；
- 要求近似答案；
- 字符串具有特殊结构；
- 查询的 `k` 很小；
- 允许使用随机化或位运算优化。

---

# 回文子串

## 11. 回文子串常见算法

回文子串要求选择的字符必须连续。

常见方法如下：

| 问题 | 方法 | 时间复杂度 |
|---|---|---:|
| 判断一个字符串是否回文 | 双指针 | `O(n)` |
| 判断多个区间是否回文 | 字符串哈希 | 预处理 `O(n)`，查询 `O(1)` |
| 最长回文子串 | 中心扩展 | `O(n²)` |
| 最长回文子串 | 区间 DP | `O(n²)` |
| 最长回文子串 | Manacher | `O(n)` |
| 统计回文子串个数 | Manacher | `O(n)` |
| 维护本质不同的回文子串 | 回文自动机 PAM | 接近 `O(n)` |

---

### 11.1 双指针判断回文

```cpp
bool isPalindrome(const string& s) {
    int l = 0;
    int r = static_cast<int>(s.size()) - 1;

    while (l < r) {
        if (s[l] != s[r]) {
            return false;
        }

        l++;
        r--;
    }

    return true;
}
```

时间复杂度：`O(n)`。

---

### 11.2 中心扩展

每个回文串都存在一个中心。

回文中心有两种：

1. 单个字符，例如 `"aba"` 的中心是 `b`。
2. 两个字符之间，例如 `"abba"` 的中心在两个 `b` 之间。

因此可以枚举所有中心并向两侧扩展。

```cpp
#include <bits/stdc++.h>
using namespace std;

string longestPalindromeSubstring(const string& s) {
    int n = s.size();

    if (n == 0) {
        return "";
    }

    int answerLeft = 0;
    int answerLength = 1;

    auto expand = [&](int l, int r) {
        while (l >= 0 && r < n && s[l] == s[r]) {
            int length = r - l + 1;

            if (length > answerLength) {
                answerLength = length;
                answerLeft = l;
            }

            l--;
            r++;
        }
    };

    for (int center = 0; center < n; center++) {
        expand(center, center);
        expand(center, center + 1);
    }

    return s.substr(answerLeft, answerLength);
}
```

复杂度：

- 时间复杂度：`O(n²)`
- 空间复杂度：`O(1)`

---

### 11.3 区间 DP 判断回文子串

设：

```cpp
dp[l][r]
```

表示子串 `s[l..r]` 是否为回文串。

转移：

```cpp
dp[l][r]
    = (s[l] == s[r])
      && (r - l <= 2 || dp[l + 1][r - 1]);
```

代码：

```cpp
#include <bits/stdc++.h>
using namespace std;

string longestPalindromeSubstring(const string& s) {
    int n = s.size();

    if (n == 0) {
        return "";
    }

    vector<vector<bool>> dp(
        n,
        vector<bool>(n, false)
    );

    int answerLeft = 0;
    int answerLength = 1;

    for (int i = 0; i < n; i++) {
        dp[i][i] = true;
    }

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;

            if (s[l] == s[r] &&
                (len <= 2 || dp[l + 1][r - 1])) {
                dp[l][r] = true;

                if (len > answerLength) {
                    answerLength = len;
                    answerLeft = l;
                }
            }
        }
    }

    return s.substr(answerLeft, answerLength);
}
```

复杂度：

- 时间复杂度：`O(n²)`
- 空间复杂度：`O(n²)`

---

### 11.4 Manacher 算法

Manacher 可以在线性时间内求出：

- 每个位置的最大回文半径；
- 最长回文子串；
- 回文子串总数。

通常将原字符串插入分隔符，将奇数长度回文和偶数长度回文统一处理。

例如：

```text
abba
```

转换为：

```text
^#a#b#b#a#$
```

核心复杂度：

```text
O(n)
```

适用于：

- 字符串很长；
- 需要最长回文子串；
- 需要统计所有回文子串数量；
- 需要每个中心的回文半径。

---

### 11.5 字符串哈希

如果有大量询问：

> 子串 `s[l..r]` 是否为回文串？

可以分别维护：

- 原字符串的前缀哈希；
- 反转字符串的前缀哈希。

若原串区间的哈希值与反转串对应区间的哈希值相等，则该区间大概率为回文串。

复杂度：

- 预处理：`O(n)`
- 单次查询：`O(1)`

为了降低哈希冲突概率，可以使用：

- 双模哈希；
- `unsigned long long` 自然溢出哈希；
- 双哈希与自然溢出结合。

---

### 11.6 回文自动机 PAM

回文自动机也称：

- Palindromic Tree
- Eertree
- PAM

它可以维护字符串中的所有本质不同回文子串。

每个节点对应一个本质不同的回文子串。

常见维护内容包括：

- 回文子串长度；
- 出现次数；
- 最长回文后缀；
- 失配指针 `fail`；
- 不同回文子串个数；
- 每个前缀中的回文后缀信息。

复杂度通常接近：

```text
O(n)
```

适用于：

- 统计本质不同的回文子串；
- 统计每个回文子串出现次数；
- 在线向字符串末尾添加字符；
- 回文子串相关 DP。

注意：

> 回文自动机维护的是回文子串，不是回文子序列。

---

## 12. 方法总结

### 12.1 回文子序列

| 问题 | 常用方法 | 时间复杂度 | 空间复杂度 |
|---|---|---:|---:|
| 最长回文子序列长度 | 区间 DP | `O(n²)` | `O(n²)` |
| 最长回文子序列长度 | 一维区间 DP | `O(n²)` | `O(n)` |
| 最长回文子序列长度 | LCS | `O(n²)` | `O(n²)` |
| 构造最长回文子序列 | 区间 DP + 倒推 | `O(n²)` | `O(n²)` |
| 最少删除形成回文串 | `n - LPS` | `O(n²)` | 视 LPS 实现而定 |
| 最少插入形成回文串 | `n - LPS` 或区间 DP | `O(n²)` | `O(n²)` |
| 按位置统计回文子序列 | 区间 DP | `O(n²)` | `O(n²)` |
| 统计本质不同回文子序列 | 区间 DP + 去重 | `O(n²)` | `O(n²)` |
| 长度恰好为 `k` 的数量 | 三维 DP | `O(n²k)` | `O(n²k)` |
| 大规模最长回文子序列 | bitset LCS | 约 `O(n²/64)` | 与实现有关 |

---

### 12.2 回文子串

| 问题 | 常用方法 | 时间复杂度 |
|---|---|---:|
| 判断整个字符串是否回文 | 双指针 | `O(n)` |
| 最长回文子串 | 中心扩展 | `O(n²)` |
| 最长回文子串 | 区间 DP | `O(n²)` |
| 最长回文子串 | Manacher | `O(n)` |
| 多次判断区间是否回文 | 字符串哈希 | 查询 `O(1)` |
| 统计本质不同回文子串 | 回文自动机 PAM | 接近 `O(n)` |

---

## 13. 常见易错点

### 13.1 混淆子串与子序列

- 子串要求连续。
- 子序列只要求相对顺序不变。

题目中出现：

> 选择若干位置，保持相对顺序不变。

通常就是子序列。

---

### 13.2 区间 DP 的枚举顺序错误

状态 `dp[l][r]` 依赖更短的区间，所以必须按照区间长度从小到大枚举：

```cpp
for (int len = 1; len <= n; len++) {
    for (int l = 0; l + len - 1 < n; l++) {
        int r = l + len - 1;
    }
}
```

也可以让左端点从右向左枚举。

---

### 13.3 忽略空区间

对于：

```cpp
dp[l + 1][r - 1]
```

当区间长度为 `2` 时，会出现空区间。

通常需要：

- 单独判断 `len == 2`；
- 或定义辅助函数，使空区间答案为 `0`。

---

### 13.4 计数问题没有说明去重方式

“统计回文子序列数量”可能有两种含义：

1. 位置集合不同就算不同方案。
2. 最终字符串相同只算一次。

例如：

```text
s = "aaa"
```

- 按位置计数：`7`
- 本质不同计数：`3`

做题时必须先确认题目的计数定义。

---

### 13.5 取模减法得到负数

计数 DP 中经常出现：

```cpp
a + b - c
```

取模时应写成：

```cpp
answer = (a + b - c) % mod;

if (answer < 0) {
    answer += mod;
}
```

如果可能连续减去多个值，可以写成：

```cpp
answer %= mod;
answer = (answer + mod) % mod;
```

---

### 13.6 把回文子串算法用于回文子序列

以下算法主要用于回文子串：

- Manacher
- 中心扩展
- 回文自动机
- 正反字符串哈希

它们不能直接解决一般的最长回文子序列问题。

最长回文子序列的基础方法仍然是：

```cpp
if (s[l] == s[r]) {
    dp[l][r] = dp[l + 1][r - 1] + 2;
} else {
    dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
}
```

---

## 结语

回文题首先要判断研究对象是：

```text
回文子串
```

还是：

```text
回文子序列
```

如果要求连续，通常考虑：

- 中心扩展；
- Manacher；
- 字符串哈希；
- 回文自动机。

如果只要求保持相对顺序，通常考虑：

- 区间 DP；
- LCS；
- 计数 DP；
- bitset 优化。

其中最长回文子序列最核心的状态转移是：

```cpp
if (s[l] == s[r]) {
    dp[l][r] = dp[l + 1][r - 1] + 2;
} else {
    dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
}
```