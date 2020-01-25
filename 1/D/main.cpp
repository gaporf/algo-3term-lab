#include <iostream>
#include <vector>
#include <queue>

void dfs(size_t v, std::vector<std::vector<size_t> > &g,
         std::vector<size_t> &outcomes, std::vector<char> &cl) {
    cl[v] = 'b';
    for (size_t &to : g[v]) {
        outcomes[v]++;
        if (cl[to] == 'w') {
            dfs(to, g, outcomes, cl);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m, s;
    std::cin >> n >> m >> s;
    --s;
    std::vector<std::vector<size_t> > g(n), g_t(n);
    while (m-- > 0) {
        size_t from, to;
        std::cin >> from >> to;
        --from;
        --to;
        g[from].push_back(to);
        g_t[to].push_back(from);
    }
    std::vector<size_t> outcomes(n, 0);
    std::vector<char> cl(n, 'w');
    dfs(s, g, outcomes, cl);
    std::queue<size_t> q;
    std::vector<char> dp(n, 'l');
    for (size_t i = 0; i < n; i++) {
        if (cl[i] == 'b' && outcomes[i] == 0) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (auto &from : g_t[v]) {
            if (--outcomes[from] == 0) {
                q.push(from);
            }
            if (dp[v] == 'l') {
                dp[from] = 'w';
            }
        }
    }
    std::cout << ((dp[s] == 'w') ? "First" : "Second") << " player wins" << std::endl;
    return 0;
}
