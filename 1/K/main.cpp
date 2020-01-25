#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void dfs(size_t v, size_t p, std::vector<std::vector<std::pair<size_t, size_t> > > &g,
         std::vector<size_t> &tin, std::vector<size_t> &min_t,
         std::vector<bool> &is_bridge, size_t &timer) {
    tin[v] = min_t[v] = timer++;
    for (std::pair<size_t, size_t> &to : g[v]) {
        if (tin[to.first] == 0) {
            dfs(to.first, v, g, tin, min_t, is_bridge, timer);
            min_t[v] = std::min(min_t[v], min_t[to.first]);
            if (min_t[to.first] == tin[to.first]) {
                is_bridge[to.second] = true;
            }
        } else if (to.first != p) {
            min_t[v] = std::min(min_t[v], tin[to.first]);
        }
    }
}

void another_dfs(size_t v, std::vector<std::vector<std::pair<size_t, size_t> > > &g,
                 std::vector<bool> &visit, std::vector<bool> &is_bridge,
                 std::vector<size_t> &ans, size_t comp) {
    visit[v] = true;
    ans[v] = comp;
    for (std::pair<size_t, size_t> &to : g[v]) {
        if (!is_bridge[to.second] && !visit[to.first]) {
            another_dfs(to.first, g, visit, is_bridge, ans, comp);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("bicone.in");
    size_t n, m;
    in >> n >> m;
    std::vector<std::vector<std::pair<size_t, size_t> > > g(n);
    for (size_t i = 0; i < m; i++) {
        size_t from, to;
        in >> from >> to;
        from--;
        to--;
        g[from].emplace_back(to, i);
        g[to].emplace_back(from, i);
    }
    in.close();
    std::vector<bool> is_bridge(m, false);
    size_t timer = 1;
    std::vector<size_t> tin(n, 0),
            min_t(n);
    for (size_t i = 0; i < n; i++) {
        if (tin[i] == 0) {
            dfs(i, i, g, tin, min_t, is_bridge, timer);
        }
    }
    std::vector<size_t> ans(n);
    std::vector<bool> visit(n, false);
    size_t comp = 1;
    for (size_t i = 0; i < n; i++) {
        if (!visit[i]) {
            another_dfs(i, g, visit, is_bridge, ans, comp++);
        }
    }
    std::ofstream out("bicone.out");
    out << comp - 1 << std::endl;
    std::for_each(ans.begin(), ans.end(), [&out](size_t &num) { out << num << " "; });
    out << std::endl;
    out.close();
    return 0;
}
