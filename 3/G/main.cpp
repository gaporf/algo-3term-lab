#include <iostream>
#include <vector>
#include <algorithm>

void dfs(size_t v, std::vector<std::vector<size_t> > &g, std::vector<size_t> &ans) {
    for (size_t i = 0; i < g[v].size(); i++) {
        if (g[v][i] != 0) {
            g[v][i]--;
            g[i][v]--;
            dfs(i, g, ans);
        }
    }
    ans.push_back(v + 1);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<size_t> d(n);
    std::vector<std::vector<size_t> > g(n, std::vector<size_t>(n, 0));
    size_t st = 0;
    for (size_t i = 0; i < n; i++) {
        size_t k;
        std::cin >> k;
        d[i] = k;
        if (k % 2 == 1) {
            st = i;
        }
        while (k-- > 0) {
            size_t j;
            std::cin >> j;
            j--;
            g[i][j]++;
        }
    }
    std::vector<size_t> ans;
    dfs(st, g, ans);
    std::cout << ans.size() - 1 << std::endl;
    std::for_each(ans.rbegin(), ans.rend(), [](size_t v) { std::cout << v << " "; });
    std::cout << std::endl;
    return 0;
}
