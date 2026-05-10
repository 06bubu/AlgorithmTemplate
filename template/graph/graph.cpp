#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N = 2e5 + 10;
const int INF = 1e16;
const int MOD = 987654321;

class tututu_suodian
{
private:
    int n, m;
    vector<int> e[N];
    int low[N], dfn[N], cnt = 0;
    int sta[N], top = 0, in_sta[N];

    int scc[N], scc_num[N], num; // 所属的强连通分量编号   强连通分量的大小   强连通分量数目
public:
    void tarjan(int u)
    {
        dfn[u] = low[u] = ++cnt;
        sta[++top] = u;
        in_sta[u] = 1;
        for (int v : e[u])
        {
            if (!dfn[v]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (in_sta[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u])
        {
            num++;
            while (1)
            {
                int v = sta[top--];
                in_sta[v] = 0;
                scc[v] = num;
                scc_num[num]++;
                if (u == v)
                    break;
            }
        }
    }
};

class tar
{
    vector<int> edge[N];
    int dfn[N], low[N], idx;
    int n, m;
    int sta[N], top, bcc;
    vector<int> ans[N];

    void tarjan1(int u, int fa)
    {
        dfn[u] = low[u] = ++idx;
        sta[++top] = u;
        int children = 0;
        for (auto v : edge[u])
        {
            if (!dfn[v])
            {
                children++;
                tarjan1(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] <= dfn[u])
                {
                    // 割点
                }
            }
            else if (v != fa)
            {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (u == fa && children >= 2)
        {
            // 是割点
        }
    }

    void tarjan(int u, int fa)
    {
        dfn[u] = low[u] = ++idx;
        sta[++top] = u;
        int children = 0;
        for (auto v : edge[u])
        {
            if (!dfn[v])
            {
                children++;
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u])
                {
                    bcc++;
                    while (sta[top + 1] != v)
                        ans[bcc].push_back(sta[top--]);
                    ans[bcc].push_back(u);
                }
            }
            else if (v != fa)
            {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (fa == u && children == 0)
        {
            ans[++bcc].push_back(u);
        }
    }
};

/***
 * 判断负环
 */
class SPFA {
private:
    vector<array<int,2>> e[N];//v w
    vector<int> dis = vector<int>(N, INF);
    vector<int> cnt = vector<int>(N, 0);
    vector<int> vis = vector<int>(N, 0);
    deque<int> q;
public:
    bool spfa(int n, int s) {
        dis[s] = 0;
        q.clear();
        q.push_back(s);
        vis[s] = 1;
        while (!q.empty()) {
            // ===== swap / LLL 简化优化 =====
            if (q.size() >= 2 && dis[q.front()] > dis[q.back()]) {
                swap(q.front(), q.back());
            }

            int u = q.front();
            q.pop_front();
            vis[u] = 0;

            for (auto [v,w] : e[u]) {
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    cnt[v] = cnt[u] + 1;
                    if (cnt[v] >= n) return false; // 负环

                    if (!vis[v]) {
                        // ===== SLF 优化 =====
                        if (!q.empty() && dis[v] < dis[q.front()])
                            q.push_front(v);
                        else
                            q.push_back(v);
                        vis[v] = 1;
                    }
                }
            }
        }
        return true;
    }
};

/***
 * 欧拉回路：度数全部都是偶数 然后随便一个起点就可以--->欧拉图
 * 欧拉路径：仅存在两个度数为奇数的结点然后这俩一个起点一个终点--->半欧拉图
 * 然后如果要保存路径的话注意是 后序存储
 */
class Eulerian {
private:
    vector<vector<array<int,2>>> e = vector<vector<array<int,2>>>(N);//v,id(边的)
    vector<bool> vis = vector<bool>(N,false);
    vector<int> path;
public:
    void dfs (int u) {
        while(!e[u].empty())
        {
            auto [v,id] = e[u].back();
            e[u].pop_back();
            if(vis[id]) continue;
            vis[id] = true;
            dfs(v);
        }            
        path.push_back(u);
    };
    //所有都弄完记得反转一下path
    // reverse(path.begin(),path.end());
};

/**
 * 长链剖分
 * 查/改 树上某条路径
 * 查/改 子树所有节点
 * LCA
 * 查询两点路径上的第 k 个点
 * 动态维护树上点权或边权
 * 转化到一个区间去维护
 */
struct HLD {
    int n;
    int timer;
    vector<vector<int>> g;

    vector<int> fa;     // fa[u]：u 的父亲
    vector<int> dep;    // dep[u]：u 的深度
    vector<int> siz;    // siz[u]：u 子树大小
    vector<int> son;    // son[u]：u 的重儿子
    vector<int> top;    // top[u]：u 所在重链的链顶
    vector<int> dfn;    // dfn[u]：u 的 DFS 序编号
    vector<int> rnk;    // rnk[i]：DFS 序编号 i 对应的节点

    HLD() {}

    HLD(int n) {
        init(n);
    }

    void init(int n_) {
        n = n_;
        timer = 0;

        g.assign(n + 1, {});

        fa.assign(n + 1, 0);
        dep.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        son.assign(n + 1, 0);
        top.assign(n + 1, 0);
        dfn.assign(n + 1, 0);
        rnk.assign(n + 1, 0);
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs1(int u, int father) {
        fa[u] = father;
        dep[u] = dep[father] + 1;
        siz[u] = 1;
        son[u] = 0;

        for (int v : g[u]) {
            if (v == father) continue;
            dfs1(v, u);
            siz[u] += siz[v];
            if (son[u] == 0 || siz[v] > siz[son[u]]) {
                son[u] = v;
            }
        }
    }

    void dfs2(int u, int topf) {
        top[u] = topf;
        dfn[u] = ++timer;
        rnk[timer] = u;

        // 优先遍历重儿子
        // 这样同一条重链上的节点 DFS 序连续
        if (son[u]) dfs2(son[u], topf);
        
        // 再遍历轻儿子
        for (int v : g[u]) {
            if (v == fa[u] || v == son[u]) continue;
            dfs2(v, v);
        }
    }

    // 建完边后调用
    void work(int root = 1) {
        timer = 0;
        dfs1(root, 0);
        dfs2(root, root);
    }

    int lca(int u, int v) {
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                swap(u, v);
            }
            u = fa[top[u]];
        }

        return dep[u] < dep[v] ? u : v;
    }

    // 求树上两点距离，边数
    int dist(int u, int v) {
        int p = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[p];
    }

    // 判断 u 是否是 v 的祖先
    bool isAncestor(int u, int v) {
        return dfn[u] <= dfn[v] && dfn[v] <= dfn[u] + siz[u] - 1;
    }

    // 返回 u 的子树在线性序列中的区间，闭区间 [l, r]
    pair<int, int> subtreeRange(int u) {
        return {dfn[u], dfn[u] + siz[u] - 1};
    }

    // 从 u 往上跳 k 步
    // k = 0 返回 u 本身
    // 如果不存在，返回 0
    int jump(int u, int k) {
        if (dep[u] <= k) {
            return 0;
        }

        int targetDepth = dep[u] - k;

        while (dep[top[u]] > targetDepth) {
            u = fa[top[u]];
        }

        // 此时 targetDepth 在 u 当前所在重链上
        return rnk[dfn[u] - (dep[u] - targetDepth)];
    }

    // 把 u-v 路径拆成若干段 DFS 序区间
    // 返回的每个 pair 是一个闭区间 [l, r]
    //
    // 如果你用线段树维护点权：
    // 对每个 [l, r] 在线段树上查询/修改即可。
    //
    // 注意：
    // 返回的区间不保证按照路径从 u 到 v 的顺序排列。
    // 如果只做 sum/max/min 这类满足交换律的操作，没问题。
    vector<pair<int, int>> getPathSegments(int u, int v) {
        vector<pair<int, int>> segs;

        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) {
                swap(u, v);
            }

            segs.push_back({dfn[top[u]], dfn[u]});
            u = fa[top[u]];
        }

