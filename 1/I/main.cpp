#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void dfs(size_t v, size_t p, std::vector<std::vector<size_t> > &g, std::vector<size_t> &tin,
         std::vector<size_t> &min_t, std::vector<size_t> &ans, size_t &timer) {
    tin[v] = min_t[v] = timer++;
    bool is_cut = false;
    for (size_t &to : g[v]) {
        if (tin[to] == 0) {
            dfs(to, v, g, tin, min_t, ans, timer);
            min_t[v] = std::min(min_t[v], min_t[to]);
            if (min_t[to] >= tin[v]) {
                is_cut = true;
            }
        } else if (to != p) {
            min_t[v] = std::min(min_t[v], tin[to]);
        }
    }
    if (is_cut) {
        ans.push_back(v + 1);
    }
}

void dfs(size_t v, std::vector<std::vector<size_t> > &g, std::vector<size_t> &tin,
         std::vector<size_t> &min_t, std::vector<size_t> &ans, size_t &timer) {
    size_t children = 0;
    tin[v] = min_t[v] = timer++;
    for (size_t &to : g[v]) {
        if (tin[to] == 0) {
            dfs(to, v, g, tin, min_t, ans, timer);
            children++;
        }
    }
    if (children > 1) {
        ans.push_back(v + 1);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("points.in");
    size_t n, m;
    in >> n >> m;
    std::vector<std::vector<size_t> > g(n);
    while (m-- > 0) {
        size_t from, to;
        in >> from >> to;
        from--;
        to--;
        g[from].push_back(to);
        g[to].push_back(from);
    }
    in.close();
    size_t timer = 1;
    std::vector<size_t> tin(n, 0),
            min_t(n),
            ans;
    for (size_t i = 0; i < n; i++) {
        if (tin[i] == 0) {
            dfs(i, g, tin, min_t, ans, timer);
        }
    }
    std::sort(ans.begin(), ans.end());
    std::ofstream out("points.out");
    out << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [&out](size_t &num) { out << num << " "; });
    out << std::endl;
    out.close();
    return 0;
}
