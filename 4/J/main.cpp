#include <iostream>
#include <vector>
#include <queue>

struct edge {
    size_t to;
    int32_t flow, capacity;

    edge(size_t to, int32_t flow, int32_t capacity) : to(to), flow(flow), capacity(capacity) {}
};

bool bfs(std::vector<std::vector<size_t> > &g, std::vector<edge> &edges, std::vector<size_t> &dp) {
    dp.assign(dp.size(), dp.size());
    std::queue<size_t> q;
    q.push(0);
    dp[0] = 0;
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (size_t num : g[v]) {
            size_t to = edges[num].to;
            if (dp[to] == dp.size() && edges[num].flow < edges[num].capacity) {
                dp[to] = dp[v] + 1;
                q.push(to);
            }
        }
    }
    return dp.back() != dp.size();
}

size_t dfs(size_t v, std::vector<std::vector<size_t> > &g,
           std::vector<edge> &edges, std::vector<size_t> &dp, std::vector<size_t> &it) {
    if (v == g.size() - 1) {
        return 1;
    }
    for (; it[v] < g[v].size(); it[v]++) {
        edge &e = edges[g[v][it[v]]];
        if (dp[e.to] == dp[v] + 1 && e.flow < e.capacity) {
            if (dfs(e.to, g, edges, dp, it) > 0) {
                edges[g[v][it[v]]].flow += 1;
                edges[g[v][it[v]] ^ 1].flow -= 1;
                return 1;
            }
        }
    }
    return 0;
}

bool check(int32_t min_cost, std::vector<std::vector<int32_t> > &c) {
    size_t n = c.size();
    std::vector<edge> edges;
    std::vector<std::vector<size_t> > g(2 * n + 2);
    for (size_t i = 1; i <= n; i++) {
        g[0].push_back(edges.size());
        edges.emplace_back(i, 0, 1);
        g[i].push_back(edges.size());
        edges.emplace_back(0, 0, 0);
    }
    for (size_t j = n + 1; j <= 2 * n; j++) {
        g[j].push_back(edges.size());
        edges.emplace_back(2 * n + 1, 0, 1);
        g.back().push_back(edges.size());
        edges.emplace_back(j, 0, 0);
    }
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (c[i][j] >= min_cost) {
                g[i + 1].push_back(edges.size());
                edges.emplace_back(n + 1 + j, 0, 1);
                g[n + 1 + j].push_back(edges.size());
                edges.emplace_back(i + 1, 0, 0);
            }
        }
    }
    std::vector<size_t> dp(2 * n + 2), it(2 * n + 2);
    while (bfs(g, edges, dp)) {
        it.assign(2 * n + 2, 0);
        while (dfs(0, g, edges, dp, it) > 0) {}
    }
    for (size_t num : g[0]) {
        if (edges[num].flow != edges[num].capacity) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<std::vector<int32_t> > c(n, std::vector<int32_t>(n));
    for (auto &v : c) {
        for (auto &cost : v) {
            std::cin >> cost;
        }
    }
    int32_t l = -1,
            r = 1000001;
    while (l + 1 < r) {
        int32_t mid = (l + r) / 2;
        if (check(mid, c)) {
            l = mid;
        } else {
            r = mid;
        }
    }
    std::cout << l << std::endl;
    return 0;
}
