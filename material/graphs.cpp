#include <bits/stdc++.h>
using namespace std;
const int inf = 1e9;

/************** Bridges, Cut points */
class bridges {
    int n;
    vector<int> vis, dep, low, is_bridge, is_cut_point, id;
    const vector<vector<pair<int, int>>> &g;

public:
    bridges(vector<vector<pair<int, int>>> &_g, int m) : g(_g) {
        n = g.size() - 1;
        vis = dep = is_cut_point = vector(n + 1, 0);
        is_bridge = vector(m, 0);
        id = vector(n + 1, -1);
        low = vector(n + 1, (int)1e9);
        function<void(int, int)> build = [&](int v, int p) {
            int children = 0;
            vis[v] = 1;
            for (auto &[ch, idx] : g[v]) {
                if (ch == p) {
                    continue;
                }
                if (vis[ch]) {
                    low[v] = min(low[v], dep[ch]);
                    continue;
                }
                children++;

                dep[ch] = dep[v] + 1;
                build(ch, v);
                low[v] = min(low[v], low[ch]);
                if (low[ch] > dep[v]) {
                    is_bridge[idx] = 1;
                }
                if (low[ch] >= dep[v] && p != -1) {
                    is_cut_point[v] = 1;
                }
            }
            if (p == -1 && children > 1) {
                is_cut_point[v] = 1;
            }
        };
        build(1, -1);
    }

    vector<int> get_bridges() {
        return is_bridge;
    }

    vector<int> get_cut_points() {
        return is_cut_point;
    }

    vector<vector<int>> get_tree() {
        int cur_id = 0;
        function<void(int)> dfs = [&](int v) {
            id[v] = cur_id + 1;
            for (auto &[ch, idx] : g[v]) {
                if (is_bridge[idx] || id[ch] != -1) {
                    continue;
                }
                dfs(ch);
            }
        };

        for (int i = 1; i <= n; ++i) {
            if (id[i] != -1) {
                continue;
            }
            dfs(i);
            cur_id++;
        }

        vector<vector<int>> tree(cur_id + 1);
        for (int v = 1; v <= n; ++v) {
            for (auto &[ch, _] : g[v]) {
                if (id[v] != id[ch]) {
                    tree[id[v]].emplace_back(id[ch]);
                }
            }
        }
        return tree;
    }
};
/*                SCC                  */
template <int base = 1>
class SCC {
private:
    stack<int> st;
    int n, id = 1, comp_cnt;
    const vector<vector<int>> &g;
    vector<vector<int>> dag, comps;
    vector<int> on_stack, low_link, ids, in_deg;

    void dfs(int v) {
        st.emplace(v), on_stack[v] = 1;
        low_link[v] = ids[v] = id++;

        for (auto &ch : g[v]) {
            if (ids[ch] == -1) {
                dfs(ch);
            }
            if (on_stack[ch]) {
                low_link[v] = low_link[ch] = min(low_link[v], low_link[ch]);
            }
        }

        if (ids[v] == low_link[v]) {
            while (true) {
                int cur = st.top();
                st.pop(), on_stack[cur] = 0;
                low_link[cur] = low_link[v];
                if (cur == v) {
                    break;
                }
            }
        }
    }

public:
    SCC() = default;
    SCC(const vector<vector<int>> &_g) : g(_g) {
        n = g.size() - 1;
        ids = vector(n + 1, -1);
        on_stack = low_link = vector(n + 1, 0);

        for (int i = base; i <= n; ++i) {
            if (ids[i] == -1) {
                dfs(i);
            }
        }

        set<int> s(low_link.begin() + base, low_link.end());
        vector<int> c(s.begin(), s.end());
        comp_cnt = c.size();
        for (int i = base; i <= n; ++i) {
            low_link[i] = lower_bound(c.begin(), c.end(), low_link[i]) - c.begin();
        }
    }

    int components_cnt() {
        return comp_cnt;
    }

    int component(int v) {
        return low_link[v];
    }

    bool same_component(int u, int v) {
        return component(u) == component(v);
    }

