#include <iostream>
#include <vector>

int32_t INF = 1000000000;

void
dfs(size_t v, std::vector<std::vector<size_t> > &g, std::vector<size_t> &p, std::vector<size_t> &dp,
    std::vector<std::vector<int32_t> > &f, std::vector<std::vector<int32_t> > &c) {
    if (v == g.size() - 1) {
        return;
    }
    for (size_t to : g[v]) {
        if (dp[to] > dp[v] + 1 && c[v][to] > f[v][to]) {
            p[to] = v;
            dp[to] = dp[v] + 1;
            dfs(to, g, p, dp, f, c);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int32_t> > f(n, std::vector<int32_t>(n, 0)),
            c(n, std::vector<int32_t>(n, 0));
    std::vector<std::vector<size_t> > g(n);
    while (m--) {
        size_t from, to;
        int32_t capacity;
        std::cin >> from >> to >> capacity;
        from--;
        to--;
        c[from][to] += capacity;
        g[from].push_back(to);
        g[to].push_back(from);
    }
    std::vector<size_t> p(n, n);
    while (true) {
        std::vector<size_t> dp(n, n);
        dp[0] = 0;
        dfs(0, g, p, dp, f, c);
        if (p.back() == g.size()) {
            break;
        }
        int32_t cur_flow = INF;
        for (size_t u = n - 1; u != 0; u = p[u]) {
            size_t prev = p[u];
            cur_flow = std::min(cur_flow, c[prev][u] - f[prev][u]);
        }
        for (size_t u = n - 1; u != 0; u = p[u]) {
            size_t prev = p[u];
            f[prev][u] += cur_flow;
            f[u][prev] -= cur_flow;
        }
        p.assign(n, n);
    }
    int32_t ans = 0;
    for (size_t i = 0; i < n; i++) {
        ans += f[0][i];
    }
    std::cout << ans << std::endl;
    return 0;
}
