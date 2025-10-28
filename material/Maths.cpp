#include "bits/stdc++.h"
using namespace std;

/*====================== Primitive Root ============== */
int64_t power(int64_t a, int64_t b, const int MOD) {
    a %= MOD;
    int64_t s = 1;
    while (b) {
        if (b % 2 == 1)
            s = s * a % MOD;
        a = a * a % MOD;
        b /= 2;
    }
    return s;
}

// p * log(p) ^ 6
int primitive_root(int p) {
    if (!is_prime(p)) {
        return -1;
    }
    vector<int> fact;
    int phi = p - 1, n = phi;
    for (int i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            fact.emplace_back(i);
            while (n % i == 0) {
                n /= i;
            }
        }
    }
    if (n > 1) {
        fact.emplace_back(n);
    }

    for (int res = 2; res <= p; ++res) {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i) {
            ok &= power(res, phi / fact[i], p) != 1;
        }
        if (ok) {
            return res;
        }
    }
    return -1;
}

/*
a^x = b (mod m)

x = np - q
n = sqrt(m)
p = [1, n]
q = [0, n]

a^(n * p) = b * a^q (mod m)
*/
int64_t discrete_log(int64_t a, int64_t b, int64_t m) {
    a %= m, b %= m;

    // if 0^0 is undefined
    if (a == 0) {
        return b == 0 ? 1 : -1;
    }

    int64_t k = 1, add = 0, g;
    while ((g = gcd(a, m)) > 1) {
        if (b == k) {
            return add;
        }
        if (b % g) {
            return -1;
        }
        b /= g, m /= g, ++add;
        k = (__int128_t)k * (a / g) % m;
    }

    int64_t n = sqrtl(m) + 1;
    int64_t an = 1; // a^n
    for (int i = 0; i < n; ++i) {
        an = (__int128_t)an * a % m;
    }

    unordered_map<int64_t, int64_t> rhs;
    for (int64_t q = 0, cur = b; q <= n; ++q) {
        rhs[cur] = q;
        cur = (__int128_t)cur * a % m;
    }

    int64_t ans = -1;
    for (int64_t p = 1, cur = k; p <= n; ++p) {
        cur = (__int128_t)cur * an % m; // a^(n * p)
        if (rhs.count(cur)) {
            ans = n * p - rhs[cur] + add; // x = np - q
            break;
        }
    }
    return ans;
}

/** precompute inverses **/
int inv[1000], mod = 1e9 + 7;
void pre_inverse() {
    inv[1] = 1;
    for (int i = 2; i < 1000; ++i) {
        inv[i] = mod - 1LL * (mod / i) * inv[mod % i];
    }
}

/********************** Miller Rabin ***************** */
using u128 = __uint128_t;
using u64 = uint64_t;

u64 rand(u64 l, u64 r) {
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    return uniform_int_distribution<u64>(l, r)(rng);
}

namespace primallity {
    u64 mulmod(u64 a, u64 b, const u64 mod) {
        return (u128)a * b % mod;
    }

    u64 power(u64 base, u64 e, const u64 mod) {
        u64 result = 1;
        base %= mod;
        while (e) {
            if (e & 1) {
                result = mulmod(result, base, mod);
            }
            base = mulmod(base, base, mod);
            e >>= 1;
        }
        return result;
    }

    bool check_composite(u64 n, u64 a, u64 d, int s) {
        u64 x = power(a, d, n);
        if (x == 1 || x == n - 1) {
            return false;
        }
        for (int r = 1; r < s; r++) {
            x = mulmod(x, x, n);
            if (x == n - 1) {
                return false;
            }
        }
        return true;
    };
}

bool is_prime(u64 n, int iter = 5) {
    if (n < 4) {
        return n == 2 || n == 3;
    }
    int s = 0;
    u64 d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    for (int i = 0; i < iter; i++) {
        u64 a = rand(2, n - 2);
        if (primallity::check_composite(n, a, d, s)) {
            return false;
        }
    }
    return true;
}

/************************ Sieve ************************/
/************************ Everything Sieve **************/
const int N = 1e6 + 5, mod = 1e9 + 7;
int phi[N], spf[N];
bitset<N> is_prime;
void sieve_all() {
    is_prime.set();
    is_prime[0] = is_prime[1] = false;
    for (int i = 1; i < N; i++) {
        phi[i] = spf[i] = i;
    }
    for (int i = 2; i < N; i++) {
        if (is_prime[i]) {
            for (int j = i; j < N; j += i) {
                if (i != j) {
                    is_prime[j] = false;
                }
                if (spf[j] == j) {
                    spf[j] = i;
                }
                phi[j] -= phi[j] / i;
            }
        }
    }
}

/*********************** Linear Sieve ***************************/

const int N = 1e6;
vector<int> spf(N + 1), primes;
void linear_sieve() {
    for (int i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.emplace_back(i);
        }
        for (int j = 0; i * primes[j] <= N; ++j) {
            spf[i * primes[j]] = primes[j];
            if (primes[j] == spf[i]) {
                break;
            }
        }
    }
}

