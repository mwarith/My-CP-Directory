#include "bits/stdc++.h"
using namespace std;
const int dx[] = {0, 0, 1, -1, 1, 1, -1, -1};
const int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};

void __attribute__((constructor)) calc();

#ifdef ONLINE_JUDGE
freopen("lamps.in", "r", stdin);
#endif

/**************   bit frequency from 1 to n ****************** */
int64_t bit_freq(int64_t n, int b) {
    int64_t res, pow = (1LL << b);
    int64_t last_interval = (n / pow + 1) / 2;

    res = last_interval * pow;
    if (last_interval > 0) {
        int64_t start = pow * (2 * (last_interval - 1) + 1), end = start + pow - 1;
        if (n >= start && n <= end) {
            res -= end - n;
        }
    }

    return res;
}

/**********************  matching bitset  *************************** */
class matching_bitset {
    static constexpr int N = 1e5 + 5;
    string text;
    bitset<N> mask[26], pat_mask;

public:
    matching_bitset(string &s) : text(s) {
        for (int i = 0; i < text.size(); i++) {
            mask[text[i] - 'a'][i] = 1;
        }
    }

    void SetText(const string &s) {
        text = s;
        for (int i = 0; i < 26; i++) {
            mask[i].reset();
        }
    }

    void Update(int idx, char ch) {
        mask[text[idx] - 'a'][idx] = 0;
        text[idx] = ch;
        mask[text[idx] - 'a'][idx] = 1;
    }

    int count_pattern(string &pat, int l, int r) {
        if (r - l + 1 < pat.size()) {
            return 0;
        }
        pat_mask.set();
        for (int i = 0; i < pat.size(); ++i) {
            pat_mask &= (mask[pat[i] - 'a'] >> i);
        }
        return (pat_mask >> l).count() -
               (pat_mask >> (r - (int)pat.size() + 2)).count();
    }

    vector<int> pos_pattern(string &pattern, int l, int r) {
        count_pattern(pattern, l, r);
        vector<int> pos;
        int i = pat_mask._Find_next(l - 1);
        while (i <= r - pattern.size() + 1) {
            pos.emplace_back(i);
            i = pat_mask._Find_next(i);
        }
        return pos;
    }
};

int log(long long base, long long n) {
    int ret = -1;
    while (n) {
        ret++;
        n /= base;
    }
    return ret;
}

/****** Leetcode *******/
auto const ios = []() {
    cin.tie(nullptr)->sync_with_stdio(false);
    return nullptr;
}();

/*    hashmap   */
struct HASH {
    int64_t operator()(int64_t x) const {
        x ^= (x >> 30), x *= UINT64_C(0xbf58476d1ce4e5b9);
        x ^= (x >> 27), x *= UINT64_C(0x94d049bb133111eb);
        return x ^ (x >> 31);
    }
};

template <typename k, typename v>
struct hash_map {
    unordered_map<k, v, HASH> m;
    k toXor;
    hash_map() {
        toXor = chrono::steady_clock::now().time_since_epoch().count();
        m.reserve(4096);
        m.max_load_factor(0.25);
    }
    v &operator[](const k &key) { return m[key ^ toXor]; }
    auto find(const k &key) { return m.find(key ^ toXor); }
    auto end() { return m.end(); }
};

/********** Big Integer ***********/
namespace math_operations {
    int base = 10;

