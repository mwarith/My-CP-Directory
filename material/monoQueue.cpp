#include "bits/stdc++.h"
using namespace std;
/*
compare functor determines the monotonic order:
    - less<T> (default): (min at front)
    - greater<T>: (max at front)
*/
template <typename T, typename compare = less<T>>
class mono_queue {
    deque<pair<T, int>> dq;
    compare cmp;

public:
    void push(T value, int ind) {
        // remove all elements that break monotonic order
        while (!dq.empty() && cmp(value, dq.back().first)) {
            dq.pop_back();
        }
        dq.emplace_back(value, ind);
    }

    void pop(int ind) {
        if (!dq.empty() && dq.front().second <= ind) {
            dq.pop_front();
        }
    }

    T top() const {
        return dq.front().first;
    }

    bool empty() const {
        return dq.empty();
    }
};

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
    }
}