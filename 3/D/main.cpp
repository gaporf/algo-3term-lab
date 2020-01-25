#include <iostream>
#include <vector>
#include <algorithm>

bool dfs(size_t v, std::vector<char> &visit,
         std::vector<std::vector<char> > &g, std::vector<size_t> &match) {
    if (visit[v]) {
        return false;
    }
    visit[v] = true;
    for (size_t to = 0; to < match.size(); to++) {
        if (g[v][to]) {
            if (match[to] == visit.size() || dfs(match[to], visit, g, match)) {
                match[to] = v;
                return true;
            }
        }
    }
    return false;
}

void dfs(size_t v, size_t part,
         std::vector<std::vector<char> > &g, std::vector<std::vector<char> > &gg,
         size_t n, size_t m, std::vector<char> &visit_first, std::vector<char> &visit_second) {
    if (part == 0) {
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

void solve() {
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<char> > g(n, std::vector<char>(m, false)),
            gg(m, std::vector<char>(n, false));
    for (size_t i = 0; i < n; i++) {
        size_t j;
        std::cin >> j;
        while (j > 0) {
            j--;
            g[i][j] = true;
            std::cin >> j;
        }
    }
    std::for_each(g.begin(), g.end(), [](std::vector<char> &v) {
        std::for_each(v.begin(), v.end(), [](char &v) {
            v = !v;
        });
    });
    std::vector<size_t> match(m, n);
    std::vector<char> visit(n);
    for (size_t i = 0; i < n; i++) {
        visit.assign(n, false);
        dfs(i, visit, g, match);
    }
    std::vector<char> is_match(n, false);
    for (size_t i = 0; i < m; i++) {
        if (match[i] != n) {
            is_match[match[i]] = true;
            g[match[i]][i] = false;
            gg[i][match[i]] = true;
        }
    }
    std::vector<char> visit_first(n, false),
            visit_second(m, false);
    for (size_t i = 0; i < n; i++) {
        if (!is_match[i] && !visit_first[i]) {
            dfs(i, 0, g, gg, n, m, visit_first, visit_second);
        }
    }
    size_t ans = 0,
        men = 0,
        women = 0;
    std::for_each(visit_first.begin(), visit_first.end(), [&ans, &men](char v) {
       if (v) {
           ans++;
           men++;
       }
    });
    std::for_each(visit_second.begin(), visit_second.end(), [&ans, &women](char v) {
       if (!v) {
           ans++;
           women++;
       }
    });
    std::cout << ans << std::endl << men << " " << women << std::endl;
    for (size_t i = 0; i < n; i++) {
        if (visit_first[i]) {
            std::cout << i + 1 << " ";
        }
    }
    std::cout << std::endl;
    for (size_t i = 0; i < m; i++) {
        if (!visit_second[i]) {
            std::cout << i + 1 << " ";
        }
    }
    std::cout << std::endl << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t t;
    std::cin >> t;
    while (t-- > 0) {
        solve();
    }
    return 0;
}
