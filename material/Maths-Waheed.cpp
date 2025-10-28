#include "bits/stdc++.h"
using namespace std;

/*
Math Guide
<---------------------------------------------------------------->

extended euclid: x*a + y*b = gcd(a, b)

bezout: different pairs from {x, y}

diophantine: x*a + y*b = c ----solvable-if---> (c % gcd(a, b) == 0)

CRT:
x = R1 (mod M1)
x = R2 (mod M2)
x = R3 (mod M3)

/////////////////////////////////////////////////////

a*x = a*y (mod M)
x = y (mod M / gcd(a, M))

/////////////////////////////////////////////////////

(x + y)^p = x^p + y^p (mod p)

/////////////////////////////////////////////////////

modular equation:
x * a = b (mod M)
x * a - b = y*M
x * a - y * M = b

solve diophantine(a, M, b)

/////////////////////////////////////////////////////

a^phi(M) = 1 (mod M) --if--> gcd(a, M) = 1

/////////////////////////////////////////////////////

x = 0
for(d:divisors(n)) x += mobius(d)
--> x = [n = 1]

/////////////////////////////////////////////////////

x = 0
for(d:divisors(n)) x += phi(d)
--> x = n

/////////////////////////////////////////////////////

---> d|y check if d divides y
gcd(x, y) = summation(i, 1, N) phi(i) * [d|x] * [d|y]

/////////////////////////////////////////////////////

geometric sequence: (1+r+r^2+...+r^n) = (r^(n+1) - 1) / (r - 1)
(1^2+2^2+3^2+...+n^2) = n*(n+1)*(2*n+1)/6

*/

/////////////////////////////////////////////////////////////////////////////////////

namespace sieve {
    vector<bool> is_prime(int N) {
        vector<int> _primes;
        vector<bool> _is_prime(N + 1, 1);
        _is_prime[0] = _is_prime[1] = 0;
        for (int64_t i = 1; i <= N; i++) {
            if (_is_prime[i]) {
                _primes.emplace_back(i);
            }
            for (auto it : _primes) {
                if (i * it > N) {
                    break;
                }
                _is_prime[i * it] = 0;
                if (i % it == 0) {
                    break;
                }
            }
        }
        return _is_prime;
    }

    vector<int> primes(int N) {
        vector<int> _primes;
        vector<bool> _is_prime(N + 1, 1);
        _is_prime[0] = _is_prime[1] = 0;
        for (int64_t i = 1; i <= N; i++) {
            if (_is_prime[i]) {
                _primes.emplace_back(i);
            }
            for (auto it : _primes) {
                if (i * it > N) {
                    break;
                }
                _is_prime[i * it] = 0;
                if (i % it == 0) {
                    break;
                }
            }
        }
        return _primes;
    }

    vector<int> mobius(int N) {
        vector<int> _primes, mob(N + 1, 1);
        vector<bool> _is_prime(N + 1, 1);
        _is_prime[0] = _is_prime[1] = 0;
        for (int64_t i = 1; i <= N; i++) {
            if (_is_prime[i]) {
                mob[i] = -1;
                _primes.emplace_back(i);
            }
            for (auto it : _primes) {
                if (i * it > N) {
                    break;
                }
                mob[i * it] = !!(i % it) * -mob[i];
                _is_prime[i * it] = 0;
                if (i % it == 0) {
                    break;
                }
            }
        }
        return mob;
    }

    vector<bool> segmented_sieve(int64_t l, int64_t r) {
        vector<bool> primes_seg(r - l + 1, 1);
        if (l <= 1 && 1 <= r) {
            primes_seg[1 - l] = 0;
        }
        vector<int> prime = primes((int)sqrt(r));
        for (auto it : prime) {
            int64_t start = l / it * it + (l % it ? it : 0);
            for (int64_t i = start; i <= r; i += it) {
                if (i != it) {
                    primes_seg[i - l] = 0;
                }
            }
        }
        return primes_seg;
    }

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

