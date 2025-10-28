#include "bits/stdc++.h"
using namespace std;

template <class node>
class merge_segment_tree {
private:
    int size;
    vector<node> seg;

    template <class T>
    void build(int x, int lx, int rx, const vector<T> &nums) {
        if (lx == rx) {
            seg[x].apply(nums[lx]);
            return;
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        build(left, lx, mid, nums);
        build(right, mid + 1, rx, nums);
        seg[x].merge(seg[left], seg[right]);
    }

    node query(int x, int lx, int rx, int l, int r) {
        if (l <= lx && rx <= r) {
            return seg[x];
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
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

    int find_greater(int x, int lx, int rx, int l, int r, int val) {
        if (l <= lx && rx <= r) {
            auto &vec = seg[x].vec;
            int id = upper_bound(vec.begin(), vec.end(), val) - vec.begin();
            return (int)vec.size() - id;
        }
        int mid = lx + rx >> 1;
        int left = x + 1;
        int right = x + 2 * (mid - lx + 1);
        if (r < mid + 1) {
            return find_greater(left, lx, mid, l, r, val);
        }
        if (mid < l) {
            return find_greater(right, mid + 1, rx, l, r, val);
        }
        return find_greater(left, lx, mid, l, r, val) +
               find_greater(right, mid + 1, rx, l, r, val);
    }

public:
    merge_segment_tree(int n) {
        size = n;
        seg.resize(2 * size - 1);
    }

    template <class T>
    merge_segment_tree(const vector<T> &nums) {
        size = nums.size();
        seg.resize(2 * size - 1);
        build(0, 0, size - 1, nums);
    }

    node query(int l, int r) {
        return query(0, 0, size - 1, l, r);
    }

    int find_greater(int l, int r, int val) {
        return find_greater(0, 0, size - 1, l, r, val);
    }
};

struct node {
    vector<int> vec;

    void apply(int x) {
        vec = {x};
    }

    void merge(const node &x, const node &y) {
        const auto &a = x.vec, &b = y.vec;
        vec.resize(a.size() + b.size());
        int i = 0, j = 0, k = 0;
        while (i < a.size() && j < b.size()) {
            vec[k++] = (a[i] < b[j] ? a[i++] : b[j++]);
        }
        while (i < a.size()) {
            vec[k++] = a[i++];
        }
        while (j < b.size()) {
            vec[k++] = b[j++];
        }
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
    // cin >> _;
    for (int i = 1; i <= T; i++) {
        // cout << "Case #" << i << ":\n";
        ac();
    }
}