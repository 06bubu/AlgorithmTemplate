#include <bits/stdc++.h>
using namespace std;
#define int long long
#define ld long double
#define pll pair<int, int>
#define pdd pair<double, double>
#define tlll tuple<int, int, int>
#define lowbit(x) x & (-x)
const int N = 2e5 + 10;
const int M = 1e3 + 10;
const int mx = 1e18;
const int LOGN = 20;
const int MOD = 987654321;
int LOG2[LOGN];
int n, m, q;

/***
 * 并查集
 * 连通块最值哦
 */
class DSU {
private:
    int n; 
    int components;
    vector<int> pa;//<0表示根  绝对值是大小 >=0是父节点
    vector<vector<int>> comp;
public:
    DSU() : n(0),components(0) {}
    
    DSU(int _n) {
        init(_n);
    }
    
    void init(int x) {
        n = x;
        components = n;
        pa.assign(n,-1);
        comp.assign(n,{});
        for(int i=0;i<n;i++) comp[i].push_back(i);
    }
    
    int find(int x) {
        if(pa[x] < 0) return x;
        return pa[x] = find(pa[x]);
    }
    
    bool same(int u,int v) {
        u = find(u);
        v = find(v);
        return u==v;
    }
    
    int size(int u) {
        return -pa[find(u)];
    }

    int groups_num() const {
        return components;
    }

    int merge(int a,int b) {
        a = find(a);
        b = find(b);
        if(a == b) return a;
        if(-pa[a] < -pa[b]) swap(a,b);
        pa[a] += pa[b];
        pa[b] = a;

        for(int x : comp[b]) comp[a].push_back(x);
        comp[b].clear();
        components--;
        return a;
    }

    const vector<int>& group(int x) {
        return comp[find(x)];
    }
    
    vector<int> roots() {
        vector<int> res;
        for(int i=0;i<n;i++) {
            if(pa[i] < 0) 
                res.push_back(i);
        }
        return res;
    }
    
    vector<vector<int>> all_groups() {
        vector<vector<int>> res;
        for(int i=0;i<n;i++)
            if(pa[i] < 0)
                res.push_back(comp[i]);
        return res;
    }
};

/**
 *  线段树维护区间相加  
 *  区间相乘
 *  区间和
 */ 
struct Tree
{
    struct aa{
        int l, r, sum, tag_add, tag_mul;
    }seg[N*4];
    void pushup(int id)
    {
        seg[id].sum = seg[id<<1].sum + seg[id<<1|1].sum;
    }
    void pushdown(int id)
    {
        int mid = (seg[id].r + seg[id].l) >> 1;
        int l_len = mid - seg[id].l + 1;
        int r_len = seg[id].r - mid;
        if(seg[id].tag_mul != 1) {
            seg[id<<1].tag_add *= seg[id].tag_mul;
            seg[id<<1|1].tag_add *= seg[id].tag_mul;
            seg[id<<1].tag_mul *= seg[id].tag_mul;
            seg[id<<1|1].tag_mul *= seg[id].tag_mul;
            seg[id<<1].sum *= seg[id].tag_mul;
            seg[id<<1|1].sum *= seg[id].tag_mul;
            
            seg[id].tag_mul = 1;
        }
        if(seg[id].tag_add != 0)
        {
            seg[id<<1].sum += l_len * seg[id].tag_add;
            seg[id<<1|1].sum += r_len * seg[id].tag_add;
            seg[id<<1].tag_add += seg[id].tag_add;
            seg[id<<1|1].tag_add += seg[id].tag_add;

            seg[id].tag_add = 0;
        }
    }
    void build(int l,int r,int id)
    {
        seg[id] = {l,r,0,0,1};
        if(l == r) {
            return;
        }
        int mid = (l+r)>>1;
        build(l,mid,id<<1);
        build(mid+1,r,id<<1|1);
        pushup(id);
    }
    void modify_add(int l,int r,int id,int x)
    {
        if(l<=seg[id].l && seg[id].r <= r) {
            seg[id].tag_add += x;
            seg[id].sum += (seg[id].r-seg[id].l+1)*x;
            return;
        }
        pushdown(id);
        int mid = (seg[id].l + seg[id].r)>>1;
        if(l <= mid) modify_add(l,r,id<<1,x);
        if(r >  mid) modify_add(l,r,id<<1|1,x);
        pushup(id);
    }
    void modify_mul(int l,int r,int id,int x)
    {
        if(l<=seg[id].l && seg[id].r <= r) {
            seg[id].tag_add *= x;
            seg[id].tag_mul *= x;
            seg[id].sum *= x;
            return;
        }
        pushdown(id);
        int mid = (seg[id].l + seg[id].r)>>1;
        if(l <= mid) modify_mul(l,r,id<<1,x);
        if(r >  mid) modify_mul(l,r,id<<1|1,x);
        pushup(id);
    }
    int query(int l,int r,int id)
    {
        if(l<=seg[id].l && seg[id].r <= r) {
            return seg[id].sum;
        }
        int res = 0;
        int mid = (seg[id].l + seg[id].r)>>1;
        pushdown(id);
        if(l <= mid) res += query(l,r,id<<1);
        if(r >  mid) res += query(l,r,id<<1|1);
        return res;
    }
} Segment_Tree;

