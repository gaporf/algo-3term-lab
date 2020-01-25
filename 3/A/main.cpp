#include <iostream>
#include <vector>
#include <algorithm>

bool dfs(size_t v, std::vector<char> &visit,
         std::vector<std::vector<size_t> > &g, std::vector<size_t> &match) {
    if (visit[v]) {
        return false;
    }
    visit[v] = true;
    for (size_t to : g[v]) {
        if (match[to] == g.size() || dfs(match[to], visit, g, match)) {
            match[to] = v;
            return true;
        }
    }
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m, k;
    std::cin >> n >> m >> k;
    std::vector<std::vector<size_t> > f_g(n);
    while (k-- > 0) {
        size_t from, to;
        std::cin >> from >> to;
        from--;
        to--;
        f_g[from].push_back(to);
    }
    std::vector<size_t> match(m, n);
    std::vector<char> visit(n, false);
    for (size_t i = 0; i < n; i++) {
        visit.assign(n, false);
        dfs(i, visit, f_g, match);
    }
    size_t ans = 0;
    std::for_each(match.begin(), match.end(), [&ans, n](size_t m) {
        if (m != n) {
            ans++;
        }
    });
    std::cout << ans << std::endl;
    return 0;
}
