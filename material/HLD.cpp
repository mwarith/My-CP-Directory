#include "bits/stdc++.h"
using namespace std;

template <class node>
class segment_tree {
private:
    int size;
    vector<node> seg;

    template <class T>
    void build(int x, int lx, int rx, const vector<T> &nums) {
        if (lx == rx) {
            return seg[x].apply(lx, rx, nums[lx]);
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        build(left, lx, mid, nums);
        build(right, mid + 1, rx, nums);
        seg[x].merge(seg[left], seg[right]);
    }

    template <class T>
    void update(int x, int lx, int rx, int l, int r, T value) {
        if (l <= lx && rx <= r) {
            return seg[x].apply(lx, rx, value);
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        seg[x].propagate(lx, rx, seg[left], seg[right]);
        if (l <= mid) {
            update(left, lx, mid, l, r, value);
        }
        if (mid + 1 <= r) {
            update(right, mid + 1, rx, l, r, value);
        }
        seg[x].merge(seg[left], seg[right]);
    }

    node query(int x, int lx, int rx, int l, int r) {
        if (l <= lx && rx <= r) {
            return seg[x];
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        seg[x].propagate(lx, rx, seg[left], seg[right]);
        if (r < mid + 1) {
            return query(left, lx, mid, l, r);
        }
        if (mid < l) {
            return query(right, mid + 1, rx, l, r);
        }
        node ret;
        ret.merge(query(left, lx, mid, l, r), query(right, mid + 1, rx, l, r));
        return ret;
    }

public:
    template <class T>
    segment_tree(int n, T val) {
        size = n;
        seg.resize(2 * size - 1);
        build(0, 0, size - 1, vector<T>(n, val));
    }

    template <class T>
    segment_tree(const vector<T> &nums) {
        size = nums.size();
        seg.resize(2 * size - 1);
        build(0, 0, size - 1, nums);
    }

    template <class T>
    void update(int l, int r, T value) {
        update(0, 0, size - 1, l, r, value);
    }

    node query(int l, int r) {
        return query(0, 0, size - 1, l, r);
    }
};

struct node {
    int64_t sum, lazy = -1;

    void apply(int lx, int rx, int64_t val = 0) {
        sum = (rx - lx + 1) * val;
        lazy = val;
    }

    void merge(const node &a, const node &b) {
        sum = a.sum + b.sum;
    }

    void propagate(int lx, int rx, node &left, node &right) {
        int mid = lx + rx >> 1;
        if (~lazy) {
            left.apply(lx, mid, lazy);
            right.apply(mid + 1, rx, lazy);
            lazy = -1;
        }
    }
};

template <bool val_on_edge = false>
class HLD {
private:
    int size;
    segment_tree<node> st = segment_tree<node>(1, 0); // any DS
    vector<int> par, sz, dep, heavy, id, head;

public:
    vector<pair<int, int>> get_path(int l, int r) {
        vector<pair<int, int>> left, right;
        while (head[l] != head[r]) {
            if (dep[head[r]] > dep[head[l]]) {
                right.emplace_back(id[head[r]], id[r]);
                r = par[head[r]];
            } else {
                left.emplace_back(id[l], id[head[l]]);
                l = par[head[l]];
            }
        }

        if (!val_on_edge) {
            left.emplace_back(id[l], id[r]);
        } else if (l != r) {
            left.emplace_back(id[l] > id[r] ? make_pair(id[l], id[r] + 1)
                                            : make_pair(id[l] + 1, id[r]));
        }
        left.insert(left.end(), right.rbegin(), right.rend());
        return left;
    }

    HLD(const vector<vector<int>> &g, int root = 1) {
        size = g.size();
        heavy = par = sz = dep = id = head = vector<int>(size);
        st = segment_tree<node>(size, 0);
        function<void(int, int)> build = [&](int u, int p) {
            sz[u] = 1;
            for (auto &ch : g[u]) {
                if (ch == p) {
                    continue;
                }
                dep[ch] = dep[u] + 1;
                par[ch] = u;
                build(ch, u);
                sz[u] += sz[ch];
                if (sz[ch] > sz[heavy[u]]) {
                    heavy[u] = ch;
                }
            }
            return sz[u];
        };

        int timer = 0;
        function<void(int, int)> flatten = [&](int u, int p) {
            id[u] = timer++;
            if (heavy[u]) {
                head[heavy[u]] = head[u];
                flatten(heavy[u], u);
            }

            for (auto &ch : g[u]) {
                if (ch == p || ch == heavy[u]) {
                    continue;
                }
                head[ch] = ch;
                flatten(ch, u);
            }
        };
        dep[root] = 0;
        build(root, -1);

        head[root] = root;
        flatten(root, -1);
    }

    template <class T>
    void update_subtree(int u, const T &val) {
        int L = id[u] + (val_on_edge), R = id[u] + sz[u] - 1;
        if (L <= R) {
            st.update(L, R, val);
        }
    }

    template <class T>
    void update_path(int u, int v, const T &val) {
        for (auto [L, R] : get_path(u, v)) {
            if (L > R) {
                swap(L, R);
            }
            st.update(L, R, val);
        }
    }

    node query_subtree(int u) {
        if (val_on_edge && sz[u] == 1) {
            return node();
        }
        return st.query(id[u] + (val_on_edge), id[u] + sz[u] - 1);
    }

    node query_path(int u, int v) {
        auto path = get_path(u, v);
        auto [l, r] = path[0];
        if (l > r) {
            swap(l, r);
        }
        node ret = st.query(l, r);
        for (int i = 1; i < path.size(); ++i) {
            tie(l, r) = path[i];
            if (l > r) {
                swap(l, r);
            }
            ret.merge(ret, st.query(l, r));
        }
        return ret;
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
