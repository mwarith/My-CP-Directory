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

    void add(int x, int y, int64_t val) {
        ++x, ++y;
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                tree[i][j] += val;
            }
        }
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

    void update(int x, int y, int64_t val) {
        add(x, y, val - query(x, y, x, y));
    }
};

/*************************** Fenwick Tree ********************** */
template <typename T>
class fenwick_tree {
    vector<T> tree;

    void add(int idx, T val) {
        ++idx;
        while (idx < tree.size()) {
            tree[idx] += val;
            idx += (idx & -idx);
        }
    }

    T get(int idx) {
        ++idx;
        T ret = 0;
        while (idx) {
            ret += tree[idx];
            idx -= (idx & -idx);
        }
        return ret;
    }

public:
    fenwick_tree(int n) {
        tree.assign(n + 1, 0);
    }

    void update(int idx, T val) {
        add(idx, val - query(idx, idx));
    }

    T query(int l, int r) {
        return get(r) - get(l - 1);
    }
};

/*************************** SQRT Decomposition ********************** */
template <class T>
class sqrt_deco {
private:
    vector<T> v, block;
    int n, SQ;

public:
    sqrt_deco(const vector<T> &v) {
        n = v.size();
        SQ = (int)sqrt(n) + 1;
        this->v = v;
        block.resize(SQ);
        for (int i = 0; i < n; ++i) {
            block[i / SQ] += v[i];
        }
    }

    void update(int i, T val) {
        int block_id = i / SQ;
        block[block_id] -= v[i];
        v[i] = val;
        block[block_id] += v[i];
    }

    T query(int l, int r) {
        T sum = 0;
        int start = l / SQ, end = r / SQ;
        if (start == end) {
            for (int i = l; i <= r; i++)
                sum += v[i];
        } else {
            for (int i = l; i < (start + 1) * SQ; i++)
                sum += v[i];
            for (int i = start + 1; i < end; i++)
                sum += block[i];
            for (int i = end * SQ; i <= r; i++)
                sum += v[i];
        }
        return sum;
    }
};

/*************************** Sparse Table ********************** */
template <typename T>
class sparse_table {
    vector<vector<T>> dp;
    function<T(T &, T &)> merge;

public:
    sparse_table(const vector<T> &v, const auto &_merge)
        : merge(_merge) {
        int n = v.size(), LOG = __lg(n) + 1;
        dp = vector(n, vector<T>(LOG, T{}));
        for (int i = 0; i < n; ++i) {
            dp[i][0] = v[i];
        }

        for (int bit = 1; bit < LOG; ++bit) {
            for (int i = 0; i + (1 << bit) <= n; ++i) {
                dp[i][bit] = merge(dp[i][bit - 1],
                                   dp[i + (1 << (bit - 1))][bit - 1]);
            }
        }
    }

    T query(int l, int r) {
        int x = __lg(r - l + 1);
        return merge(dp[l][x], dp[r - (1 << x) + 1][x]);
    }
};

/*************************** MO's Algorithm ********************** */

const int N = 2e5 + 5;
class MO {
    static inline int64_t hilbertOrder(int x, int y, int pow, int rotate) {
        if (pow == 0) {
            return 0;
        }
        int hpow = 1 << (pow - 1);
        int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
        seg = (seg + rotate) & 3;
        const int rotateDelta[4] = {3, 0, 0, 1};
        int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
        int nrot = (rotate + rotateDelta[seg]) & 3;
        int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
        int64_t ans = seg * subSquareSize;
        int64_t add = hilbertOrder(nx, ny, pow - 1, nrot);
        ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
        return ans;
    }

    struct query {
        int l, r, id;
        int64_t ord;
        inline void calcOrder() {
            ord = hilbertOrder(l, r, 21, 0);
        }
    };

    int q, SQ, res = 0;
    vector<int> v, ans;
    vector<query> queries;

    int freq[N]{};

    void add(int id) {
    }
    void remove(int id) {
    }

    int calc() {
        return res;
    }

public:
    MO(const vector<int> &_v, int _q) : q(_q), v(_v) {
        SQ = sqrt((int)v.size()) + 1;
        ans.assign(q, 0);
    }

    void add_query(int l, int r) {
        queries.emplace_back(l, r, (int)queries.size());
    }

    void do_work() {
        for (auto &it : queries)
            it.calcOrder();

        sort(queries.begin(), queries.end(), [&](query a, query b) {
            // return pair{a.l / SQ, a.r} < pair{b.l / SQ, b.r};
            return a.ord < b.ord;
        });

        int l = 0, r = -1;
        for (auto &[lq, rq, idx, ord] : queries) {
            while (l > lq) {
                add(--l);
            }
            while (r < rq) {
                add(++r);
            }
            while (l < lq) {
                remove(l++);
            }
            while (r > rq) {
                remove(r--);
            }
            ans[idx] = calc();
        }

        for (auto it : ans)
            cout << it << '\n';
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
    }
}