    string add(string a, string b) {
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        while (a.size() < b.size()) {
            a += '0';
        }
        while (a.size() > b.size()) {
            b += '0';
        }
        string ret((int)a.size(), '0');

        int carry = 0;
        for (int i = 0; i < a.size(); ++i) {
            int res = (a[i] - '0') + (b[i] - '0') + carry;
            ret[i] = (res % base + '0');
            carry = res / base;
        }
        if (carry) {
            ret.push_back('1');
        }
        while (ret.size() > 1 && ret.back() == '0') {
            ret.pop_back();
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }

    string sub(string a, string b) {
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        while (a.size() < b.size()) {
            a += '0';
        }
        while (a.size() > b.size()) {
            b += '0';
        }

        string sign, ret((int)a.size(), '0');
        for (int i = (int)a.size() - 1; ~i; --i) {
            if (b[i] > a[i]) {
                sign = "-";
                swap(a, b);
                break;
            }
            if (b[i] < a[i]) {
                break;
            }
        }

        for (int i = 0; i < a.size(); ++i) {
            ret[i] = (a[i] - b[i] + '0');
            if (a[i] < b[i]) {
                int p = i + 1;
                while (a[p] == '0') {
                    a[p] = base - 1 + '0';
                    ++p;
                }
                --a[p];
                ret[i] += base;
            }
        }
        while (ret.size() > 1 && ret.back() == '0') {
            ret.pop_back();
        }
        reverse(ret.begin(), ret.end());
        return sign + ret;
    }

    string mul(string a, string b) {
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        string ret = "0";
        for (int i = 0; i < b.size(); ++i) {
            int carry = 0;
            string res((int)a.size(), '0');
            for (int j = 0; j < a.size(); ++j) {
                int x = (int)(a[j] - '0') * (b[i] - '0') + carry;
                res[j] = (x % base + '0');
                carry = x / base;
            }
            if (carry) {
                res.push_back(carry + '0');
            }
            reverse(res.begin(), res.end());
            ret = add(ret, res + string(i, '0'));
        }
        return ret;
    }

    string divide(const string &dividend, const string &divisor) {
        string quotient, current;
        int n = dividend.size();
        for (int i = 0; i < n; ++i) {
            current.push_back(dividend[i]);
            current.erase(0, min(current.find_first_not_of('0'), current.size() - 1));

            int count = 0, low = 0, high = base - 1;
            while (low <= high) {
                int mid = (low + high) / 2;
                string product = mul(divisor, to_string(mid));
                if (sub(current, product)[0] != '-') {
                    count = mid;
                    low = mid + 1;
                } else {
                    high = mid - 1;
                }
            }

            current = sub(current, mul(divisor, to_string(count)));
            quotient.push_back(count + '0');
        }

        quotient.erase(0, min(quotient.find_first_not_of('0'), quotient.size() - 1));
        return quotient;
    }

    string modulo(string a, string b) {
        return sub(a, mul(b, divide(a, b)));
    }
}
using namespace math_operations;

// #include "bits/stdc++.h"
// using namespace std;

// void ac() {

// }

// int main() {
//     cin.tie(nullptr)->sync_with_stdio(false);
//     if (fopen("in.txt", "r")) {
//         freopen("in.txt", "r", stdin);
//         freopen("out.txt", "w", stdout);
//     }
//     int T = 1;
//     // cin >> T;
//     for (int i = 1; i <= T; i++) {
//         ac();
//     }
// }

/****** Min PQ ********/
template <class T>
using min_priority_queue = priority_queue<T, vector<T>, greater<>>;

/*********** Rope **********/
#include <ext/rope>
using namespace __gnu_cxx;

/************************** random *********************/
mt19937_64 rng(random_device{}());
int64_t rand(int l, int r) {
    return uniform_int_distribution<int64_t>(l, r)(rng);
}

/*******************/

/*************  Ternary Search   ********* */
bool f(double x) {
    // contents;
    return true;
}
double ternary_search(double l, double r) {
    double eps = 1e-9; // set the error limit here
    while (r - l > eps) {
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;
        double f1 = f(m1); // evaluates the function at m1
        double f2 = f(m2); // evaluates the function at m2
        if (f1 < f2) {
            l = m1;
        } else {
            r = m2;
        }
    }
    return f(l); // return the maximum of f(x) in [l, r]
}

/************************** Ordered Set *****************************/
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
template <class T>
using ordered_set = tree<
    T,
    null_type,
    less<T>,
    rb_tree_tag,
    tree_order_statistics_node_update>;

template <class T>
using ordered_multiset = tree<
    T,
    null_type,
    less_equal<T>,
    rb_tree_tag,
    tree_order_statistics_node_update>;

/*
 * Some Tips Here:
 * order_of_key ==> Gets
 */
template <class T>
void erase(ordered_set<T> &x, T v) {
    int rank = x.order_of_key(v);    // Number of elements that are less than v in t
    auto it = x.find_by_order(rank); // Iterator that points to the (rank+1)th element in t
    x.erase(it);
}

/********* Coordinate Compression */
template <typename T>
vector<T> CC(vector<T> &v) {
    auto real = v;
    sort(real.begin(), real.end());
    real.erase(unique(real.begin(), real.end()), real.end());

    for (auto &it : v) {
        it = lower_bound(real.begin(), real.end(), it) - real.begin();
    }
    return real;
}

/***************/
// LengthOfLIS
int lengthOfLIS(vector<int> &nums) {
    int n = nums.size();
    vector<int> ans;
    ans.push_back(nums[0]);
    for (int i = 1; i < n; i++) {
        if (nums[i] > ans.back()) {
            ans.push_back(nums[i]);
        } else {
            int low = lower_bound(ans.begin(), ans.end(), nums[i]) - ans.begin();
            ans[low] = nums[i];
        }
    }
    return ans.size();
}

/****************** int128 *****************/

__int128 read() {
    __int128 x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-')
            f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}

void print(__int128 x) {
    if (x < 0) {
        putchar('-');
        x = -x;
    }
    if (x > 9)
        print(x / 10);
    putchar(x % 10 + '0');
}

bool cmp(__int128 x, __int128 y) {
    return x > y;
}