/**
 * 线段树维护区间相加
 * 区间相乘
 * 区间和
 * 取模版
 */
struct Tree
{
    struct aa {
        int l, r, sum, tag_add, tag_mul;
    } seg[N << 2];

    void pushup(int id) {
        seg[id].sum = (seg[id << 1].sum + seg[id << 1 | 1].sum) % MOD;
    }

    void pushdown(int id) {
        int mid = (seg[id].r + seg[id].l) >> 1;
        int l_len = mid - seg[id].l + 1;
        int r_len = seg[id].r - mid;

        if (seg[id].tag_mul != 1) {
            seg[id<<1].tag_add=seg[id<<1].tag_add*seg[id].tag_mul%MOD;
            seg[id<<1|1].tag_add=seg[id<<1|1].tag_add*seg[id].tag_mul%MOD;

            seg[id<<1].tag_mul = seg[id<<1].tag_mul * seg[id].tag_mul % MOD;
            seg[id<<1|1].tag_mul = seg[id << 1 | 1].tag_mul * seg[id].tag_mul % MOD;

            seg[id << 1].sum = seg[id << 1].sum * seg[id].tag_mul % MOD;
            seg[id << 1 | 1].sum = seg[id << 1 | 1].sum * seg[id].tag_mul % MOD;

            seg[id].tag_mul = 1;
        }

        if (seg[id].tag_add != 0) {
            seg[id << 1].sum = (seg[id << 1].sum + l_len * seg[id].tag_add) % MOD;
            seg[id << 1 | 1].sum = (seg[id << 1 | 1].sum + r_len * seg[id].tag_add) % MOD;

            seg[id << 1].tag_add = (seg[id << 1].tag_add + seg[id].tag_add) % MOD;
            seg[id << 1 | 1].tag_add = (seg[id << 1 | 1].tag_add + seg[id].tag_add) % MOD;

            seg[id].tag_add = 0;
        }
    }

    void build(int l, int r, int id) {
        seg[id] = {l, r, 0, 0, 1};
        if (l == r) {
            if (l == 1) seg[id].sum = 1;
            return;
        }
        int mid = (l + r) >> 1;
        build(l, mid, id << 1);
        build(mid + 1, r, id << 1 | 1);
        pushup(id);
    }

    void modify_add(int l, int r, int id, int x) {
        if (l <= seg[id].l && seg[id].r <= r) {
            seg[id].tag_add=(seg[id].tag_add+x) % MOD;
            seg[id].sum=(seg[id].sum+(seg[id].r-seg[id].l+1)*x)%MOD;
            return;
        }
        pushdown(id);
        int mid = (seg[id].l + seg[id].r) >> 1;
        if (l <= mid) modify_add(l, r, id << 1, x);
        if (r > mid) modify_add(l, r, id << 1 | 1, x);
        pushup(id);
    }

    void modify_mul(int l, int r, int id, int x) {
        if (l <= seg[id].l && seg[id].r <= r) {
            seg[id].tag_add = seg[id].tag_add*x%MOD;
            seg[id].tag_mul = seg[id].tag_mul*x%MOD;
            seg[id].sum = seg[id].sum*x%MOD;
            return;
        }
        pushdown(id);
        int mid = (seg[id].l + seg[id].r) >> 1;
        if (l <= mid) modify_mul(l, r, id << 1, x);
        if (r > mid) modify_mul(l, r, id << 1 | 1, x);
        pushup(id);
    }