/*********************** Normal Sieve ***************************/
const int N = 1e7 + 5;
bitset<N> is_prime;
void sieve() {
    is_prime.set();
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= N / i; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < N; j += i) {
                is_prime[j] = false;
            }
        }
    }
}

/************************ SPF ************************/
// sieve SPF
const int N = 1e7 + 5;
int spf[N];
void sieveSPF() {
    for (int i = 1; i < N; ++i) {
        spf[i] = (i & 1 ? i : 2);
    }

    for (int i = 3; i < N / i; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j < N; j += i) {
                if (spf[j] == j) {
                    spf[j] = i;
                }
            }
        }
    }
}

/************************ Fast Prime Check ************************/
bool fast_prime(int64_t x) {
    bool y = true;
    if (x == 2 || x == 3) {
        y = true;
    } else if (x <= 1 || x % 2 == 0 || x % 3 == 0) {
        y = false;
    } else {
        for (int64_t i = 5; i * i <= x; i += 6) {
            if (x % i == 0 || x % (i + 2) == 0) {
                y = false;
                break;
            }
        }
    }
    return y;
}

/************************ Fast Power ************************/

int64_t power(int64_t b, int64_t n, const int MOD = mod) {
    b %= MOD;
    int64_t s = 1;
    while (n) {
        if (n % 2 == 1) {
            s = s * b % MOD;
        }
        b = b * b % MOD;
        n /= 2;
    }
    return s;
}

/************************ Divisors ************************/
/*
  Number of Divisors
  requires Sieve SPF to get the number of divisors in log(n)
  We can also get it in sqrt(n) as usual.
*/

int64_t number_of_divisors(int64_t n) {
    int64_t ans = 1;
    while (n > 1) {
        int64_t num = 0, p = spf[n];
        while (n % p == 0) {
            n /= p, ++num;
        }
        ans *= (num + 1);
    }
    return ans;
}

int64_t product_of_divisors(int64_t n) {
    int64_t num = number_of_divisors(n);
    return power(n, num / 2) * (num & 1 ? (int64_t)sqrtl(n) : 1);
}

int64_t sum_of_divisors(int64_t n) {
    int64_t ans = 1;
    while (n > 1) {
        int64_t num = 0, p = spf[n];
        while (n % p == 0) {
            n /= p, ++num;
        }
        (ans *= (power(p, num + 1) - 1) * power(p - 1, mod - 2)) %= mod;
    }
    return ans;
}

/************************ Get Divisors from Prime Factorization ************************/
vector<pair<int, int>> fact;
set<int> divisors;

// get prime factorization
vector<pair<int, int>> prime_fact(int64_t n) {
    vector<pair<int, int>> ret;
    for (int i = 2; i <= n / i; i++) {
        int cnt = 0;
        while (n % i == 0) {
            n /= i;
            cnt++;
        }
        if (cnt) {
            ret.emplace_back(i, cnt);
        }
    }
    if (n > 1) {
        ret.emplace_back(n, 1);
    }
    return ret;
}

void get_divisors(int64_t ind, int64_t div) {
    if (ind == fact.size()) {
        return void(divisors.insert(div));
    }
    for (int i = 0; i <= fact[ind].second; ++i) {
        get_divisors(ind + 1, div);
        div *= fact[ind].first;
    }
}

/************************ Phi (Euler's Totient) ************************/

int64_t Phi(int64_t n) { // O(sqrt(n))
    int64_t ans = n;
    for (int64_t p = 2; p <= n / p; p++) {
        if (n % p == 0) {
            while (n % p == 0) {
                n /= p;
            }
            ans -= ans / p;
        }
    }
    if (n > 1) {
        ans -= ans / n;
    }
    return ans;
}

