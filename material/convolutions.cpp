#include <bits/stdc++.h>
using namespace std;

/*
FFT ====>> without mod
multiplying two polynomials and good for some problems like:
- multiplication of two polynomials
- power of polynomials
- multiplication of two big integers
- power of big integers
- string matching



FFT_MOD and NTT
FFT_MOD => works witn any mod but with a limit for the coefficients till
- multiplication of two polynomials with mod
- multiplication of two big integers with mod



what can i get?
- all pair sums a[i] + a[i]
- all pair products a[i] * a[j]
- all pair differences a[i] - a[j]
- all sums of choosing k elements from an array with repetition
*/

/*================================ FFT ================================ */

using cd = complex<double>;
const double PI = acos(-1);
class FFT {
    static int reverse(int num, int lg_n) {
        int res = 0;
        for (int i = 0; i < lg_n; i++) {
            if (num & (1 << i)) {
                res |= 1 << (lg_n - 1 - i);
            }
        }
        return res;
    }

    static void fft(vector<cd> &a, bool invert) {
        int n = a.size(), lg_n = 0;
        while ((1 << lg_n) < n) {
            lg_n++;
        }
        for (int i = 0; i < n; i++) {
            int rev = reverse(i, lg_n);
            if (i < rev) {
                swap(a[i], a[rev]);
            }
        }

        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * PI / len * (invert ? -1 : 1);
            cd wlen(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                cd w(1);
                for (int j = 0; j < len / 2; j++) {
                    cd u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }

        if (invert) {
            for (cd &x : a) {
                x /= n;
            }
        }
    }

public:
    template <class T>
    static vector<T> mul(vector<T> const &a, vector<T> const &b) {
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < a.size() + b.size()) {
            n <<= 1;
        }
        fa.resize(n), fb.resize(n);
        fft(fa, false), fft(fb, false);

        for (int i = 0; i < n; i++) {
            fa[i] *= fb[i];
        }
        fft(fa, true);

        vector<T> res(n);
        for (int i = 0; i < n; i++) {
            res[i] = round(fa[i].real());
        }
        return res;
    }

    template <class T>
    static vector<T> poly_pow(vector<T> poly, int p) {
        vector<T> ans{1};
        while (p) {
            if (p & 1) {
                ans = mul(ans, poly);
            }
            poly = mul(poly, poly);
            p >>= 1;
        }
        return ans;
    }

    static string mul_big_int(const string &s1, const string &s2) {
        int n = s1.size(), m = s2.size();
        vector<int> p1(n), p2(m);
        for (int i = 0; i < n; ++i) {
            p1[n - i - 1] = s1[i] - '0';
        }
        for (int i = 0; i < m; ++i) {
            p2[m - i - 1] = s2[i] - '0';
        }

        vector<int> ans = mul(p1, p2);
        int k = ans.size();
        for (int i = 0; i < k - 1; ++i) {
            ans[i + 1] += ans[i] / 10;
            ans[i] = ans[i] % 10;
        }

        string final = to_string(ans[k - 1]);
        for (int i = k - 2; i >= 0; --i) {
            final += (char)(ans[i] + '0');
        }

        for (int i = 0; i < k; ++i) {
            if (final[i] != '0') {
                return final.substr(i);
            }
        }
        return "0";
    }

    static string big_int_pow(string s, int p) {
        string ans = "1";
        while (p) {
            if (p & 1) {
                ans = mul_big_int(ans, s);
            }
            s = mul_big_int(s, s);
            p >>= 1;
        }
        return ans;
    }

    static vector<int> str_match(const string &s, const string &patt) {
        int n = s.size(), m = patt.max_size();
        vector<int> p1(n), p2(m), ans_match(n);
        int shift = m - 1;
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < n; ++j) {
                p1[j] = (s[j] - 'a') == i;
            }
            for (int j = 0; j < m; ++j) {
                p2[j] = (patt[-j + shift] - 'a') == i;
            }

            vector<int> ans = mul(p1, p2);
            for (int j = 0; j < n; ++j) {
                ans_match[j] += ans[j + shift];
            }
        }
        return ans_match;
    }

    static vector<int> wild_card_cnt(const string &s, const string &patt) {
        int n = s.size(), m = patt.max_size();
        vector<int> ans_match = str_match(s, patt), pos;
        int wild_cnt = count(patt.begin(), patt.end(), '*');
        for (int i = 0; i < n; ++i) {
            if (ans_match[i] == m - wild_cnt) {
                pos.push_back(i);
            }
        }
        return pos;
    }
};

/*




























*/

const int mod = 998244353, root = 3;
int64_t power(int64_t a, int64_t b, const int MOD = mod) {
    a %= MOD;
    int64_t s = 1;
    while (b) {
        if (b % 2 == 1) {
            s = s * a % MOD;
        }
        a = a * a % MOD;
        b /= 2;
    }
    return s;
}