    vector<vector<int>> &compress() {
        if (dag.empty()) {
            in_deg.assign(components_cnt(), 0);
            dag.resize(components_cnt());
            for (int v = base; v <= n; ++v) {
                for (auto &ch : g[v]) {
                    if (!same_component(v, ch)) {
                        dag[component(v)].emplace_back(component(ch));
                        in_deg[component(ch)]++;
                    }
                }
            }
        }
        return dag;
    }

    vector<vector<int>> &get_components() {
        if (comps.empty()) {
            comps.resize(components_cnt());
            for (int i = base; i <= n; ++i) {
                comps[component(i)].emplace_back(i);
            }
        }
        return comps;
    }

    bool is_head(int comp) {
        compress();
        return !in_deg[comp];
    }
};

/*        Bellman Ford                 */
constexpr int64_t INF = 1e18;
template <int base = 1, int mode = 1> // mode = 1 (min), mode = -1 (max)
class bellman_ford {
private:
    int n;
    set<int> inf_nodes;
    vector<int64_t> dis;
    vector<int> par1, par2, par3;

public:
    bellman_ford() = default;

    bellman_ford(vector<vector<pair<int, int64_t>>> &g, const vector<int> &src = {base}) {
        n = (int)g.size() - 1;
        dis = vector<int64_t>(n + 1, INF);
        par1 = par2 = vector<int>(n + 1, -1);

        for (auto it : src) {
            dis[it] = 0;
        }

        int itr = n;
        while (itr--) {
            for (int i = base; i <= n; ++i) {
                for (auto [next, cost] : g[i]) {
                    if (dis[i] != INF && dis[i] + mode * cost < dis[next]) {
                        if (itr) {
                            dis[next] = dis[i] + mode * cost;
                        } else {
                            inf_nodes.insert(next);
                        }
                        par1[next] = i;
                    }
                }
            }
        }

        vector<int> q(inf_nodes.begin(), inf_nodes.end());
        for (int i = 0; i < q.size(); ++i) {
            if (dis[q[i]] == -INF) {
                continue;
            }
            dis[q[i]] = -INF;

            for (auto [it, c] : g[q[i]]) {
                if (dis[it] != -INF) {
                    par2[it] = q[i];
                    q.emplace_back(it);
                }
            }
        }

        q = src;
        vector<int> vis(n + 1);
        par3 = vector<int>(n + 1);
        for (auto it : src) {
            par3[it] = -1;
        }
        for (int i = 0; i < q.size(); ++i) {
            if (vis[q[i]]) {
                continue;
            }
            vis[q[i]] = 1;
            for (auto [it, c] : g[q[i]]) {
                if (!vis[it]) {
                    if (~par3[it]) {
                        par3[it] = q[i];
                    }
                    q.emplace_back(it);
                }
            }
        }
    }

    int64_t best(int node) {
        return mode * dis[node] == INF ? -1 : mode * dis[node];
    }

    tuple<vector<int>, int, int> get_path(int node) {
        if (dis[node] == INF) {
            return {};
        }

        vector<int> ret, vis(n + 1);
        while (dis[node] == -INF && !inf_nodes.count(node)) {
            ret.emplace_back(node);
            node = par2[node];
        }

        for (int itr = n + 1; ~node && itr--; node = par1[node]) {
            ret.emplace_back(node);
        }

        int l = (int)ret.size() - 1 - (find(ret.rbegin() + 1, ret.rend(), ret.back()) - ret.rbegin());
        int r = (int)ret.size() - 1;
        for (; ~node; node = par3[node]) {
            ret.emplace_back(node);
        }
        reverse(ret.begin(), ret.end());
        l = (int)ret.size() - l - 1;
        r = (int)ret.size() - r - 1;
        swap(l, r);
        if (dis[ret.back()] != -INF) {
            l = r = -1;
        }
        return {ret, l, r};
    }
};

/******************* Tree Diameter */
const int N = 1e5;
vector<int> g[N];
int far = 0, mx = 0;
pair<int, int> dfs(int v, int p) {
    int max_height[3] = {-1, -1, -1}, diameter = 0;
    for (auto ch : g[v]) {
        if (ch == p) {
            continue;
        }
        auto cur = dfs(ch, v);
        diameter = max(diameter, cur.first);
        max_height[0] = cur.second + 1;
        sort(max_height, max_height + 3);
    }
    for (auto &it : max_height) {
        if (!~it) {
            it = 0;
        }
    }
    diameter = max(diameter, max_height[1] + max_height[2]);
    return {diameter, max_height[2]};
}

