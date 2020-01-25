#include <iostream>
#include <vector>

void dfs(size_t v, std::vector<std::vector<size_t> > &g,
         std::vector<char> &cl, std::vector<size_t> &ans) {
    cl[v] = 'g';
    for (size_t i = 0; i < g[v].size(); i++) {
        size_t to = g[v][i];
        if (cl[to] == 'w') {
            dfs(to, g, cl, ans);
        } else if (cl[to] == 'g') {
            throw std::runtime_error("Found cycle");
        }
    }
    ans.push_back(v + 1);
    cl[v] = 'b';
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<size_t> > g(n);
    while (m-- > 0) {
        size_t from, to;
        std::cin >> from >> to;
        g[--from].push_back(--to);
    }
    std::vector<char> cl(n, 'w');
    std::vector<size_t> ans;
    for (size_t i = 0; i < n; i++) {
        if (cl[i] == 'w') {
            try {
                dfs(i, g, cl, ans);
            } catch (std::exception &e) {
                std::cout << "-1";
                return 0;
            }
        }
    }
    for (size_t i = n; i > 0; i--) {
        std::cout << ans[i - 1] << " ";
    }
    std::cout << std::endl;
    return 0;
}
