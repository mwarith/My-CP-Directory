#include "bits/stdc++.h"
using namespace std;

const int N = 1e6 + 5, mod = 1e9 + 7;
template <typename T>
T inverse(T a, T MOD = mod) {
    T u = 0, v = 1;
    while (a != 0) {
        T t = MOD / a;
        MOD -= t * a;
        swap(a, MOD);
        u -= t * v;
        swap(u, v);
    }
    return u;
}

template <const int mod = (int)1e9 + 7>
struct mint {
    // static int mod; // for variable mod
    int64_t value;

    mint(int64_t x = 0) {
        value = x % mod;
        if (value < 0)
            value += mod;
    }

    mint power(int64_t p) {
        mint ret = 1, base = value;
        while (p) {
            if (p & 1)
                ret *= base;
            base *= base;
            p /= 2;
        }
        return ret;
    }

    friend mint operator+(mint a, mint b) { return mint(a.value + b.value); }
    friend mint operator-(mint a, mint b) { return mint(a.value - b.value); }
    friend mint operator*(mint a, mint b) { return mint(a.value * b.value); }
    friend mint operator/(mint a, mint b) { return mint(a.value * b.power(mod - 2)); }
    friend bool operator==(mint a, mint b) { return a.value == b.value; }
    friend bool operator!=(mint a, mint b) { return a.value != b.value; }

    mint &operator+=(mint x) { return *this = *this + x; }
    mint &operator-=(mint x) { return *this = *this - x; }
    mint &operator*=(mint x) { return *this = *this * x; }
    mint &operator/=(mint x) { return *this = *this / x; }

    friend istream &operator>>(istream &in, mint &a) { return in >> a.value, a = a.value, in; }
    friend ostream &operator<<(ostream &out, mint a) { return out << a.value; }
};
// int mint::mod = 2; // for variable mod
using Z = mint<(int)1e9 + 7>;

namespace combinatorics {
    vector<Z> fact = {1, 1}, fact_inv = {1, 1}, inv = {1, 1};
    auto pre = []() {
        fact.reserve(N + 1);
        fact_inv.reserve(N + 1);
        inv.reserve(N + 1);
        for (int i = fact.size(); i <= N; i++) {
            fact.emplace_back(fact[i - 1] * i);
            inv.emplace_back(mod - mod / i * inv[mod % i]);
            fact_inv.emplace_back(fact_inv[i - 1] * inv[i]);
        }
        return 0;
    }();

    Z nCr(int n, int r) {
        return fact[n] * fact_inv[n - r] * fact_inv[r];
    }

    Z nPr(int n, int r) {
        return fact[n] * fact_inv[n - r];
    }

    // nCr = nC(n - r)
    Z stars_pars(int things, int positions) {
        return nCr(positions + things - 1, things);
    }

    Z catalan(int n) {
        return nCr(2 * n, n) * inv[n + 1];
    }
};
using namespace combinatorics;

int main() {
}