// another way
{
    auto bfs = [&](int start) {
        vector<int> dis(n + 1, -1);
        queue<int> q;
        dis[start] = 0;
        q.emplace(start);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int ch : tree[cur]) {
                if (dis[ch] == -1) {
                    dis[ch] = dis[cur] + 1;
                    q.emplace(ch);
                }
            }
        }
        return dis;
    };

    auto d = bfs(1);
    int max_node = max_element(d.begin(), d.end()) - d.begin();
    auto d_mx = bfs(max_node);
    int diameter = *max_element(d_mx.begin(), d_mx.end());
}
/*********** Tarjan ********** */

vector<int64_t> dfsn, low, inStack, comp;
vector<vector<int>> comps, g;
stack<int64_t> st;
int timer = 0;

void tarjan(int u) {
    dfsn[u] = low[u] = timer++;
    st.push(u);
    inStack[u] = 1;

    for (auto ch : g[u]) {
        if (dfsn[ch] == -1) {
            tarjan(ch);
            low[u] = min(low[u], low[ch]);
        } else if (inStack[ch]) {
            low[u] = min(low[u], dfsn[ch]);
        }
    }

    if (low[u] == dfsn[u]) {
        comps.emplace_back(vector<int>());
        while (true) {
            auto v = st.top();
            st.pop();
            inStack[v] = 0;
            comps.back().push_back(v);
            comp[v] = comps.size();
            if (v == u)
                break;
        }
    }
}

/*********** Prim ********** */
int64_t prim(const vector<vector<pair<int, int>>> &g) {
    const int n = g.size();
    int64_t min_cost = 0;
    vector<int64_t> dist(n, inf);
    priority_queue<pair<int64_t, int>, vector<pair<int64_t, int>>, greater<>> q;
    dist[0] = 0;
    q.push({0, 0});
    int added = 0;
    while (added < n) {
        if (q.empty()) {
            return -1;
        }
        auto [curr_cost, v] = q.top();
        q.pop();
        if (dist[v] < curr_cost) {
            continue;
        }
        added++;
        min_cost += curr_cost;
        for (auto &[ch, n_cost] : g[v]) {
            if (n_cost < dist[ch]) {
                dist[ch] = n_cost;
                q.push({n_cost, ch});
            }
        }
    }

    return min_cost;
}

const int N = 1e5 + 5, inf = 1e9;
int dis[N];
vector<int> g[N];
int n, m;
/******************** Bipartite Graph Check ******************/
void check_bipartite() {
    queue<int> q;
    vector<int> side(n, -1);
    bool is_bipartite = true;
    for (int i = 0; i < n; i++) {
        if (~side[i])
            continue;
        q.push(i);
        side[i] = 0;
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (auto ch : g[cur]) {
                if (~side[ch])
                    is_bipartite &= (side[cur] != side[ch]);
                else
                    side[ch] = side[cur] ^ 1, q.push(ch);
            }
        }
    }
}

/************************ Floyd ******************************/
void floyd() {
    int n, m;
    cin >> n >> m;
    const int64_t inf = 1e18;
    vector dis(n + 1, vector(n + 1, inf));
    for (int i = 0, x, y, w; i < m; ++i) {
        cin >> x >> y >> w;
        dis[x][y] = min<int64_t>(w, dis[x][y]);
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (i == j && dis[i][j] != inf) {
                dis[i][j] = 0;
            }
        }
    }

    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (dis[i][k] < inf && dis[k][j] < inf) {
                    dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
                }
            }
        }
    }
}

/******************** Bellman Ford ********************/
int n, m, k;
struct edge {
    int a, b;
    int64_t w;
    edge(int from, int to, int64_t w) : a(from), b(to), w(w) {}
};