    int query(int l, int r, int id) {
        if (l > r) return 0;
        if (l <= seg[id].l && seg[id].r <= r) return seg[id].sum%MOD;

        int res=0;
        int mid=(seg[id].l+seg[id].r)>>1;
        pushdown(id);
        if (l <= mid) res=(res+query(l,r,id<<1))%MOD;
        if (r > mid) res=(res+query(l,r,id<<1|1))%MOD;
        return res%MOD;
    }
} Segment_Tree;

/*** 
 * 笛卡尔树
 */
struct Tree {

};

/***
 * Treap
 * 小根堆
 */
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct Treap {
    struct node {
        int l,r;//左右儿子
        int val;//权值
        int key;//随机优先级
        int siz;//子树大小
    }tr[N];
    int root = 0;
    int tot = 0;

    int newNode(int val) {
        ++tot;
        tr[tot].l = tr[tot].r = 0;
        tr[tot].val = val;
        tr[tot].key = rng();
        tr[tot].siz = 1;
        return tot;
    }
    int getSize(int p) {
        return p ? tr[p].siz : 0;
    }
    void pushup(int x) {
        if(!x) return;
        tr[x].siz = getSize(tr[x].l)+getSize(tr[x].r)+1;
    }
    /***
     * 
     * @return 左右树根节点
     */
    pair<int,int> split(int p,int val) {
        if(!p) return {0,0};
        
        if(tr[p].val <= val) {
            auto res = split(tr[p].r,val);
            
            tr[p].r = res.first;
            pushup(p);

            return {p , res.second};
        } else {
            auto res = split(tr[p].l,val);

            tr[p].l = res.second;
            pushup(p);

            return {res.first,p};
        }
    }
    // 合并 x 和 y
    // 前提：x 中所有值 <= y 中所有值
    int merge(int x, int y) {
        if (!x || !y) return x + y;

        if (tr[x].key < tr[y].key) {
            tr[x].r = merge(tr[x].r, y);
            pushup(x);
            return x;
        } else {
            tr[y].l = merge(x, tr[y].l);
            pushup(y);
            return y;
        }
    }
    void insert(int val) {
        auto [x, y] = split(root,val);
        int node = newNode(val);
        root = merge(merge(x, node), y);
    }
    // 删除一个数 val，只删一个
    void erase(int val) {
        // a: <= val
        // c: > val
        auto [a,c] = split(root, val);

        // x: <  val
        // y: == val
        auto [x,y] = split(a, val - 1);

        // y 中全是 val，删除其中一个
        // 删除根节点，然后合并根的左右子树
        if(y) {
            y = merge(tr[y].l,tr[y].r);
        }

        root = merge(merge(x, y), c);
    }

    // 查询 val 的排名：小于 val 的个数 + 1
    int getRank(int val) {
        // x: < val
        // y: >= val
        auto [x,y] = split(root, val - 1);

        int ans = getSize(x) + 1;

        root = merge(x, y);

        return ans;
    }

    // 查询第 k 小
    int getKth(int k) {
        int p = root;

        while (p) {
            int leftSize = getSize(tr[p].l);

            if (k <= leftSize) {
                p = tr[p].l;
            } else if (k == leftSize + 1) {
                return tr[p].val;
            } else {
                k -= leftSize + 1;
                p = tr[p].r;
            }
        }

        return -1;
    }

    // 查询 val 的前驱：小于 val 的最大数
    int getPre(int val) {
        // x: < val
        // y: >= val
        auto [x,y] = split(root, val - 1);

        int p = x;
        while (tr[p].r) p = tr[p].r;

        int ans = tr[p].val;

        root = merge(x, y);

        return ans;
    }

    // 查询 val 的后继：大于 val 的最小数
    int getNxt(int val) {
        // x: <= val
        // y: > val
        auto [x,y] = split(root, val);

        int p = y;
        while (tr[p].l) p = tr[p].l;

        int ans = tr[p].val;

        root = merge(x, y);

        return ans;
    }
};

/***
 * 珂朵莉
 */
struct keDuoLiTree {

};