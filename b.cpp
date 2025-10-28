#include "bits/stdc++.h"
using namespace std;

const int N = 1e6, mod = 10000007;
namespace sieve {
    vector<int> spf(int N) {
        vector<int> _primes;
        vector<int> _spf(N + 1, 0);
        iota(_spf.begin(), _spf.end(), 0);
        for (int64_t i = 2; i <= N; i++) {
            if (_spf[i] == i) {
                _primes.emplace_back(i);
            }
            for (auto it : _primes) {
                if (i * it > N) {
                    break;
                }
                _spf[i * it] = min(it, _spf[i]);
                if (i % it == 0) {
                    break;
                }
            }
        }
        return _spf;
    }

    vector<vector<pair<int, int>>> factors_cnt(int N) {
        auto _spf = spf(N);
        vector<vector<pair<int, int>>> fact(N + 1);
        for (int i = 1; i <= N; i++) {
            int cur = i;
            while (cur > 1) {
                int cnt = 0;
                auto p = _spf[cur];
                while (cur % p == 0) {
                    cur /= p;
                    cnt++;
                }
                if (cnt) fact[i].emplace_back(p, cnt);
            }
        }
        return fact;
    }
}
auto fact = sieve::factors_cnt(N);

template <typename T>
T inverse(T a, T MOD = mod) {
    T b = MOD, u = 1, v = 0;
    while (b) {
        T t = a / b;
        a -= t * b;
        swap(a, b);
        u -= t * v;
        swap(u, v);
    }
    if (a != 1) return 0;
    u %= MOD;
    if (u < 0) u += MOD;
    return u;
}

int cnt[N + 5], ans[N + 5];
int64_t sum(int64_t n) {
    return n * (n + 1) / 2;
}

template <class T>
ostream &operator<<(ostream &os, const vector<T> &v) {
    for (const auto &x : v) {
        os << x << ' ';
    }
    cout << '\n';
    return os;
}

template <class T>
ostream &operator<<(ostream &os, const vector<vector<T>> &v) {
    for (const auto &x : v) {
        for (const auto &y : x) {
            os << y << ' ';
        }
        cout << '\n';
    }
    return os;
}

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    if (fopen("in.txt", "r")) {
        freopen("in.txt", "r", stdin);
        freopen("out.txt", "w", stdout);
    }
    ans[1] = 1;
    for (int i = 2; i <= N; i++) {
        for (auto &[p, c] : fact[i]) {
            ans[i] = (__int128_t)ans[i - 1] * inverse(sum(cnt[p] + 1)) % mod;
            cnt[p] += c;
            ans[i] = (__int128_t)ans[i] * sum(cnt[p] + 1) % mod;
        }
    }

    while (true) {
        int n;
        cin >> n;
        if (!n) {
            break;
        }
        cout << ans[n] << '\n';
    }
}
