#include <iostream>
#include <vector>
#include <algorithm>

void dfs(size_t v, size_t part,
        std::vector<std::vector<char> > &g, std::vector<std::vector<char> > &gg,
        size_t n, size_t m, std::vector<char> &visit_first, std::vector<char> &visit_second) {
    if (!part) {
        visit_first[v] = true;
        for (size_t i = 0; i < m; i++) {
            if (g[v][i] && !visit_second[i]) {
                dfs(i, 1, g, gg, n, m, visit_first, visit_second);
            }
        }
    } else {
        visit_second[v] = true;
        for (size_t i = 0; i < n; i++) {
            if (gg[v][i] && !visit_first[i]) {
                dfs(i, 0, g, gg, n, m, visit_first, visit_second);
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<char> > g(n, std::vector<char>(m, false)),
        gg(m, std::vector<char>(n, false));
    for (size_t i = 0; i < n; i++) {
        size_t k;
        std::cin >> k;
        while (k-- > 0) {
            size_t j;
            std::cin >> j;
            j--;
            g[i][j] = true;
        }
    }
    size_t ans = 0;
    std::vector<char> is_match(n, false);
    for (size_t i = 0; i < n; i++) {
        size_t j;
        std::cin >> j;
        if (j != 0) {
            is_match[i] = true;
            ans++;
            j--;
            g[i][j] = false;
            gg[j][i] = true;
        }
    }
    std::cout << ans << std::endl;
    std::vector<char> visit_first(n, false),
            visit_second(m, false);
    for (size_t i = 0; i < n; i++) {
        if (!is_match[i] && !visit_first[i]) {
            dfs(i, 0, g, gg, n, m, visit_first, visit_second);
        }
    }
    ans = 0;
    std::for_each(visit_first.begin(), visit_first.end(), [&ans](char v) {
        if (!v) {
            ans++;
        }
    });
    std::cout << ans << " ";
    for (size_t i = 0; i < n; i++) {
        if (!visit_first[i]) {
            std::cout << i + 1 << " ";
        }
    }
    std::cout << std::endl;
    ans = 0;
    std::for_each(visit_second.begin(), visit_second.end(), [&ans](char v) {
        if (v) {
            ans++;
        }
    });
    std::cout << ans << " ";
    for (size_t i = 0; i < m; i++) {
        if (visit_second[i]) {
            std::cout << i + 1 << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
