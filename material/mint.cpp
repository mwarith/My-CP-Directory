#include "bits/stdc++.h"
using namespace std;

int mod;
template <typename T = int64_t>
class Modular {
public:
    T val, MOD = mod;

    Modular(T v = 0) {
        val = v % MOD;
        if (val < 0) {
            val += MOD;
        }
    }

    Modular &operator+=(Modular const &b) {
        val += b.val;
        if (val >= MOD) {
            val -= MOD;
        }
        return *this;
    }

    Modular &operator-=(Modular const &b) {
        val -= b.val;
        if (val < 0) {
            val += MOD;
        }
        return *this;
    }

    Modular &operator*=(Modular const &b) {
        val = (__int128_t)val * b.val % MOD;
        return *this;
    }

    friend Modular power(Modular a, T e) {
        Modular res = 1;
        while (e) {
            if (e & 1) {
                res *= a;
            }
            a *= a;
            e >>= 1LL;
        }
        return res;
    }

    friend Modular inverse(Modular a) { return power(a, MOD - 2); }
    Modular &operator/=(Modular const &b) { return *this *= inverse(b); }
    friend Modular operator+(Modular a, Modular const b) { return a += b; }
    friend Modular operator-(Modular a, Modular const b) { return a -= b; }
    friend Modular operator*(Modular a, Modular const b) { return a *= b; }
    friend Modular operator/(Modular a, Modular const b) { return a /= b; }
    friend Modular operator-(Modular const a) { return 0 - a; }
    friend ostream &operator<<(ostream &os, Modular const &a) { return os << a.val; }
    friend bool operator==(Modular const &a, Modular const &b) { return a.val == b.val; }
    friend bool operator!=(Modular const &a, Modular const &b) { return a.val != b.val; }
};

void ac() {
    int64_t n, t;
    cin >> n >> t;
    mod = n;
    while (t--) {
        int64_t a, b;
        char op;
        cin >> a >> op >> b;
        Modular x = a, y = b;
        if (op == '+') {
            cout << x + y << '\n';
        } else if (op == '-') {
            cout << x - y << '\n';
        } else if (op == '8') {
            cout << x * y << '\n';
        } else if (op == '/') {
            cout << x / y << '\n';
        }
    }
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
