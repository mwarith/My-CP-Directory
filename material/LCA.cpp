#include "bits/stdc++.h"
using namespace std;

class LCA {
    int LOG;

    vector<int> dep, flat, st;
    vector<vector<int>> anc, dp;

    int merge(int a, int b) {
        if (dep[a] < dep[b]) {
            return a;
        }
        return b;
    }

public:
    LCA(vector<vector<int>> const &g) {
        int n = (int)g.size() - 1;
        LOG = __lg(n) + 1;

        anc = vector(n + 1, vector(LOG + 1, 0));
        st = dep = vector(n + 1, 0);

        function<void(int, int)> build = [&](int u, int p) {
            st[u] = flat.size();
            flat.emplace_back(u);
            for (auto ch : g[u]) {
                if (ch == p) {
                    continue;
                }

                dep[ch] = dep[u] + 1;
                anc[ch][0] = u;

                for (int bit = 1; bit < LOG; ++bit) {
                    anc[ch][bit] = anc[anc[ch][bit - 1]][bit - 1];
                }

                build(ch, u);
                flat.emplace_back(u);
            }
        };
        build(1, 0);

        int f = flat.size();
        dp.assign(f, vector<int>(__lg(f) + 1));
        for (int i = 0; i < (int)flat.size(); ++i) {
            dp[i][0] = flat[i];
        }

        for (int bit = 1; bit <= LOG; ++bit) {
            for (int i = 0; i + (1 << bit) < f; ++i) {
                dp[i][bit] = merge(dp[i][bit - 1],
                                   dp[i + (1 << (bit - 1))][bit - 1]);
            }
        }
    }

    int query(int l, int r) {
        int x = __lg(r - l + 1);
        return merge(dp[l][x], dp[r - (1 << x) + 1][x]);
    }

    int kth_anc(int u, int k) {
        for (int i = 0; i < LOG; ++i) {
            if ((k >> i) & 1) {
                u = anc[u][i];
            }
        }
        return u;
    }

    int lca(int u, int v) {
        if (st[u] > st[v]) {
            swap(u, v);
        }
        int l = st[u], r = st[v];
        return query(l, r);
    }
};

const int64_t inf = 1e18;
template <class T = pair<int64_t, int64_t>>
class LCA_weighted {

    int n, LOG;
    vector<int> dep, par;
    vector<vector<int>> anc;
    vector<vector<T>> cost;
    vector<vector<pair<int, int64_t>>> &g;

    T merge(const T &a, const T &b) {
        return pair{min(a.first, b.first), max(a.second, b.second)};
    }

    T get_cost(int u, int k) {
        T res = {inf, -inf};
        for (int bit = 0; bit < LOG; ++bit) {
            if ((k >> bit) & 1) {
                res = merge(res, cost[u][bit]);
                u = anc[u][bit];
            }
        }
        return res;
    }

public:
    LCA_weighted(int _n, vector<vector<pair<int, int64_t>>> &_g)
        : n(_n), g(_g), LOG(__lg(n) + 1) {
        anc = vector(n + 1, vector(LOG + 1, 0));
        cost = vector(n + 1, vector<T>(LOG + 1, pair{inf, -inf}));
        dep = par = vector(n + 1, 0);

        function<void(int, int)> build = [&](int u, int p) {
            for (const auto &[ch, w] : g[u]) {
                if (ch == p) {
                    continue;
                }
                dep[ch] = dep[u] + 1;
                par[ch] = anc[ch][0] = u;
                cost[ch][0] = {w, w};
                for (int bit = 1; bit < LOG; ++bit) {
                    anc[ch][bit] = anc[anc[ch][bit - 1]][bit - 1];
                    cost[ch][bit] = merge(cost[anc[ch][bit - 1]][bit - 1], cost[ch][bit - 1]);
                }
                build(ch, u);
            }
        };
        build(1, 0);
    }

    int kth_anc(int u, int k) {
        for (int i = 0; i < LOG; ++i) {
            if ((k >> i) & 1) {
                u = anc[u][i];
            }
        }
        return u;
    }

    int lca(int u, int v) {
        if (dep[u] < dep[v]) {
            swap(u, v);
        }
        u = kth_anc(u, dep[u] - dep[v]);
        if (u == v) {
            return u;
        }

        for (int bit = LOG - 1; ~bit; --bit) {
            if (anc[u][bit] != anc[v][bit]) {
                u = anc[u][bit];
                v = anc[v][bit];
            }
        }
        return anc[u][0];
    }

    T query(int u, int v) {
        auto lc = lca(u, v);
        return merge(get_cost(u, dep[u] - dep[lc]), get_cost(v, dep[v] - dep[lc]));
    }
};

void ac() {
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    if (fopen("in.txt", "r")) {
        freopen("in.txt", "r", stdin);
        freopen("out.txt", "w", stdout);
    }
    int T = 1;
    // cin >> T;
    for (int i = 1; i <= T; i++) {
        ac();
        if (i < T) {
            cout << "\n";
        }
    }
}