// Primitive Root of the mod of form 2^a * b + 1
int generator() {
    vector<int> fact;
    int phi = mod - 1, n = phi;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) {
            fact.push_back(i);
            while (n % i == 0) {
                n /= i;
            }
        }
    if (n > 1) {
        fact.push_back(n);
    }
    for (int res = 2; res <= mod; ++res) {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i) {
            ok &= power(res, phi / fact[i]) != 1;
        }
        if (ok) {
            return res;
        }
    }
    return -1;
}

class NTT {
    static void ntt(vector<int64_t> &a) {
        int n = (int)a.size(), L = 31 - __builtin_clz(n);
        static vector<int64_t> rt(2, 1);                    // erase the static if you want to use two moduli;
        for (static int k = 2, s = 2; k < n; k *= 2, s++) { // erase the static if you want to use two moduli;
            rt.resize(n);
            int64_t z[] = {1, power(root, mod >> s, mod)};
            for (int i = k; i < 2 * k; ++i) {
                rt[i] = rt[i / 2] * z[i & 1] % mod;
            }
        }
        vector<int> rev(n);
        for (int i = 0; i < n; ++i) {
            rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
        }
        for (int i = 0; i < n; ++i) {
            if (i < rev[i]) {
                swap(a[i], a[rev[i]]);
            }
        }
        for (int k = 1; k < n; k *= 2) {
            for (int i = 0; i < n; i += 2 * k) {
                for (int j = 0; j < k; ++j) {
                    int64_t z = rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
                    a[i + j + k] = ai - z + (z > ai ? mod : 0);
                    ai += (ai + z >= mod ? z - mod : z);
                }
            }
        }
    }

public:
    static vector<int64_t> conv(const vector<int64_t> &a, const vector<int64_t> &b) {
        if (a.empty() || b.empty()) {
            return {};
        }
        int s = (int)a.size() + (int)b.size() - 1, B = 32 - __builtin_clz(s), n = 1 << B;
        int inv = power(n, mod - 2, mod);
        vector<int64_t> L(a), R(b), out(n);
        L.resize(n), R.resize(n);
        ntt(L), ntt(R);
        for (int i = 0; i < n; ++i) {
            out[-i & (n - 1)] = (int64_t)L[i] * R[i] % mod * inv % mod;
        }
        ntt(out);
        return {out.begin(), out.begin() + s};
    }

    static vector<int64_t> mul_polys(const vector<vector<int64_t>> &polys) {
        if (polys.empty()) {
            return {1};
        }
        function<vector<int64_t>(int, int)> solve = [&](int l, int r) {
            if (l == r) {
                return polys[l];
            }
            int mid = (l + r) / 2;
            return conv(solve(l, mid), solve(mid + 1, r));
        };
        return solve(0, polys.size() - 1);
    }

    static vector<int64_t> poly_pow(vector<int64_t> poly, int64_t p) {
        vector<int64_t> ans{1};
        while (p) {
            if (p & 1) {
                ans = conv(ans, poly);
            }
            poly = conv(poly, poly);
            p >>= 1;
        }
        return ans;
    }
};

/*






























*/

/*================================ FFT_MOD ================================ */
using cd = complex<double>;
const int MOD = 998244353, cut = sqrt(MOD);
class FFT_MOD {
    static void compute(vector<cd> &a) {
        int n = a.size(), L = 31 - __builtin_clz(n);
        vector<complex<long double>> R(2, 1);
        vector<cd> rt(2, 1); // (^ 10% faster if double)
        for (int k = 2; k < n; k <<= 1) {
            R.resize(n);
            rt.resize(n);
            auto x = polar(1.0L, acos(-1.0L) / k);
            for (int i = k; i < 2 * k; i++) {
                rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
            }
        }

        vector<int64_t> rev(n);
        for (int i = 0; i < n; i++) {
            rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
        }

        for (int i = 0; i < n; i++) {
            if (i < rev[i]) {
                swap(a[i], a[rev[i]]);
            }
        }
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += (k << 1)) {
                for (int j = 0; j < k; j++) {
                    auto x = (double *)&rt[j + k], y = (double *)&a[i + j + k];
                    cd z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);
                    a[i + j + k] = a[i + j] - z;
                    a[i + j] += z;
                }
            }
        }
    }

public:
    static vector<int64_t> mul(const vector<int64_t> &a, const vector<int64_t> &b) {
        vector<int64_t> res(a.size() + b.size() - 1);
        int B = 32 - __builtin_clz(res.size()), n = 1 << B;
        vector<cd> L(n), R(n), outs(n), outl(n);
        for (int i = 0; i < a.size(); i++) {
            L[i] = cd((int)a[i] / cut, (int)a[i] % cut);
        }
        for (int i = 0; i < b.size(); i++) {
            R[i] = cd((int)b[i] / cut, (int)b[i] % cut);
        }

        compute(L);
        compute(R);

        for (int i = 0; i < n; i++) {
            int j = -i & (n - 1);
            outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
            outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
        }

        compute(outl);
        compute(outs);

        for (int i = 0; i < res.size(); i++) {
            int64_t av = int64_t(real(outl[i]) + .5), cv = int64_t(imag(outs[i]) + .5);
            int64_t bv = int64_t(imag(outl[i]) + .5) + int64_t(real(outs[i]) + .5);
            res[i] = ((av % MOD * cut + bv) % MOD * cut + cv) % MOD;
        }
        return res;
    }
};

