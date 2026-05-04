#include <bits/stdc++.h>
// #include <bit>
using namespace std;
#define int long long
#define ld long double
#define pll pair<int,int>
// #define i128 __int128
#define endl '\n'
const int N = 1e6 + 10;
const int INF = 1e18;
const int MOD = 1e9+7;


void init() {

}

int n,m,q;
struct Line {
    int b[32];
    Line() {
        memset(b,0,sizeof(b));
    }
    void insert(int x) {
        for(int i=31;i>=0;i--) {
            if(x & (1LL<<i)) {
                if(b[i] == 0) {
                    b[i] = x;
                    return;
                } 
                x ^= b[i];
            }
        }
    }

    int queryMax(int x) {
        for(int i=31;i>=0;i--) {
            if((x^b[i]) > x) {
                x ^= b[i];
            }
        }
        return x;
    }

    void mergeForm(Line& oth) {
        for(int i=31;i>=0;i--) {
            if(oth.b[i]) {
                this -> insert(oth.b[i]);
            }
        }
    }
};
struct DSU {
    vector<int> fa,sz;
    vector<int> xr;//r到fa[x]的异或距离
    vector<Line> lb;
    DSU(int n) {
        fa.resize(n+1);
        sz.assign(n+1,1);
        xr.assign(n+1,0);
        lb.resize(n+1);
        for(int i=1;i<=n;i++) fa[i]=i;
    }
    
    pll find(int x) {
        if(fa[x] == x) {
            return {x,0};
        }
        auto res = find(fa[x]);
        int root = res.first;
        int val  = res.second;

        xr[x] ^= val;
        fa[x] = root;
        return {fa[x],xr[x]};
    }
    void add(int x,int y,int w) {
        auto [rx,dx] = find(x);
        auto [ry,dy] = find(y);

        int cnt = dx^dy^w;
        if(rx == ry) {
            lb[rx].insert(cnt);
            return;
        }

        if(sz[rx] < sz[ry]) swap(rx,ry);
        fa[ry] = rx;
        xr[ry] = dx^dy^w;//ry----y-x-----rx 
        
        sz[rx] += sz[ry];
        lb[rx].mergeForm(lb[ry]);
    }
    int query(int x,int y) {
        auto [rx,dx] = find(x);
        auto [ry,dy] = find(y);

        if (rx != ry) return -1;

        return lb[rx].queryMax(dx^dy);
    }
};


void solve() {
    cin>>n>>m;
    DSU dsu(n);
    for(int i=1,u,v,w;i<=m;i++) {
        cin>>u>>v>>w;
        dsu.add(u,v,w);
    }
    cin>>q;
    while(q--) {
        int t,x,y,z;cin>>t;
        if(t == 1) {
            cin>>x>>y;
            cout << dsu.query(x,y) << endl;
        } else {
            cin>>x>>y>>z;
            dsu.add(x,y,z);
        }
    }
}

/*

*/
signed main()
{
    ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    init();
    int _ = 1;
    // std::cin >> _;
    while (_--)
        solve();
    return 0;
}
/*
注意二分

存在上界 正面情况很多  如果太难果断去思考反向角度
正向不好思考，反过来思考最坏的情况是什么样子？

条件能否排序化？

是否可以转化成f(k)-f(k-1)这种形式，求解前缀和即可


加油！！！！！！！！！！！！
ABC出F 
牛客周赛稳定AK  小白月赛出F  练习赛出E  
div3出F  div2出E
*/
