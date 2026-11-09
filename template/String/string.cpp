#include <bits/stdc++.h>
using namespace  std;

/**
 * 字典树
 */
struct trie {
  int nex[100000][26], cnt;
  bool exist[100000];

  void insert(string s) {
    int p = 0;
    int n = s.length();
    for (int i = 0; i < n; i++) {
      int c = s[i] - 'a';
      if (!nex[p][c]) nex[p][c] = ++cnt;
      p = nex[p][c];
    }
    exist[p] = true;
  }

  bool find(string s) {
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
 * KMP
 * Z函数
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
 */
struct ACAutomation {
    struct node {
        int next[26];
        int fail;
        int out;
        node() {
            memset(next,0,sizeof(next));
            fail = 0;
            out = 0;
        }
    };
 
    vector<node> trie;
    ACAutomation() {
        trie.push_back(node());
    }
    void insert(string& s) {
        int u = 0;
        for(char ch : s) {
            int c = ch-'A';
            if(trie[u].next[c] == 0) {
                trie[u].next[c] = trie.size();
                trie.push_back(node());
            }
            u = trie[u].next[c];
        }
        trie[u].out = 1;
    }
    void build() {
        queue<int> pq;
        for(int c=0;c<26;c++) {
            if(trie[0].next[c] != 0) {
                pq.push(trie[0].next[c]);
            }
        }
        while(pq.size()) {
            int u = pq.front();
            pq.pop();
            trie[u].out |= trie[trie[u].fail].out;

            for(int c=0;c<26;c++) {
                int v = trie[u].next[c];
                if(v == 0) {
                    trie[u].next[c] = trie[trie[u].fail].next[c];
                } else {
                    trie[trie[u].next[c]].fail = trie[trie[u].fail].next[c];
                    pq.push(trie[u].next[c]);
                }
            }
        }
    }
};