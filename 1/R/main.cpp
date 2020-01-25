#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

bool is_ancestor(size_t v, size_t u, std::vector<size_t> &tin, std::vector<size_t> &tout) {
    return tin[v] <= tin[u] && tout[v] >= tout[u];
}

void dfs_first(size_t v, std::vector<std::vector<size_t> > &g,
               std::vector<size_t> &tin, std::vector<size_t> &tout, size_t &timer) {
    tin[v] = timer++;
    for (size_t &to : g[v]) {
        if (tin[to] == 0) {
            dfs_first(to, g, tin, tout, timer);
        }
    }
    tout[v] = timer++;
}

void dfs(size_t v, size_t p, std::vector<size_t> &par, std::vector<std::vector<size_t> > &g,
         std::vector<size_t> &cycle_from, std::vector<size_t> &cycle_to,
         std::vector<size_t> &tin, std::vector<size_t> &tout,
         std::vector<size_t> &h, size_t cur_h, std::ofstream &out) {
    h[v] = cur_h;
    par[v] = p;
    for (size_t &to : g[v]) {
        if (h[to] == 0) {
            dfs(to, v, par, g, cycle_from, cycle_to, tin, tout, h, cur_h + 1, out);
        } else if (to != p && h[to] < h[v]) {
            if ((h[v] - h[to]) % 2 == 1) {
                out << h[v] - h[to] + 1 << std::endl;
                while (v != to) {
                    out << v + 1 << " ";
                    v = par[v];
                }
                out << to + 1 << std::endl;
                throw std::runtime_error("Success");
            } else {
                size_t u = v;
                while (u != to && cycle_to[u] == g.size()) {
                    u = par[u];
                }
                if (u == to) {
                    u = v;
                    while (u != to) {
                        cycle_to[u] = to;
                        cycle_from[par[u]] = v;
                        u = par[u];
                    }
                } else {
                    size_t new_v = cycle_from[par[u]],
                            new_to = cycle_to[u];
                    std::vector<size_t> ans;
                    while (!is_ancestor(v, new_v, tin, tout)) {
                        ans.push_back(v + 1);
                        v = par[v];
                    }
                    ans.push_back(v + 1);
                    std::vector<size_t> reverse_ans;
                    while (v != new_v) {
                        reverse_ans.push_back(new_v + 1);
                        new_v = par[new_v];
                    }
                    std::for_each(reverse_ans.rbegin(), reverse_ans.rend(),
                                  [&ans](size_t &v) { ans.push_back(v); });
                    reverse_ans.clear();
                    ans.push_back(new_to + 1);
                    if (h[new_to] < h[to]) {
                        while (h[new_to] < h[to]) {
                            reverse_ans.push_back(to + 1);
                            to = par[to];
                        }
                        std::for_each(reverse_ans.rbegin(), reverse_ans.rend(),
                                      [&ans](size_t &v) { ans.push_back(v); });
                    } else {
                        while (h[new_to] > h[to]) {
                            new_to = par[new_to];
                            ans.push_back(new_to + 1);
                        }
                    }
                    out << ans.size() << std::endl;
                    std::for_each(ans.begin(), ans.end(), [&out](size_t &v) { out << v << " "; });
                    out << std::endl;
                    throw std::runtime_error("Success");
                }
            }
        }
    }
}

void solve(std::ifstream &in, std::ofstream &out) {
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
    size_t timer = 1;
    std::vector<size_t> tin(n, 0),
            tout(n);
    for (size_t i = 0; i < n; i++) {
        if (tin[i] == 0) {
            dfs_first(i, g, tin, tout, timer);
        }
    }
    std::vector<size_t> cycle_from(n, n),
            cycle_to(n, n),
            h(n, 0),
            par(n);
    for (size_t i = 0; i < n; i++) {
        if (h[i] == 0) {
            try {
                dfs(i, i, par, g, cycle_from, cycle_to, tin, tout, h, 1, out);
            } catch (std::exception &e) {
                return;
            }
        }
    }
    out << -1 << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("even.in");
    std::ofstream out("even.out");
    size_t T;
    in >> T;
    while (T-- > 0) {
        solve(in, out);
    }
    return 0;
}
