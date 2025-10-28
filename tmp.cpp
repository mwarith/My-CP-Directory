#include "bits/stdc++.h"
using namespace std;

class fenwick_tree_2D {
    int n, m;
    vector<vector<int64_t>> tree;

    int64_t get(int x, int y) {
        ++x, ++y;
        int64_t ret = 0;
        for (int i = x; i; i -= i & -i) {
            for (int j = y; j; j -= j & -j) {
                ret += tree[i][j];
            }
        }
        return ret;
    }

public:
    fenwick_tree_2D(int _n, int _m) {
        n = _n;
        m = _m;
        tree.assign(n + 5, vector<int64_t>(m + 5, 0));
    }

    int64_t query(int x, int y, int x2, int y2) {
        return get(x2, y2) - get(x - 1, y2) - get(x2, y - 1) + get(x - 1, y - 1);
    }

    void add(int x, int y, int64_t val) {
        ++x, ++y;
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                tree[i][j] += val;
            }
        }
    }

    void update(int x, int y, int64_t val) {
        add(x, y, val - query(x, y, x, y));
    }
};
set<int> prime = {2, 3, 5, 7};

const int N = 31;
int dp[N][N], g[N][N];
void ac() {
    int n;
    cin >> n;

    fenwick_tree_2D ft(N, N);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> g[i][j];
            ft.add(i, j, !prime.count(g[i][j]));
        }
    }

    memset(dp, -1, sizeof(dp));
    dp[1][1] = 0;
    for (int r = 1; r <= n; ++r) {
        for (int c = 1; c <= n; ++c) {
            set<int> s;
            for (int i = 0; i + r <= n; ++i) {
                for (int j = 0; j + c <= n; ++j) {
                    if (ft.query(i, j, i + r - 1, j + c - 1) == 0) {
                        dp[r][c] = 0;
                    }
                    for (int x = 1; x < r; ++x) {
                        s.emplace(dp[x][c] ^ dp[r - x][c]);
                    }
                    for (int y = 1; y < c; ++y) {
                        s.emplace(dp[r][y] ^ dp[r][c - y]);
                    }
                    int &ret = dp[r][c];
                    while (s.count(ret)) {
                        ++ret;
                    }
                }
            }
        }
    }
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    if (fopen("in.txt", "r")) {
        freopen("in.txt", "r", stdin);
        freopen("out.txt", "w", stdout);
    }
    int T = 1;
    cin >> T;
    while (T--) {
        ac();
        if (T) {
            cout << '\n';
        }
    }
}