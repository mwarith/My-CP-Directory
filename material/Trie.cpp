#include "bits/stdc++.h"
using namespace std;
#define ll long long
#define sz(x) (int)x.size()
#define el '\n'
const int oo = 0X3F3F3F3F;
const ll OO = 0X3F3F3F3F3F3F3F3F, mod = 1e9 + 7;
int dx[] = {0, 1, 0, -1, 1, 1, -1, -1};
int dy[] = {1, 0, -1, 0, 1, -1, 1, -1};

class trie {
private:
    struct node {
        int child[26];
        int word = 0, prefix = 0;
        node() {
            fill(begin(child), end(child), -1);
        }
    };

    vector<node> tree = vector<node>(1);

public:
    void insert(const string &s, int cnt = 1) {
        int cur = 0;
        for (auto it : s) {
            int to = it - 'a';
            if (tree[cur].child[to] == -1) {
                tree[cur].child[to] = tree.size();
                tree.emplace_back(node());
            }
            cur = tree[cur].child[to];
            tree[cur].prefix += cnt;
        }
        tree[cur].word += cnt;
    }

    void erase(const string &s) {
        int cur = 0;
        for (auto it : s) {
            int to = it - 'a';
            if (tree[cur].child[to] == -1 || tree[tree[cur].child[to]].prefix == 1) {
                tree[cur].child[to] = -1;
                return;
            }
            cur = tree[cur].child[to];
            tree[cur].prefix--;
        }
        tree[cur].word--;
    }

    int count_word(const string &s) {
        int cur = 0;
        for (auto it : s) {
            int to = it - 'a';
            if (tree[cur].child[to] == -1) {
                return 0;
            }
            cur = tree[cur].child[to];
        }
        return tree[cur].word;
    }

    int count_prefix(const string &s) {
        int cur = 0;
        for (auto it : s) {
            int to = it - 'a';
            if (tree[cur].child[to] == -1) {
                return 0;
            }
            cur = tree[cur].child[to];
        }
        return tree[cur].prefix;
    }
};

template <int LOG = 62>
class binary_trie {
private:
    struct node {
        int cnt = 0, child[2] = {-1, -1};
    };
    vector<node> tree = vector<node>(1);

public:
    void insert(int x, int cnt = 1) {
        int cur = 0;
        for (int bit = LOG; ~bit; --bit) {
            int to = !!(x & (1LL << bit));
            if (tree[cur].child[to] == -1) {
                tree[cur].child[to] = tree.size();
                tree.emplace_back(node());
            }
            cur = tree[cur].child[to];
            tree[cur].cnt += cnt;
        }
    }

    void erase(int x) {
        int cur = 0;
        for (int bit = LOG; ~bit; --bit) {
            int to = !!(x & (1LL << bit));
            if (tree[cur].child[to] == -1 || tree[tree[cur].child[to]].cnt == 1) {
                tree[cur].child[to] = -1;
                return;
            }
            cur = tree[cur].child[to];
            tree[cur].cnt--;
        }
    }

    int max_xor(int x) {
        int cur = 0, ret = 0;
        for (int bit = LOG; ~bit; bit--) {
            int to = !!(x & (1LL << bit));
            if (~tree[cur].child[!to]) {
                cur = tree[cur].child[!to];
                ret += !to * (1LL << bit);
            } else {
                cur = tree[cur].child[to];
                ret += to * (1LL << bit);
            }
        }
        return x ^ ret;
    }

    int min_xor(int x) {
        return -1 ^ max_xor(x ^ -1);
    }
};

void ac() {
}

signed main() {
}