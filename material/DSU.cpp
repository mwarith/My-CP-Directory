#include "bits/stdc++.h"
using namespace std;

class DSU {
public:
    int cnt;
    vector<int> par, cmp;
    DSU(int n)
        : cnt(n), cmp(n + 1, 1), par(n + 1, 1) {
        iota(par.begin(), par.end(), 0);
    }

    int find(int x) {
        while (x != par[x]) {
            x = par[x] = par[par[x]];
        }
        return x;
    }

    void join(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return;
        }
        if (cmp[x] > cmp[y]) {
            swap(x, y);
        }
        par[x] = y;
        cmp[y] += cmp[x];
        cnt--;
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }
};

void ac() {
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);
    if (fopen("in.txt", "r")) {
        freopen("in.txt", "r", stdin);
        freopen("out.txt", "w", stdout);
    }
    int T = 1;
    // cin >> T;
    for (int i = 1; i <= T; i++) {
        ac();
    }
}