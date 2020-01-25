#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<size_t> > g(n);
    while (m-- > 0) {
        size_t from, to;
        std::cin >> from >> to;
        from--;
        to--;
        g[from].push_back(to);
        g[to].push_back(from);
    }
    std::vector<size_t> dp(n, n);
    dp[0] = 0;
    std::queue<size_t> q;
    q.push(0);
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (size_t &to : g[v]) {
            if (dp[to] == n) {
                dp[to] = dp[v] + 1;
                q.push(to);
            }
        }
    }
    std::for_each(dp.begin(), dp.end(), [](size_t &v) { std::cout << v << " "; });
    std::cout << std::endl;
    return 0;
}
