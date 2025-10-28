#include "bits/stdc++.h"
using namespace std;

/****************************** Suffix Array ****************************** */

class suffix_array {
private:
    vector<vector<int64_t>> dp;
    void build_sparse() {
        if (dp.size()) {
            return;
        }
        int n = lcp_.size(), lg = __lg(n);
        dp.resize(lg + 1, vector<int64_t>(n));

        for (int i = 0; i < n; ++i) {
            dp[0][i] = lcp_[i];
        }

        for (int j = 1; j <= lg; j++) {
            for (int i = 0; i + (1 << (j - 1)) < n; i++) {
                dp[j][i] = min(dp[j - 1][i], dp[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    int64_t query(int l, int r) {
        int lg = 32 - __builtin_clz(r - l + 1) - 1;
        return min(dp[lg][l], dp[lg][r - (1 << lg) + 1]);
    }

public:
    vector<int> suf, group, lcp_;
    suffix_array(string s) {
        int n = s.size();
        s += ' ';
        suf = group = vector<int>(n + 1);
        iota(suf.begin(), suf.end(), 0);
        sort(suf.begin(), suf.end(), [&](int i, int j) {
            return s[i] < s[j];
        });
        for (int i = 1; i <= n; ++i) {
            group[suf[i]] = group[suf[i - 1]] + (s[suf[i]] != s[suf[i - 1]]);
        }

        vector<int> cnt, arr, ptr, newG;
        int k = 1;
        auto sort = [&](int x) {
            cnt = arr = ptr = cnt = vector<int>(n + 1);
            for (int i = 0; i <= n; ++i) {
                cnt[group[(suf[i] + x) % (n + 1)]]++;
            }

            int sum = 0;
            for (int i = 0; i <= n; ++i) {
                ptr[i] = sum;
                sum += cnt[i];
            }

            for (int i = 0; i <= n; ++i) {
                arr[ptr[group[(suf[i] + x) % (n + 1)]]++] = suf[i];
            }
            suf = arr;
        };

        while (group[suf[n]] != n) {
            sort(k);
            sort(0);
            newG = group;
            for (int i = 1; i <= n; ++i) {
                newG[suf[i]] = newG[suf[i - 1]] + (group[suf[i]] != group[suf[i - 1]] || group[suf[i] + k] != group[suf[i - 1] + k]);
            }
            group = newG;
            k <<= 1;
        }

        k = 0;
        lcp_ = vector<int>(n);
        for (int i = 0; i < n; ++i) {
            int pos = group[i], j = suf[pos - 1];
            while (s[i + k] == s[j + k]) {
                ++k;
            }
            lcp_[pos - 1] = k;
            k = max(k - 1, 0);
        }
    }

    int lcp(int i, int j) {
        i = group[i], j = group[j];
        if (i > j) {
            swap(i, j);
        }
        if (i == j) {
            return (int)lcp_.size() - suf[i];
        }
        if (i + 1 == j) {
            return lcp_[i];
        }
        build_sparse();
        return query(i, j - 1);
    }
};

/****************************** manacher ****************************** */

class manacher {
private:
    vector<int> d;

public:
    manacher(string t) {
        string s = "#";
        for (auto c : t) {
            s += c + string("#");
        }
        int n = s.size(), l = 1, r = 1;
        s = "$" + s + "^";
        vector<int> p(n + 2);
        for (int i = 1; i <= n; i++) {
            p[i] = max(0, min(r - i, p[l + (r - i)]));
            while (s[i - p[i]] == s[i + p[i]]) {
                p[i]++;
            }
            if (i + p[i] > r) {
                l = i - p[i], r = i + p[i];
            }
        }
        d = vector<int>(p.begin() + 1, p.end() - 1);
    }

    int even(int i) {
        return d[2 * i] - 1;
    }

    int odd(int i) {
        return d[2 * i + 1] - 1;
    }

    bool is_palindrome(int l, int r) {
        int len = r - l + 1;
        if (len & 1) {
            return odd((l + r) / 2) >= len;
        } else {
            return even((l + r) / 2 + 1) >= len;
        }
    }
};

/****************************** 2D Hashing ****************************** */

class dr_string2D {
private:
    using i128 = __int128_t;
    static const int64_t MOD = (1LL << 61) - 1;
    static int64_t base, base2;
    vector<int64_t> pow1, pow2;
    vector<vector<int64_t>> hash;
    int n, m;

public:
    dr_string2D(const vector<string> &mat) {
        n = mat.size(), m = mat[0].size();
        pow1.assign(n + 1, 1), pow2.assign(m + 1, 1);
        hash.assign(n + 1, vector<int64_t>(m + 1, 0));

        for (int i = 0; i < n; i++)
            pow1[i + 1] = (i128)pow1[i] * base % MOD;
        for (int i = 0; i < m; i++)
            pow2[i + 1] = (i128)pow2[i] * base2 % MOD;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                hash[i + 1][j + 1] = mat[i][j] - 'a' + 1;
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= m; j++) {
                (hash[i + 1][j] += (i128)hash[i][j] * base % MOD) % MOD;
            }
        }
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j < m; j++) {
                (hash[i][j + 1] += (i128)hash[i][j] * base2 % MOD) % MOD;
            }
        }
    }

    int64_t substr(int x1, int y1, int x2, int y2) { // 1-indexed
        x1--, y1--;
        int dx = x2 - x1, dy = y2 - y1;
        return ((hash[x2][y2] - (i128)hash[x2][y1] * pow2[dy] % MOD + MOD) % MOD -
                (i128)(hash[x1][y2] - (i128)hash[x1][y1] * pow2[dy] % MOD + MOD) % MOD *
                    pow1[dx] % MOD +
                MOD) %
               MOD;
    }
};
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int64_t dr_string2D::base = (MOD >> 2) + rng() % (MOD >> 1);
int64_t dr_string2D::base2 = (MOD >> 2) + rng() % (MOD >> 1);

/****************************** Hashing ****************************** */
class dr_string {
private:
    using i128 = __int128_t;
    static const int64_t MOD = (1LL << 61) - 1;
    static int64_t base;
    vector<int64_t> pow{1}, hash{0};

