#include <bits/stdc++.h>
using namespace  std;

/**
 * 字典树
 */
struct trie {
  int nex[100000][26], cnt;
  bool exist[100000];  // 该结点结尾的字符串是否存在

  void insert(string s) {  // 插入字符串
    int p = 0;
    int n = s.length();
    for (int i = 0; i < n; i++) {
      int c = s[i] - 'a';
      if (!nex[p][c]) nex[p][c] = ++cnt;  // 如果没有，就添加结点
      p = nex[p][c];
    }
    exist[p] = true;
  }

  bool find(string s) {  // 查找字符串
    int p = 0;
    int n = s.length();
    for (int i = 0; i < n; i++) {
      int c = s[i] - 'a';
      if (!nex[p][c]) return 0;
      p = nex[p][c];
    }
    return exist[p];
  }
};

/**
 * KMP算法前缀函数
 */
vector<int> prefix_function(string s) {
  int n = (int)s.length();
  vector<int> pi(n);
  for (int i = 1; i < n; i++) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) j++;
    pi[i] = j;
  }
  return pi;
}


/**
 * AC自动机
 * 
 */
struct ACAutomaton {
    struct Node {
        int next[26];
        int fail;
        int out; // 这个状态能匹配到哪些模式串，用 bitmask 表示
        Node() {
            memset(next, 0, sizeof(next));
            fail = 0;
            out = 0;
        }
    };
 
    vector<Node> trie;
 
    ACAutomaton() {
        trie.push_back(Node()); // 0 号点是根
    }
 
    void insert(const string &s, int id) {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (!trie[u].next[c]) {
                trie[u].next[c] = trie.size();
                trie.push_back(Node());
            }
            u = trie[u].next[c];
        }
        trie[u].out |= (1 << id);
    }
 
    void build() {
        queue<int> q;
 
        // 根的第一层处理
        for (int c = 0; c < 26; c++) {
            int v = trie[0].next[c];
            if (v) {
                trie[v].fail = 0;
                q.push(v);
            }
        }
 
        while (!q.empty()) {
            int u = q.front();
            q.pop();
 
            // fail 链上传递输出信息
            trie[u].out |= trie[trie[u].fail].out;
 
            for (int c = 0; c < 26; c++) {
                int v = trie[u].next[c];
                if (v) {
                    trie[v].fail = trie[trie[u].fail].next[c];
                    q.push(v);
                } else {
                    trie[u].next[c] = trie[trie[u].fail].next[c];
                }
            }
        }
    }
};