        if (dep[u] > dep[v]) {
            swap(u, v);
        }

        segs.push_back({dfn[u], dfn[v]});

        return segs;
    }
};

//////下边是没学会的。。。。。

/***
 * 二分图最大匹配   E*sqrt(V)
 * 最大匹配 == 最小点覆盖
 * 最大独立集 == |V| - 最小点覆盖
 */
struct HopcroftKarp {
    int nL, nR;                 // 左部点数、右部点数
    vector<vector<int>> e;      // e[u] : 左点 u 能连到哪些右点
    vector<int> dist;           // BFS 分层时，左点的层数
    vector<int> matchL, matchR; // matchL[u] = u 匹配到的右点；matchR[v] = v 匹配到的左点

    HopcroftKarp(int _nL, int _nR) {
        nL = _nL;
        nR = _nR;
        e.assign(nL + 1, {});
        dist.assign(nL + 1, 0);
        matchL.assign(nL + 1, -1);
        matchR.assign(nR + 1, -1);
    }

    void addEdge(int u, int v) {
        e[u].push_back(v);
    }

    bool bfs(vector<int>& leftNodes) {
        queue<int> q;
        for(int u : leftNodes) {
            if(matchL[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = -1;
            }
        }
        bool found = false;
        while(q.size()) {
            int u = q.front();
            q.pop();
            for(int v : e[u]) {
                int u2 = matchR[v];
                if(u2 == -1) {
                    found = true;
                } else if(dist[u2] == -1) {
                    dist[u2] = dist[u] + 1;
                    q.push(u2);
                }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : e[u]) {
            int u2 = matchR[v];

            // 情况1：v 没匹配，直接可以形成增广路终点
            // 情况2：v 已匹配给 u2，但 u2 正好在下一层，并且它后面还能继续增广
            if (u2 == -1 || (dist[u2] == dist[u] + 1 && dfs(u2))) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }

        // 这个点在当前层次图里走不通，剪枝
        dist[u] = -1;
        return false;
    }
    //最大匹配
    int maxMatching(vector<int>& leftNodes) {
        int matching = 0;
        while (bfs(leftNodes))
            for (int u : leftNodes)
                if (matchL[u] == -1 && dfs(u))
                    matching++;
        return matching;
    }
};


/**
 * 支配树 Dominator Tree
 *
 * 适用场景：
 *   有向图 + 固定起点 root
 *   判断从 root 到 v 的所有路径是否必须经过 u
 *
 * 核心性质：
 *   支配树上 u 是 v 的祖先
 *   <=>
 *   原图中从 root 到 v 的所有路径都必须经过 u
 *
 * 常用含义：
 *   祖先 = 必经点
 *   子树 = 被它支配/控制的点
 *   LCA  = 最近共同必经点
 *   删除点 x 的影响范围 = 支配树上 x 的子树
 *
 * 复杂度：
 *   Lengauer-Tarjan 建支配树：O((n + m) α(n))，通常认为接近 O(n + m)
 *
 * 注意：
 *   1. 只处理从 root 可达的点。
 *   2. 不可达点 dfn[u] = 0，idom[u] 通常为 0。
 *   3. 支配树的边 idom[v] -> v 不一定是原图中的边。
 */

struct DominatorTree {
    int n;
    int timer;

    // 原图和反图
    vector<vector<int>> *g, *rg;

    /**
     * dfn[u]：
     *   u 在 DFS 树中的访问编号。
     *   dfn[u] = 0 表示从 root 不可达。
     *
     * rev[i]：
     *   DFS 序为 i 的点是谁。
     *
     * fa[u]：
     *   DFS 树上 u 的父亲。
     */
    vector<int> dfn, rev, fa;

    /**
     * semi[u]：
     *   u 的半支配点的 DFS 序。
     *   注意 semi[u] 存的是 DFS 序，不是点编号。
     *
     * idom[u]：
     *   u 在支配树上的父亲，也就是 u 的直接支配点。
     *   build 后，idom[root] = 0。
     *
     * anc[u]：
     *   并查集父亲，用于 Lengauer-Tarjan 算法。
     *
     * label[u]：
     *   并查集路径上 semi 最小的点。
     */
    vector<int> semi, idom, anc, label;

    /**
     * bucket[x]：
     *   暂存 semi[v] == dfn[x] 的点 v。
     */
    vector<vector<int>> bucket;

    /**
     * tmp：
     *   eval 里面做非递归路径压缩用的临时数组。
     */
    vector<int> tmp;

    /**
     * 可选部分：
     *   domTree 是最后建出来的支配树。
     *   如果只需要 idom，可以不用 buildTree()。
     */
    vector<vector<int>> domTree;

    /**
     * 可选部分：
     *   tin/tout/sz 用于判断祖先和统计子树大小。
     *   如果不用 dominate() / subtree_size()，可以删掉。
     */
    vector<int> tin, tout, sz;
    int dfsTimer;

    /**
     * 可选部分：
     *   LCA 倍增数组。
     *   如果不用最近共同必经点，可以删掉 LOG 和 up。
     */
    int LOG;
    vector<vector<int>> up;
    vector<int> depth;

    DominatorTree(int n, vector<vector<int>> &g, vector<vector<int>> &rg)
        : n(n), g(&g), rg(&rg) {

        timer = 0;

        dfn.assign(n + 1, 0);
        rev.assign(n + 1, 0);
        fa.assign(n + 1, 0);

        semi.assign(n + 1, 0);
        idom.assign(n + 1, 0);
        anc.assign(n + 1, 0);
        label.assign(n + 1, 0);

        bucket.assign(n + 1, {});
        tmp.reserve(n + 1);

        domTree.assign(n + 1, {});

        tin.assign(n + 1, 0);
        tout.assign(n + 1, 0);
        sz.assign(n + 1, 0);
        dfsTimer = 0;

        LOG = 1;
        while ((1 << LOG) <= n) LOG++;
        up.assign(LOG, vector<int>(n + 1, 0));
        depth.assign(n + 1, 0);
    }

    /**
     * 第一步：
     *   从 root 出发建 DFS 树，并给可达点编号。
     *
     * 复杂度：
     *   O(n + m)
     */
    void dfs_order(int root) {
        vector<int> it(n + 1, 0);
        vector<int> st;

        st.push_back(root);

        dfn[root] = ++timer;
        rev[timer] = root;
        semi[root] = timer;
        label[root] = root;

        while (!st.empty()) {
            int u = st.back();

            if (it[u] == (int)(*g)[u].size()) {
                st.pop_back();
                continue;
            }

            int v = (*g)[u][it[u]++];

            if (!dfn[v]) {
                fa[v] = u;

                dfn[v] = ++timer;
                rev[timer] = v;
                semi[v] = timer;
                label[v] = v;

                st.push_back(v);
            }
        }
    }

    /**
     * eval(v)：
     *   返回从 v 往上走的并查集路径中，semi 最小的那个点。
     *
     * 这是 Lengauer-Tarjan 的核心辅助函数。
     *
     * 复杂度：
     *   均摊接近 O(α(n))
     */
    int eval(int v) {
        if (!anc[v]) return label[v];

        tmp.clear();

        int x = v;

        // 找到接近根的位置，同时记录路径
        while (anc[anc[x]]) {
            tmp.push_back(x);
            x = anc[x];
        }

        // 倒着路径压缩，并维护 label
        for (int i = (int)tmp.size() - 1; i >= 0; i--) {
            int y = tmp[i];

            if (semi[label[anc[y]]] < semi[label[y]]) {
                label[y] = label[anc[y]];
            }

            anc[y] = anc[anc[y]];
        }

        return label[v];
    }

    /**
     * build(root)：
     *   建出支配树的父亲数组 idom。
     *
     * 返回：
     *   idom[v] = v 的直接支配点
     *
     * 复杂度：
     *   O((n + m) α(n))，通常可以看作 O(n + m)
     */
    vector<int> build(int root = 1) {
        dfs_order(root);

        /**
         * 按 DFS 序倒序处理。
         * i = timer ... 2
         * root 不需要处理，所以到 2 停。
         */
        for (int i = timer; i >= 2; i--) {
            int w = rev[i];

            /**
             * 遍历所有原图中指向 w 的边 v -> w。
             * 用这些前驱来更新 semi[w]。
             */
            for (int v : (*rg)[w]) {
                if (!dfn[v]) continue; // 不可达点忽略

                int u = eval(v);

                if (semi[u] < semi[w]) {
                    semi[w] = semi[u];
                }
            }

            /**
             * semi[w] 存的是 DFS 序。
             * rev[semi[w]] 才是真正的点编号。
             */
            bucket[rev[semi[w]]].push_back(w);

            /**
             * link：
             *   把 w 挂到 DFS 树父亲 fa[w] 下面。
             */
            anc[w] = fa[w];

            int p = fa[w];

            /**
             * 处理 bucket[p]。
             * 这些点 v 的半支配点是 p，现在尝试确定它们的 idom。
             */
            for (int v : bucket[p]) {
                int u = eval(v);

                if (semi[u] < semi[v]) {
                    idom[v] = u;
                } else {
                    idom[v] = p;
                }
            }

            bucket[p].clear();
        }

        /**
         * 修正 idom。
         * 前面得到的 idom 可能还不是最终答案。
         */
        for (int i = 2; i <= timer; i++) {
            int w = rev[i];

            if (idom[w] != rev[semi[w]]) {
                idom[w] = idom[idom[w]];
            }
        }

        idom[root] = 0;

        return idom;
    }

    /**
     * 可选操作 1：
     *   根据 idom 建出支配树邻接表。
     *
     * 用途：
     *   后面求祖先关系、子树大小、LCA 都需要它。
     *
     * 复杂度：
     *   O(n)
     *
     * 如果你只需要 idom 数组，可以删掉这个函数和 domTree。
     */
    void buildTree(int root = 1) {
        for (int i = 1; i <= n; i++) {
            domTree[i].clear();
        }

        for (int v = 1; v <= n; v++) {
            if (v == root) continue;

            // dfn[v] == 0 表示从 root 不可达
            if (!dfn[v]) continue;

            if (idom[v]) {
                domTree[idom[v]].push_back(v);
            }
        }
    }

    /**
     * 可选操作 2：
     *   对支配树求 DFS 序和子树大小。
     *
     * 用途：
     *   1. 判断 u 是否支配 v
     *   2. 统计删除某点后影响多少点
     *
     * 复杂度：
     *   O(n)
     *
     * 如果不用 dominate() 和 subtree_size()，可以删掉。
     */
    void dfsDomTree(int root = 1) {
        dfsTimer = 0;

        vector<pair<int, int>> st;
        st.push_back({root, 0});

        tin[root] = ++dfsTimer;
        sz[root] = 1;

        while (!st.empty()) {
            int u = st.back().first;
            int &idx = st.back().second;

            if (idx == (int)domTree[u].size()) {
                tout[u] = dfsTimer;
                st.pop_back();

                if (!st.empty()) {
                    int p = st.back().first;
                    sz[p] += sz[u];
                }

                continue;
            }

            int v = domTree[u][idx++];

            tin[v] = ++dfsTimer;
            sz[v] = 1;

            st.push_back({v, 0});
        }
    }

    /**
     * 可选操作 3：
     *   判断 u 是否支配 v。
     *
     * 含义：
     *   true 表示从 root 到 v 的所有路径都经过 u。
     *
     * 前置条件：
     *   需要先调用：
     *      build(root)
     *      buildTree(root)
     *      dfsDomTree(root)
     *
     * 单次复杂度：
     *   O(1)
     *
     * 如果不用判断祖先，可以删掉。
     */
    bool dominate(int u, int v) {
        if (!dfn[u] || !dfn[v]) return false;
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    }

    /**
     * 可选操作 4：
     *   返回支配树上 u 的子树大小。
     *
     * 含义：
     *   u 支配多少个点，包含 u 自己。
     *
     * 删除 u 后，从 root 不可达的点数量通常就是 sz[u]。
     * 如果题目不算 u 自己，则是 sz[u] - 1。
     *
     * 前置条件：
     *   需要先调用：
     *      build(root)
     *      buildTree(root)
     *      dfsDomTree(root)
     *
     * 单次复杂度：
     *   O(1)
     *
     * 如果不用统计删点影响，可以删掉。
     */
    int subtree_size(int u) {
        if (!dfn[u]) return 0;
        return sz[u];
    }

    /**
     * 可选操作 5：
     *   LCA 预处理。
     *
     * 用途：
     *   查询两个点的最近共同必经点。
     *
     * 前置条件：
     *   需要先调用：
     *      build(root)
     *      buildTree(root)
     *
     * 复杂度：
     *   预处理 O(n log n)
     *
     * 如果不用 LCA，可以删掉：
     *   LOG, up, depth, buildLCA(), lca()
     */
    void buildLCA(int root = 1) {
        for (int j = 0; j < LOG; j++) {
            fill(up[j].begin(), up[j].end(), 0);
        }

        fill(depth.begin(), depth.end(), 0);

        vector<int> st;
        st.push_back(root);

        up[0][root] = 0;
        depth[root] = 0;

        while (!st.empty()) {
            int u = st.back();
            st.pop_back();

            for (int j = 1; j < LOG; j++) {
                up[j][u] = up[j - 1][up[j - 1][u]];
            }

            for (int v : domTree[u]) {
                up[0][v] = u;
                depth[v] = depth[u] + 1;
                st.push_back(v);
            }
        }
    }

    /**
     * 可选操作 6：
     *   查询支配树上 u 和 v 的 LCA。
     *
     * 含义：
     *   lca(u, v) 是从 root 到 u 和从 root 到 v 的最近共同必经点。
     *
     * 前置条件：
     *   需要先调用 buildLCA(root)
     *
     * 单次复杂度：
     *   O(log n)
     *
     * 如果不用最近共同必经点，可以删掉。
     */
    int lca(int u, int v) {
        if (!dfn[u] || !dfn[v]) return 0;

        if (depth[u] < depth[v]) swap(u, v);

        int diff = depth[u] - depth[v];

        for (int j = 0; j < LOG; j++) {
            if (diff >> j & 1) {
                u = up[j][u];
            }
        }

        if (u == v) return u;

        for (int j = LOG - 1; j >= 0; j--) {
            if (up[j][u] != up[j][v]) {
                u = up[j][u];
                v = up[j][v];
            }
        }

        return up[0][u];
    }
};