    int64_t sub(int64_t a, int64_t b) {
        a -= b;
        if (a < 0)
            a += MOD;
        return a;
    }

public:
    dr_string(const string &s) {
        for (int i = 0; i < s.size(); i++) {
            pow.emplace_back((i128)pow[i] * base % MOD);
            hash.emplace_back(((i128)hash[i] * base + s[i]) % MOD);
        }
    }

    int64_t substr(int l, int r) {
        return sub(hash[r + 1], (i128)hash[l] * pow[r - l + 1] % MOD);
    }
};
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int64_t dr_string::base = (MOD >> 2) + rng() % (MOD >> 1);

/****************************** Dynamic Hashing ****************************** */

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

class dr_string {
private:
    using i128 = __int128_t;
    static const int64_t mod = (1LL << 61) - 1;
    static int64_t base;
    static vector<int64_t> pow, pow_sum;

    struct node {
        int len, lazy = -1;
        int64_t hash, hash_rev;

        void apply(int lx, int rx, int64_t val = 0) {
            lazy = val;
            len = rx - lx + 1;
            hash = hash_rev = val * (i128)pow_sum[len] % mod;
        }

        void merge(const node &a, const node &b) {
            len = a.len + b.len;
            hash = (a.hash + (i128)b.hash * pow[a.len]) % mod;
            hash_rev = (b.hash_rev + (i128)a.hash_rev * pow[b.len]) % mod;
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

    segment_tree<node> st = segment_tree<node>(1, 0);

public:
    dr_string(const auto &s) {
        while (pow.size() <= s.size()) {
            pow_sum.emplace_back((pow_sum.back() + pow.back()) % mod);
            pow.emplace_back((i128)pow.back() * base % mod);
        }
        st = segment_tree<node>(vector<int64_t>(s.begin(), s.end()));
    }

    void update(int l, int r, char val) {
        st.update(l, r, val);
    }

    int64_t substr(int l, int r) {
        return st.query(l, r).hash;
    }

    int64_t substr_rev(int l, int r) {
        return st.query(l, r).hash_rev;
    }

    bool is_palindrome(int l, int r) {
        return substr(l, r) == substr_rev(l, r);
    }
};
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
int64_t dr_string::base = (mod >> 2) + rng() % (mod >> 1);
vector<int64_t> dr_string::pow = {1}, dr_string::pow_sum = {0};

/********************************* Z Function ********************************** */

vector<int> z_function(const string &s) {
    int n = s.size();
    vector<int> z(n);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i < r) {
            z[i] = min(r - i, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}

/*************************** Prefix Function (KMP) ********************************* */
vector<int> prefix_function(const string &s) {
    int n = s.size();
    vector<int> pi(n, 0);
    for (int i = 1, j = 0; i < n; ++i) {
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];
        if (s[i] == s[j])
            ++j;
        pi[i] = j;
    }
    return pi;
}

vector<int> kmp(const string &s, const string &pat) {
    int n = s.size();
    auto pi = prefix_function(pat);
    vector<int> ret(n, 0);
    for (int i = 0, j = 0; i < n; ++i) {
        while (j > 0 && s[i] != pat[j])
            j = pi[j - 1];
        if (s[i] == pat[j])
            ++j;
        if (j == (int)pat.size())
            j = pi[j - 1], ret[i - (int)pat.size() + 1] = 1;
    }
    return ret;
}

void ac() {
}

int main() {
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