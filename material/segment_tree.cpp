#include "bits/stdc++.h"
using namespace std;

/*

find: l, r, left_first, F

search range: (from l to r)
0 <= left_first <= 1
F(node): function that returns if node is valid or not
running time: log

///////////////////////////////////////

int ans = find(l, r, 1, valid);

same as:

int lo = l, hi = r, ans = -1;
while (l <= r) {
    int mid = (lo + hi) / 2;
    if (valid(query(l, mid))) {
        hi = mid - 1;
        ans = mid;
    }
    else {
        lo = mid + 1;
    }
}

///////////////////////////////////////////////

int ans = find(l, r, 0, valid);

same as:

int lo = l, hi = r, ans = -1;
while (l <= r) {
    int mid = (lo + hi) / 2;
    if (valid(query(mid, hi))) {
        lo = mid + 1;
        ans = mid;
    }
    else {
        hi = mid - 1;
    }
}


*/

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