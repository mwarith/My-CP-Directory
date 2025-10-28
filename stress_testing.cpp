#include "bits/stdc++.h"
using namespace std;
#include "ext/pb_ds/assoc_container.hpp"
using namespace __gnu_pbds;

auto tt = clock();

const int N = 1E7 + 5;
int phi[N + 1];

struct Compare {
    bool operator()(const pair<int, int> &a, const pair<int, int> &b) const {
        return 1LL * a.first * b.second < 1LL * b.first * a.second;
    }
};
map<pair<int, int>, vector<int>, Compare> m;

int fre[10]{};
bool check(int s, int t) {
    memset(fre, 0, sizeof(fre));
    while (s > 0) {
        fre[s % 10]++;
        s /= 10;
    }
    while (t > 0) {
        fre[t % 10]--;
        t /= 10;
    }

    for (int i = 0; i < 10; ++i) {
        if (fre[i] != 0) return false;
    }
    return true;
}
void init() {
    for (int i = 0; i <= N; i++) {
        phi[i] = i;
    }

    for (int i = 2; i <= N; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= N; j += i) {
                phi[j] -= phi[j] / i;
            }
        }

        if (check(i, phi[i])) {
            int g = __gcd(i, phi[i]);
            m[{i / g, phi[i] / g}].emplace_back(i);
        }
    }

    for (auto &pair : m) {
        sort(pair.second.begin(), pair.second.end());
    }
}

namespace stress_test {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    int64_t rand(int64_t l, int64_t r) {
        return uniform_int_distribution<int64_t>(l, r)(rng);
    }

    void shuffle(auto &x) {
        shuffle(x.begin(), x.end(), rng);
    }

    int turn;
    stringstream cin, out1, out2;
    struct {
        auto operator<<(const auto &x) {
            if (turn == 0) {
                cin << x;
            }
            if (turn == 1) {
                out1 << x;
            }
            if (turn == 2) {
                out2 << x;
            }
            if (turn == 3) {
                cout << x;
            }
            return *this;
        }
    } cout;

    void generate() {
        cout << rand(1, 10000);
    }

    void brute() {
        int n;
        cin >> n;

        int64_t best_N = -1, best_Phi = 1;
        for (int i = 3; i < n; i++) {
            int phi_N = phi[i];
            if (check(i, phi_N) && (best_N == -1 || i * best_Phi <= best_N * phi_N)) {
                best_N = i;
                best_Phi = phi_N;
            }
        }

        if (best_N == -1) {
            cout << "No solution.";
        } else {
            cout << best_N;
        }
    }

    void ac() {
        int n;
        cin >> n;

        for (auto &x : m) {
            auto &v = x.second;
            auto it = upper_bound(v.begin(), v.end(), n - 1);
            if (it != v.begin()) {
                it = prev(it);
            }
            if (*it < n) {
                return cout << *it, void();
            }
        }
        cout << "No solution.";
    }

    void stress_test(int T = 5E4) {
        string input;
        for (int i = 1; i <= T; ++i) {
            cin = stringstream(), out1 = stringstream(), out2 = stringstream();
            turn = 0, generate(), input = cin.str();
            turn = 1, brute(), cin = stringstream(input);
            turn = 2, ac(), cin = stringstream(input);
            turn = 3;
            if (out1.str() != out2.str()) {
                cout << "Wrong: " << i << "\n\n" + input + "\n\nbrute:\n" + out1.str() + "\n\nac:\n" + out2.str() + "\n\n";
                exit(0);
            }
        }
        cout << "✅Accepted✅";
        cerr << "Time: " << (clock() - tt) * 1000 / CLOCKS_PER_SEC << " ms\n";
        exit(0);
    }
}

int main() {
    freopen("out.txt", "w", stdout);
    stress_test::stress_test();
}