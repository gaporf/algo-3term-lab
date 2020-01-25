#include <iostream>
#include <vector>
#include <algorithm>

const int32_t INF = 1000000000;
const int32_t NO_EDGE = 1000000000;

struct edge {
    size_t from, to;
    int32_t dist;

    edge(size_t from, size_t to, int32_t dist) : from(from), to(to), dist(dist) {}
};

void solve(size_t s, std::vector<bool> &visit, size_t n, std::vector<edge> &edges) {
    std::vector<int32_t> dp(n, INF);
    std::vector<size_t> p(n, n);
    dp[s] = 0;
    for (size_t i = 0; i < n; i++) {
        for (edge &e : edges) {
            if (dp[e.from] != INF && dp[e.to] > dp[e.from] + e.dist) {
                dp[e.to] = dp[e.from] + e.dist;
                p[e.to] = e.from;
            }
        }
    }
    for (size_t i = 0; i < n; i++) {
        if (dp[i] != INF) {
            visit[i] = true;
        }
    }
    for (edge &e : edges) {
        if (dp[e.from] != INF && dp[e.to] > dp[e.from] + e.dist) {
            std::cout << "YES" << std::endl;
            size_t v = e.to;
            for (size_t i = 0; i < n; i++, v = p[v]) {}
            std::vector<size_t> path;
            for (size_t u = v; u != v || path.size() <= 1; u = p[u]) {
                path.push_back(u + 1);
            }
            path.push_back(v + 1);
            std::cout << path.size() << std::endl;
            std::for_each(path.rbegin(), path.rend(), [](size_t &v) { std::cout << v << " "; });
            std::cout << std::endl;
            throw std::runtime_error("Congrats");
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<edge> edges;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            int32_t v;
            std::cin >> v;
            if (v != NO_EDGE) {
                edges.emplace_back(i, j, v);
            }
        }
    }
    std::vector<bool> visit(n, false);
    for (size_t i = 0; i < n; i++) {
        if (!visit[i]) {
            try {
                solve(i, visit, n, edges);
            } catch (std::exception &e) {
                return 0;
            }
        }
    }
    std::cout << "NO" << std::endl;
    return 0;
}
