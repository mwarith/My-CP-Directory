#include "bits/stdc++.h"
using namespace std;

const int N = 1e5 + 5, mod = 1e9 + 7;
int64_t fact[N], mod_inv[N];
int64_t power(int64_t b, int64_t n, const int MOD = 1e9 + 7) {
    b %= MOD;
    int64_t s = 1;
    while (n) {
        if (n % 2 == 1)
            s = s * b % MOD;
        b = b * b % MOD;
        n /= 2;
    }
    return s % MOD;
}

void pre() {
    fact[0] = 1;
    for (int64_t i = 1; i < N; ++i)
        fact[i] = (fact[i - 1] * i) % mod;
    mod_inv[N - 1] = power(fact[N - 1], mod - 2);
    for (int64_t i = N - 2; i >= 0; --i)
        mod_inv[i] = (i + 1) * mod_inv[i + 1] % mod;
}

int64_t nCr(int64_t n, int64_t r) {
    return fact[n] * mod_inv[n - r] % mod * mod_inv[r] % mod;
}

int64_t nPr(int64_t n, int64_t r) {
    return fact[n] * mod_inv[n - r] % mod;
}

int64_t stars_pars(int64_t things, int64_t positions) {
    return nCr(positions + things - 1, things);
}

int main() {
}