bool bellman_ford(vector<edge> &edges) {
    vector<int> p(n + 1, -1);
    int x;
    for (int i = 0; i < n; ++i) {
        x = -1;
        for (auto &[a, b, w] : edges) {
            if (dis[a] < inf && dis[b] > dis[a] + w) {
                dis[b] = dis[a] + w;
                p[b] = a;
                x = b;
            }
            if (x == -1)
                break;
        }
    }
    /*********** Find Negative Cycle Path ************/
    if (x == -1)
        cout << "No negative cycle from " << v;
    else {
        int y = x;
        for (int i = 0; i < n; ++i)
            y = p[y];

        vector<int> path;
        for (int cur = y;; cur = p[cur]) {
            path.push_back(cur);
            if (cur == y && path.size() > 1)
                break;
        }
        reverse(path.begin(), path.end());

        cout << "Negative cycle: ";
        for (int u : path)
            cout << u << ' ';
    }

    /****** Finding Path  *******/
    // vector<int> path;
    // for (int cur = t; cur != -1; cur = p[cur])
    //     path.push_back(cur);
    // reverse(path.begin(), path.end());
    return false;
}

/*********************** ********************/
// Print a cycle in an indirected graph
vector<vector<int>> g;
vector<int> vis, cycle;
int isCyclic = 0, c = 0, stop = 0;

void dfs(int p, int v) {
    vis[v] = 1;
    for (auto ch : g[v]) {
        if (isCyclic)
            break;
        if (!vis[ch])
            dfs(v, ch);
        else if (vis[ch] && p != ch) {
            isCyclic = 1;
            c = ch;
        }
    }
    if (isCyclic && !stop)
        cycle.emplace_back(v);
    if (v == c)
        stop = 1;
}

/********************* Dijkstra **********************/
void dijkstra(int64_t start) {
    vector<int64_t> dist(n + 1, inf), par(n + 1, -1);
    priority_queue<pair<int64_t, int>, vector<pair<int64_t, int>>, greater<>> pq;
    dist[1] = 0;
    pq.emplace(dist[1], 1);
    while (!pq.empty()) {
        auto [d, cur] = pq.top();
        pq.pop();
        if (d != dist[cur])
            continue;
        for (auto [to, w] : g[cur]) {
            if (dist[to] > d + w) {
                dist[to] = d + w;
                pq.emplace(dist[to], to);
                par[to] = cur;
            }
        }
    }
}

/********************************************/
// Determine Cycle in directed graph (DAG)
vector<int> vis;
vector<vector<int>> g;
bool IsDAG = true;
void DFS(int p) {         // p = parent
    vis[p] = 2;           // In Stack
    for (int ch : g[p]) { // ch = child
        if (vis[ch] == 2)
            IsDAG = false; // Here's v cycle
        if (!vis[ch])
            DFS(ch); // Recurse
    }
    vis[p] = 1; // Visited
}
/********************************************/

// Cyclic Undirected graph
vector<vector<int>> g;
vector<int> vis;
bool no = false;
void dfs(int c, int p) {
    vis[c] = 1;
    for (int ch : g[c]) {
        if (vis[ch] && ch != p)
            no = true;
        else if (!vis[ch])
            dfs(ch, c);
    }
}

/*********************************************/

// Topological Sort
vector<vector<int>> g;
vector<int> vis;
vector<int> topSort;
void dfs(int u) {
    vis[u] = true;
    for (int ch : g[u]) {
        if (!vis[ch]) {
            dfs(ch);
        }
    }
    topSort.emplace_back(u);
}

/*********************************************/

// BFS Levels
// const ll sz = 1e5+5;
int n, m, ans = 0;
// vector<int> g[sz], vis(sz, LLONG_MAX);

void bfs(int v) {
    queue<int> q;
    q.push(v);
    vis[v] = 0;
    while (!q.empty()) {
        int p = q.front();
        q.pop();
        for (auto ch : g[p]) {
            if (vis[ch] == inf) {
                vis[ch] = vis[p] + 1;
                q.push(ch);
            }
        }
    }
}

void ac() {
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    if (fopen("in.txt", "r")) {
        freopen("in.txt", "r", stdin);
        freopen("out.txt", "w", stdout);
    }
    int _ = 1;
    // cin >> _;
    for (int i = 1; i <= _; i++) {
        ac();
    }
}