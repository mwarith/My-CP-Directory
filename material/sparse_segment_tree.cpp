#include "bits/stdc++.h"
using namespace std;

template <class node, int64_t LX = 0, int64_t RX = (int64_t)1e18>
class sparse_segment_tree {
    vector<node> seg;

    void create_node(int &x) {
        if (x == -1) {
            x = seg.size();
            seg.emplace_back(node());
        }
    }

    template <class T>
    void update(int x, int64_t lx, int64_t rx, int64_t l, int64_t r, T val) {
        if (l <= lx && rx <= r) {
            return seg[x].apply(lx, rx, val);
        }
        create_node(seg[x].left);
        create_node(seg[x].right);
        int mid = lx + rx >> 1;
        seg[x].propagate(lx, rx, mid, seg[seg[x].left], seg[seg[x].right]);
        int left = seg[x].left;
        int right = seg[x].right;
        if (l <= mid) {
            update(left, lx, mid, l, r, val);
        }
        if (mid + 1 <= r) {
            update(right, mid + 1, rx, l, r, val);
        }
        seg[x].merge(seg[left], seg[right]);
    }

    node query(int x, int64_t lx, int64_t rx, int64_t l, int64_t r) {
        if (l <= lx && rx <= r) {
            return seg[x];
        }
        create_node(seg[x].left);
        create_node(seg[x].right);
        int mid = lx + rx >> 1;
        seg[x].propagate(lx, rx, mid, seg[seg[x].left], seg[seg[x].right]);
        int left = seg[x].left;
        int right = seg[x].right;
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
    sparse_segment_tree() {
        seg.resize(1);
    }

    template <class T>
    void update(int64_t l, int64_t r, T val) {
        update(0, LX, RX, l, r, val);
    }

    node query(int64_t l, int64_t r) {
        return query(0, LX, RX, l, r);
    }
};

struct node {
    int64_t sum = 0, lazy = -1;
    int left = -1, right = -1;

    void apply(int64_t lx, int64_t rx, int64_t val = 0) {
        sum = (rx - lx + 1) * val;
        lazy = val;
    }

    void merge(const node &a, const node &b) {
        sum = a.sum + b.sum;
    }

    void propagate(int lx, int rx, int mid, node &left_node, node &right_node) {
        if (~lazy) {
            left_node.apply(lx, mid, lazy);
            right_node.apply(mid + 1, rx, lazy);
            lazy = -1;
        }
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