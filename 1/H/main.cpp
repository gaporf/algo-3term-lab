#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void
dfs(size_t v, size_t p, std::vector<std::vector<std::pair<size_t, size_t> > > &g,
    std::vector<size_t> &tin, std::vector<size_t> &min_t, std::vector<size_t> &ans, size_t &timer) {
    tin[v] = min_t[v] = timer++;
    for (std::pair<size_t, size_t> &to : g[v]) {
        if (tin[to.first] == 0) {
            dfs(to.first, v, g, tin, min_t, ans, timer);
            min_t[v] = std::min(min_t[v], min_t[to.first]);
            if (min_t[to.first] == tin[to.first]) {
                ans.push_back(to.second);
            }
        } else if (to.first != p) {
            min_t[v] = std::min(min_t[v], tin[to.first]);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("bridges.in");
    size_t n, m;
    in >> n >> m;
    std::vector<std::vector<std::pair<size_t, size_t> > > g(n);
    for (size_t i = 1; i <= m; i++) {
        size_t from, to;
        in >> from >> to;
        from--;
        to--;
        g[from].emplace_back(to, i);
        g[to].emplace_back(from, i);
    }
    in.close();
    size_t timer = 1;
    std::vector<size_t> tin(n, 0),
            min_t(n),
            ans;
    for (size_t i = 0; i < n; i++) {
        if (tin[i] == 0) {
            dfs(i, i, g, tin, min_t, ans, timer);
        }
    }
    std::ofstream out("bridges.out");
    out << ans.size() << std::endl;
    std::sort(ans.begin(), ans.end());
    std::for_each(ans.begin(), ans.end(), [&out](size_t &num) { out << num << " "; });
    out.close();
    return 0;
}