const int N = 1e7 + 5;
int phi[N];
void phi_sieve() { // O(n * log(log(n)))
    for (int i = 1; i < N; i++) {
        phi[i] = i;
    }
    for (int i = 2; i < N; i++) {
        if (phi[i] == i) {
            for (int j = i; j < N; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }
}

/*
    another version computes it by subtracting the phi of its divisors
    example:
    let  n = 10
    its divisors is 1 , 2 , 5 , 10
    n = phi[1] + phi[2] + phi[5] + phi[10]
    then phi[n] = n - (phi[1] + phi[2] + phi[5])
*/
void phi_1_to_n() { // O(n * log(n) )
    vector<int> phi(N + 1);
    phi[0] = 0;
    phi[1] = 1;
    for (int i = 2; i < N; i++) {
        phi[i] = i - 1;
    }

    for (int i = 2; i < N; i++) {
        for (int j = 2 * i; j < N; j += i) {
            phi[j] -= phi[i];
        }
    }
}

/************************ Linear Diophantine Equation ************************/
/************************ Extended Euclidean ************************/
/*
    ax + by = c
    ax + by = gcd(a , b)

    solution only exists if c is divisible by gcd(a, b)

    if we have solution (x0, y0) then we can generate more solution from this formula
    x = x0 + k * (b / g)
    y = y0 - k * (a / g)
    x + y = x0 + y0 + k * ((b - a) / g)
  */

int64_t GCD(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (!b) {
        x = 1, y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t g = GCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

// find any solution
bool find_any_solution(int64_t a, int64_t b, int64_t c, int64_t &x, int64_t &y, int64_t &g) {
    g = GCD(abs(a), abs(b), x, y);
    if (c % g) {
        return false;
    }
    x *= c / g;
    y *= c / g;
    if (a < 0)
        x = -x;
    if (b < 0)
        y = -y;
    return true;
}

// to find aint64_t possible solutions in interval
void shift_solution(int64_t &x, int64_t &y, int64_t a, int64_t b, int64_t cnt) {
    x += cnt * b;
    y -= cnt * a;
}

int64_t find_aint64_t_solutions(int64_t a, int64_t b, int64_t c, int64_t minx, int64_t maxx, int64_t miny, int64_t maxy) {
    int64_t x, y, g;
    if (!find_any_solution(a, b, c, x, y, g))
        return 0;
    a /= g, b /= g;
    int64_t sign_a = a > 0 ? +1 : -1;
    int64_t sign_b = b > 0 ? +1 : -1;
    shift_solution(x, y, a, b, (minx - x) / b);
    if (x < minx)
        shift_solution(x, y, a, b, sign_b);
    if (x > maxx)
        return 0;
    int64_t lx1 = x;
    shift_solution(x, y, a, b, (maxx - x) / b);
    if (x > maxx)
        shift_solution(x, y, a, b, -sign_b);
    int64_t rx1 = x;
    shift_solution(x, y, a, b, -(miny - y) / a);
    if (y < miny)
        shift_solution(x, y, a, b, -sign_a);
    if (y > maxy)
        return 0;
    int64_t lx2 = x;
    shift_solution(x, y, a, b, -(maxy - y) / a);
    if (y > maxy)
        shift_solution(x, y, a, b, sign_a);
    int64_t rx2 = x;
    if (lx2 > rx2)
        swap(lx2, rx2);
    int64_t lx = max(lx1, lx2), rx = min(rx1, rx2);
    if (lx > rx)
        return 0;
    return (rx - lx) / abs(b) + 1;
}

/************************ Chinese Remainder Theorem  ************************/
int64_t mod_inv(int64_t x, const int MOD = mod) {
    return power(x, MOD - 2, MOD);
}

int64_t crt(vector<int64_t> &a, vector<int64_t> &m) {
    int64_t M = 1;
    for (auto &it : m) {
        M *= it;
    }
    int64_t solution = 0;
    for (int i = 0; i < m.size(); ++i) {
        int64_t a_i = a[i];
        int64_t M_i = M / m[i];
        int64_t N_i = mod_inv(M_i, m[i]);
        solution = (solution + a_i * M_i % M * N_i) % M;
    }
    return solution;
}

/*
 * we solve 2 equations and merge them and continue like this
 * T = x mod N => T = N * k + x
 * T = y mod M => T = M * p + y
 * so N * k + x = M * p + y ===>   y - x = N * k - M * p   => Linear Diophantine
 * */
int64_t solveSystemWithCRT(vector<int64_t> &rems, vector<int64_t> &mods) {
    int64_t rem = rems[0], m = mods[0];
    // solve with prev and get the new eq
    for (int i = 1; i < (int)mods.size(); ++i) {
        int64_t x, y, g, a = m, b = -mods[i], c = rems[i] - rem;
        bool f = find_any_solution(a, b, c, x, y, g);
        if (!f)
            return -1;
        rem += mod * x;
        m = m / g * mods[i];
        rem = (rem % m + m) % m;
    }
    return rem;
}

/************************ Mobuis Function  ************************/
int mobius(int64_t n) {
    if (n == 1) {
        return 1;
    }
    int ans = 1;
    for (int i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            if (n % (i * i) == 0) {
                return 0;
            }
            n /= i, ans = -ans;
        }
    }
    if (n) {
        ans = -ans;
    }
    return ans;
}

// Sieve Moebuis
const int N = 1e6 + 5;
bitset<N> is_prime;
int mobuis[N];
void moebiusSieve() {
    is_prime.set();
    is_prime[0] = is_prime[1] = false;
    mobuis[1] = 1;
    for (int i = 2; i < N; ++i) {
        mobuis[i] = 1;
    }
    for (int i = 2; i < N; i++) {
        if (is_prime[i]) {
            mobuis[i] = -1;
            for (int j = 2 * i; j < N; j += i) {
                is_prime[j] = false;
                mobuis[j] = (j % (i * i) == 0 ? 0 : -mobuis[i]);
            }
        }
    }
}