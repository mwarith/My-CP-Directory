#include <bits/stdc++.h>
using namespace std;

const int N = 2000, mod = 1e9 + 7;
string a, b;
int d, m, dp[N][N][2][2];

int64_t calc(int i, int sum, bool eqa, bool eqb) {
    if (i == b.size())
        return !sum;
    auto &ret = dp[i][sum][eqa][eqb];
    if (ret != -1)
        return ret;
    if (i & 1) {
        if ((eqa && d < a[i] - '0') || (eqb && d > b[i] - '0'))
            return ret = 0;
        return ret = calc(i + 1, (sum * 10 + d) % m, eqa & (d == a[i] - '0'), eqb & (d == b[i] - '0')) % mod;
    }
    int low = (eqa ? a[i] - '0' : 0);
    int high = (eqb ? b[i] - '0' : 9);
    ret = 0;
    for (int dg = low; dg <= high; ++dg) {
        if (dg != d)
            (ret += calc(i + 1, (sum * 10 + dg) % m, eqa & (dg == low), eqb & (dg == high))) %= mod;
    }
    return ret;
}

// pos, last , f
int64_t dp2[20][12][2];
vector<int> nums;

int64_t calc2(int cur, int prev, int f) {
    if (cur == nums.size()) {
        return 1;
    }
    auto &ret = dp2[cur][prev][f];
    if (ret != -1)
        return ret;
    ret = 0;
    int lmt = f ? 9 : nums[cur];
    for (int dg = 0; dg <= lmt; ++dg) {
        if (dg == prev)
            continue;
        int nf = f || (dg < lmt);
        ret += calc2(cur + 1, dg, nf);
    }
    return ret;
}

int64_t solve(int64_t x) {
    nums.clear();
    while (x != 0) {
        nums.emplace_back(x % 10);
        x /= 10;
    }
    reverse(nums.begin(), nums.end());
    memset(dp2, -1, sizeof(dp2));
    return calc2(0, 0, 0);
}

void ac() {
    int64_t a, b;
    cin >> a >> b;
    cout << solve(b) - solve(a - 1);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    if (freopen("in.txt", "r", stdin)) {
        freopen("out.txt", "w", stdout);
    }
    int T = 1;
    // cin >> T;
    for (int i = 1; i <= T; i++) {
        ac();
    }
}
