#include <iostream>
#include <vector>
#include <queue>
#include <limits>

struct edge {
    size_t to;
    int32_t flow, capacity;

    edge(size_t to, int32_t flow, int32_t capacity) : to(to), flow(flow), capacity(capacity) {}
};

bool bfs(std::vector<std::vector<size_t> > &g, std::vector<edge> &edges, std::vector<size_t> &dp) {
    dp.assign(dp.size(), dp.size());
    dp[0] = 0;
    std::queue<size_t> q;
    q.push(0);
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (size_t num : g[v]) {
            if (dp[edges[num].to] == dp.size() && edges[num].flow < edges[num].capacity) {
                dp[edges[num].to] = dp[v] + 1;
                q.push(edges[num].to);
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
        if (dp[e.to] == dp[v] + 1 && e.flow < e.capacity) {
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

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<size_t> > g(n);
    std::vector<edge> edges;
    edges.reserve(2 * m);
    while (m-- > 0) {
        size_t from, to;
        int32_t capacity;
        std::cin >> from >> to >> capacity;
        from--;
        to--;
        g[from].push_back(edges.size());
        edges.emplace_back(to, 0, capacity);
        g[to].push_back(edges.size());
        edges.emplace_back(from, 0, 0);
    }
    std::vector<size_t> dp(n), it(n);
    int64_t ans = 0;
    while (bfs(g, edges, dp)) {
        it.assign(n, 0);
        while (true) {
            int32_t add_flow = dfs(0, std::numeric_limits<int32_t>::max(), g, edges, dp, it);
            if (add_flow == 0) {
                break;
            }
            ans += add_flow;
        }
    }
    std::cout << ans << std::endl;
    return 0;
}
