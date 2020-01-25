#include <iostream>
#include <vector>
#include <algorithm>

const int64_t INF = 3000000000000000000;
const int64_t MIN_INF = -INF;

struct edge {
    size_t from, to;
    int64_t dist;
};

void dfs(size_t v, std::vector<std::vector<size_t> > &g, std::vector<bool> &visit) {
    visit[v] = true;
    for (size_t &to : g[v]) {
        if (!visit[to]) {
            dfs(to, g, visit);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m, s;
    std::cin >> n >> m >> s;
    s--;
    std::vector<edge> edges(m);
    std::vector<std::vector<size_t> > g(n);
    std::for_each(edges.begin(), edges.end(), [&g](edge &e) {
        std::cin >> e.from >> e.to >> e.dist;
        e.from--;
        e.to--;
        g[e.from].push_back(e.to);
    });
    std::vector<int64_t> dp(n, INF);
    dp[s] = 0;
    for (size_t i = 0; i < n - 1; i++) {
        for (edge &e : edges) {
            if (dp[e.from] != INF) {
                dp[e.to] = std::min(dp[e.to], dp[e.from] + e.dist);
                dp[e.to] = std::max(dp[e.to], MIN_INF);
            }
        }
    }
    std::vector<bool> visit(n, false);
    for (edge &e : edges) {
        if (dp[e.from] != INF && dp[e.to] > dp[e.from] + e.dist && !visit[e.to]) {
            dfs(e.to, g, visit);
        }
    }
    for (size_t i = 0; i < n; i++) {
        if (dp[i] == INF) {
            std::cout << '*' << std::endl;
        } else if (visit[i]) {
            std::cout << '-' << std::endl;
        } else {
            std::cout << dp[i] << std::endl;
        }
    }
    return 0;
}