    vector<vector<int>> factors(int N) {
        auto _spf = spf(N);
        vector<vector<int>> fact(N + 1);
        for (int i = 1; i <= N; i++) {
            int cur = i;
            while (1 < cur) {
                fact[i].emplace_back(_spf[cur]);
                cur /= _spf[cur];
            }
        }
        return fact;
    }

    vector<vector<pair<int, int>>> factors_cnt(int N) {
        auto _spf = spf(N);
        vector<vector<pair<int, int>>> fact(N + 1);
        for (int i = 1; i <= N; i++) {
            int cur = i;
            vector<int> v;
            while (cur > 1) {
                if (v.empty() || _spf[cur] != v.back()) {
                    v.emplace_back(_spf[cur]);
                }
                cur /= _spf[cur];
            }

            cur = i;
            for (auto &p : v) {
                int cnt = 0;
                while (cur % p == 0) {
                    cur /= p;
                    ++cnt;
                }
                if (cnt) {
                    fact[i].emplace_back(p, cnt);
                }
            }
        }
        return fact;
    }

    vector<vector<int>> divs(int N) {
        vector<vector<int>> divisors(N + 1);
        for (int i = 1; i <= N; i++) {
            for (int j = i; j <= N; j += i) {
                divisors[j].emplace_back(i);
            }
        }
        return divisors;
    }

    vector<int> phi(int N) {
        vector<int> _phi(N + 1);
        iota(_phi.begin(), _phi.end(), 0);
        for (int i = 1; i <= N; i++) {
            for (int j = 2 * i; j <= N; j += i) {
                _phi[j] -= _phi[i];
            }
        }
        return _phi;
    }

    vector<int> next_prime(int N) {
        N += 1000;
        auto _is_prime = is_prime(N);
        vector<int> _next_prime(N + 1, 0);
        int last = 1e9 + 7;
        for (int i = N; ~i; i--) {
            _next_prime[i] = last;
            if (_is_prime[i]) {
                last = i;
            }
        }
        return _next_prime;
    }
}

///////////////////////////////////////////////////////////////////////////////////

int64_t extended_euclid(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (a < 0) {
        auto t = extended_euclid(-a, b, x, y);
        x = -x;
        return t;
    }
    if (b < 0) {
        auto t = extended_euclid(a, -b, x, y);
        y = -y;
        return t;
    }
    if (!b) {
        x = 1, y = 0;
        return a;
    }
    int64_t g = extended_euclid(b, a % b, y, x);
    y -= x * (a / b);
    return g;
}

auto bezout(int64_t a, int64_t b, int k = 1) {
    int64_t x, y, g = extended_euclid(a, b, x, y);
    vector<pair<int64_t, int64_t>> ret(k);
    for (int i = 0; i < k; ++i) {
        ret[i] = {x + i * b / g, y - i * a / g};
    }
    return ret;
}

auto LD_equation(int64_t a, int64_t b, int64_t c, int k = 1) {
    auto sol = bezout(a, b, k);
    if (c % gcd(a, b)) {
        sol.clear();
    }
    for (auto &[x, y] : sol) {
        x *= c / gcd(a, b);
        y *= c / gcd(a, b);
    }
    return sol;
}

int64_t CRT(auto &rems, auto &mods) {
    vector<int64_t> ret;
    int64_t mod = mods[0], rem = rems[0];
    for (int i = 1; i < mods.size(); ++i) {
        auto sol = LD_equation(mod, -mods[i], rems[i] - rem);
        if (sol.empty()) {
            return -1;
        }
        auto [q1, q2] = sol[0];
        mod = lcm(mod, mods[i]);
        rem = ((q2 * mods[i] + rems[i]) % mod + mod) % mod;
    }
    return rem;
}

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
        if (i < T) {
            cout << '\n';
        }
    }
}