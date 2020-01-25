#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct edge {
    size_t num, to;
    int32_t flow, capacity;

    edge(size_t num, size_t to, int32_t flow, int32_t capacity) : num(num), to(to),
                                                                  flow(flow), capacity(capacity) {}
};

bool bfs(std::vector<std::vector<size_t> > &g, std::vector<edge> &edges, std::vector<size_t> &dp) {
    std::queue<size_t> q;
    q.push(0);
    dp.assign(dp.size(), dp.size());
    dp[0] = 0;
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (size_t num : g[v]) {
            edge &e = edges[num];
            if (dp[e.to] == dp.size() && e.flow < e.capacity) {
                dp[e.to] = dp[v] + 1;
                q.push(e.to);
            }
        }
    }
    return dp.back() != dp.size();
}

int32_t dfs(size_t v, int32_t cur_flow, std::vector<std::vector<size_t> > &g,
            std::vector<edge> &edges, std::vector<size_t> &dp, std::vector<size_t> &it) {
    if (v == g.size() - 1 || cur_flow == 0) {
        return cur_flow;
    }
    for (; it[v] < g[v].size(); it[v]++) {
        edge &e = edges[g[v][it[v]]];
        if (dp[e.to] == dp[v] + 1) {
            int32_t add_flow = dfs(e.to, std::min(cur_flow, e.capacity - e.flow), g, edges, dp, it);
            if (add_flow > 0) {
                edges[g[v][it[v]]].flow += add_flow;
                edges[g[v][it[v]] ^ 1].flow -= add_flow;
                return add_flow;
            }
        }
    }
    return 0;
}

void find_max_flow(size_t n, std::vector<std::vector<size_t> > &g, std::vector<edge> &edges) {
    std::vector<size_t> dp(n), it(n);
    while (bfs(g, edges, dp)) {
        it.assign(n, 0);
        while (dfs(0, std::numeric_limits<int32_t>::max(), g, edges, dp, it) > 0) {}
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<edge> edges;
    std::vector<std::vector<size_t> > g(n + 2);
    for (size_t i = 1; i <= m; i++) {
        size_t from, to;
        int low, capacity;
        std::cin >> from >> to >> low >> capacity;
        g[0].push_back(edges.size());
        edges.emplace_back(i, to, 0, low);
        g[to].push_back(edges.size());
        edges.emplace_back(i, 0, 0, 0);
        g[from].push_back(edges.size());
        edges.emplace_back(i, to, 0, capacity - low);
        g[to].push_back(edges.size());
        edges.emplace_back(i, from, 0, 0);
        g[from].push_back(edges.size());
        edges.emplace_back(n + 1, n + 1, 0, low);
        g.back().push_back(from);
        edges.emplace_back(n + 1, from, 0, 0);
    }
    find_max_flow(n + 2, g, edges);
    for (size_t num : g[0]) {
        edge &e = edges[num];
        if (e.flow != e.capacity) {
            std::cout << "NO" << std::endl;
            return 0;
        }
    }
    std::cout << "YES" << std::endl;
    std::vector<int32_t> ans(m, 0);
    for (size_t num : g[0]) {
        edge &e = edges[num];
        ans[e.num - 1] = e.flow;
    }
    for (size_t i = 1; i <= n; i++) {
        for (size_t num : g[i]) {
            edge &e = edges[num];
            if (e.capacity != 0 && e.to != n + 1) {
                ans[e.num - 1] += e.flow;
            }
        }
    }
    std::for_each(ans.begin(), ans.end(), [](int32_t flow) { std::cout << flow << std::endl; });
    return 0;
}
