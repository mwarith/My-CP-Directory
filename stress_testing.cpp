#include "bits/stdc++.h"
using namespace std;
#include "ext/pb_ds/assoc_container.hpp"
using namespace __gnu_pbds;

auto TTT = clock();
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
    }

    void brute() {
    }

    void ac() {
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
        cerr << "Time: " << (clock() - TTT) * 1000 / CLOCKS_PER_SEC << " ms\n";
        return;
    }
}

int main() {
    freopen("out.txt", "w", stdout);
    stress_test::stress_test();
}