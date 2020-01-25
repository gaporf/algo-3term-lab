#include <iostream>
#include <vector>
#include <algorithm>

const size_t INF = 100000;

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<size_t, size_t> > > g(n);
    std::vector<std::vector<size_t> > dp(n, std::vector<size_t>(n, n * 10000));
    for (size_t i = 0; i < n; i++) {
        dp[i][i] = 0;
    }
    while (m-- > 0) {
        size_t from, to, dist;
        std::cin >> from >> to >> dist;
        from--;
        to--;
        dp[from][to] = std::min(dp[from][to], dist);
    }
    for (size_t i = 0; i < n; i++) {
        for (size_t u = 0; u < n; u++) {
            for (size_t v = 0; v < n; v++) {
                dp[u][v] = std::min(dp[u][v], dp[u][i] + dp[i][v]);
            }
        }
    }
    std::for_each(dp.begin(), dp.end(), [](std::vector<size_t> &dist) {
        std::for_each(dist.begin(), dist.end(), [](size_t &v) { std::cout << v << " "; });
        std::cout << std::endl;
    });
    return 0;
}
