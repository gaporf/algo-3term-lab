#include <iostream>
#include <vector>
#include <queue>
#include <limits>

static const int32_t INF = std::numeric_limits<int32_t>::max();

void dfs_first(size_t v, std::vector<std::vector<std::pair<size_t, int32_t> > > &g,
               std::vector<size_t> &income, std::vector<char> &cl) {
    cl[v] = 'b';
    for (std::pair<size_t, int32_t> &to : g[v]) {
        income[to.first]++;
        if (cl[to.first] == 'w') {
            dfs_first(to.first, g, income, cl);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m, s, t;
    std::cin >> n >> m >> s >> t;
    --s;
    --t;
    std::vector<std::vector<std::pair<size_t, int32_t> > > g(n);
    while (m-- > 0) {
        size_t from, to;
        int32_t dist;
        std::cin >> from >> to >> dist;
        g[--from].emplace_back(--to, dist);
    }
    std::vector<size_t> income(n, 0);
    std::vector<char> cl(n, 'w');
    dfs_first(s, g, income, cl);
    std::queue<size_t> q;
    q.push(s);
    std::vector<int32_t> dp(n, INF);
    dp[s] = 0;
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (std::pair<size_t, int32_t> &to : g[v]) {
            income[to.first]--;
            if (income[to.first] == 0) {
                q.push(to.first);
            }
            dp[to.first] = std::min(dp[to.first], dp[v] + to.second);
        }
    }
    if (dp[t] == INF) {
        std::cout << "Unreachable" << std::endl;
    } else {
        std::cout << dp[t] << std::endl;
    }
    return 0;
}