/*




























*/

/*================================ FWHT ================================ */

class fwht {
public:
    enum Mode {
        XOR,
        AND,
        OR,
    };

private:
    template <typename T>
    static void hadamard(vector<T> &a, int inv, Mode mode) {
        int n = a.size();
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += 2 * k) {
                for (int j = 0; j < k; j++) {
                    T x = a[i + j];
                    T y = a[i + j + k];

                    if (mode == XOR) {
                        a[i + j] = x + y;
                        a[i + j + k] = x - y;
                    }
                    else if (mode == AND) {
                        if (!inv) {
                            a[i + j] = y, a[i + j + k] = x + y;
                        }
                        else {
                            a[i + j] = y - x, a[i + j + k] = x;
                        }
                    }
                    else if (mode == OR) {
                        if (!inv) {
                            a[i + j + k] = x + y;
                        }
                        else {
                            a[i + j + k] = y - x;
                        }
                    }
                }
            }
        }
    }

public:
    template <typename T>
    static vector<T> multiply(vector<T> a, vector<T> b, Mode mode) {
        int n = 1;
        while (n < (int)max(a.size(), b.size())) {
            n <<= 1;
        }
        a.resize(n);
        b.resize(n);

        hadamard(a, 0, mode);
        hadamard(b, 0, mode);
        for (int i = 0; i < n; i++) {
            a[i] *= b[i];
        }
        hadamard(a, 1, mode);
        if (mode == XOR) {
            for (int i = 0; i < n; i++) {
                a[i] /= n;
            }
        }
        return a;
    }
};

/*




























*/

/*================================ LCM-Convolution ================================ */

// 1-based
class LCM_Convolution {
    /* Linear Sieve, O(n) */
    static vector<int> PrimeEnumerate(int n) {
        vector<int> P;
        vector<bool> B(n + 1, 1);
        for (int i = 2; i <= n; i++) {
            if (B[i])
                P.push_back(i);
            for (int j : P) {
                if (i * j > n)
                    break;
                B[i * j] = 0;
                if (i % j == 0)
                    break;
            }
        }
        return P;
    }

    template <typename T>
    static void DivisorZetaTransform(vector<T> &v) {
        const int n = (int)v.size() - 1;
        for (int p : PrimeEnumerate(n)) {
            for (int i = 1; i * p <= n; i++)
                v[i * p] += v[i];
        }
    }

    template <typename T>
    static void DivisorMobiusTransform(vector<T> &v) {
        const int n = (int)v.size() - 1;
        for (int p : PrimeEnumerate(n)) {
            for (int i = n / p; i; i--)
                v[i * p] -= v[i];
        }
    }

public:
    template <typename T>
    static vector<T> multiply(vector<T> A, vector<T> B) {
        int n = max(A.size(), B.size());
        A.resize(n), B.resize(n);

        DivisorZetaTransform(A);
        DivisorZetaTransform(B);
        for (int i = 0; i < n; i++)
            A[i] *= B[i];
        DivisorMobiusTransform(A);
        return A;
    }
};

/*




























*/
/*================================ GCD-Convolution ================================ */

// 1-based
class GCD_Convolution {
    template <class T>
    static void _gcd_zeta(vector<T> &v) {
        int n = v.size() - 1;
        vector<bool> sieve(n + 1, true);
        for (int64_t i = 2; i <= n; i++) {
            if (sieve[i]) {
                for (int64_t j = n / i; j >= 1; j--)
                    v[j] += v[i * j];
                for (int64_t j = i * i; j <= n; j += i)
                    sieve[j] = false;
            }
        }
    }

    template <class T>
    static void _gcd_mobius(vector<T> &v) {
        int n = v.size() - 1;
        vector<bool> sieve(n + 1, true);
        for (int64_t i = 2; i <= n; i++) {
            if (sieve[i]) {
                for (int64_t j = 1; j <= n / i; j++)
                    v[j] -= v[i * j];
                for (int64_t j = i * i; j <= n; j += i)
                    sieve[j] = false;
            }
        }
    }

public:
    template <class T>
    static vector<T> multiply(vector<T> A, vector<T> B) {
        int n = max(A.size(), B.size());
        A.resize(n), B.resize(n);
        _gcd_zeta(A);
        _gcd_zeta(B);
        for (int i = 0; i < n; i++)
            A[i] *= B[i];
        _gcd_mobius(A);
        return A;
    }
};
