#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void dfs(size_t v, size_t p, std::vector<std::vector<std::pair<size_t, size_t> > > &g,
         std::vector<size_t> &tin, std::vector<size_t> &min_t, size_t &timer) {
    tin[v] = min_t[v] = timer++;
    for (std::pair<size_t, size_t> &to : g[v]) {
        if (tin[to.first] == 0) {
            dfs(to.first, v, g, tin, min_t, timer);
            min_t[v] = std::min(min_t[v], min_t[to.first]);
        } else if (to.first != p) {
            min_t[v] = std::min(min_t[v], tin[to.first]);
        }
    }
}

void dfs(size_t v, std::vector<std::vector<std::pair<size_t, size_t> > > &g,
         std::vector<size_t> &tin, std::vector<size_t> &min_t, size_t &timer) {
    tin[v] = min_t[v] = timer++;
    for (std::pair<size_t, size_t> &to : g[v]) {
        if (tin[to.first] == 0) {
            dfs(to.first, v, g, tin, min_t, timer);
        }
    }
}

void another_dfs(size_t v, size_t p, std::vector<std::vector<std::pair<size_t, size_t> > > &g,
                 std::vector<size_t> &tin, std::vector<size_t> &min_t, std::vector<bool> &visit,
                 std::vector<size_t> &ans, size_t cur, size_t &comp) {
    visit[v] = true;
    for (std::pair<size_t, size_t> &to : g[v]) {
        if (to.first == p) {
            continue;
        }
        if (!visit[to.first]) {
            if (min_t[to.first] >= tin[v]) {
                size_t new_cur = comp++;
                ans[to.second] = new_cur;
                another_dfs(to.first, v, g, tin, min_t, visit, ans, new_cur, comp);
            } else {
                ans[to.second] = cur;
                another_dfs(to.first, v, g, tin, min_t, visit, ans, cur, comp);
            }
        } else if (tin[to.first] < tin[v]) {
            ans[to.second] = cur;
        }
    }
}

void another_dfs(size_t v, std::vector<std::vector<std::pair<size_t, size_t> > > &g,
                 std::vector<size_t> &tin, std::vector<size_t> &min_t, std::vector<bool> &visit,
                 std::vector<size_t> &ans, size_t &comp) {
    visit[v] = true;
    for (std::pair<size_t, size_t> &to : g[v]) {
        if (!visit[to.first]) {
            size_t cur = comp++;
            ans[to.second] = cur;
            another_dfs(to.first, v, g, tin, min_t, visit, ans, cur, comp);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("biconv.in");
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
    size_t timer = 1,
            comp = 1;
    std::vector<size_t> tin(n, 0),
            min_t(n),
            ans(m);
    for (size_t i = 0; i < n; i++) {
        if (tin[i] == 0) {
            dfs(i, g, tin, min_t, timer);
        }
    }
    std::vector<bool> visit(n, false);
    for (size_t i = 0; i < n; i++) {
        if (!visit[i]) {
            another_dfs(i, g, tin, min_t, visit, ans, comp);
        }
    }
    std::ofstream out("biconv.out");
    out << comp - 1 << std::endl;
    std::for_each(ans.begin(), ans.end(), [&out](size_t &num) { out << num << " "; });
    out << std::endl;
    out.close();
    return 0;
}
