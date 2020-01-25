#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

struct edge {
    size_t from, to;
    int64_t dist, cur_dist;

    edge(size_t from, size_t to, int64_t dist) : from(from), to(to), dist(dist), cur_dist(dist) {}

    bool operator<(edge const &rhs) const {
        if (cur_dist != rhs.cur_dist) {
            return cur_dist < rhs.cur_dist;
        } else if (dist != rhs.dist) {
            return dist < rhs.dist;
        } else if (from != rhs.from) {
            return from < rhs.from;
        } else {
            return to < rhs.to;
        }
    }
};

size_t get_next(size_t v, std::vector<size_t> &comp) {
    if (comp[v] == v) {
        return v;
    } else {
        return comp[v] = get_next(comp[v], comp);
    }
}

void dfs(size_t v, std::vector<std::set<edge> > &g_t,
         std::vector<char> &cl, std::vector<size_t> &p, std::vector<int64_t> &minus,
         std::vector<size_t> &comp, std::vector<size_t> &rank, int64_t &ans) {
    size_t old_v = v;
    cl[old_v] = 'g';
    for (;;) {
        if (g_t[v].empty()) {
            throw std::runtime_error("No way to 1");
        }
        auto cur_edge = *g_t[v].begin();
        g_t[v].erase(g_t[v].begin());
        size_t to = get_next(cur_edge.from, comp);
        if (v == to) {
            continue;
        }
        ans += cur_edge.cur_dist - minus[v];
        minus[v] = cur_edge.cur_dist;
        if (cl[to] == 'w') {
            p[to] = v;
            dfs(to, g_t, cl, p, minus, comp, rank, ans);
            break;
        } else if (cl[to] == 'g') {
            std::vector<size_t> path;
            path.push_back(v);
            size_t u = v,
                    best_u = v;
            while (u != to) {
                u = p[u];
                if (g_t[u].size() > g_t[best_u].size()) {
                    best_u = u;
                }
                path.push_back(u);
            }
            for (size_t cur_v : path) {
                if (cur_v != best_u) {
                    while (!g_t[cur_v].empty()) {
                        edge old_edge = *g_t[cur_v].begin();
                        g_t[cur_v].erase(g_t[cur_v].begin());
                        old_edge.cur_dist -= minus[cur_v] - minus[best_u];
                        g_t[best_u].insert(old_edge);
                    }
                }
            }
            for (size_t cur_v : path) {
                rank[cur_v] = 0;
                if (cur_v != best_u) {
                    comp[cur_v] = best_u;
                }
            }
            v = best_u;
            p[v] = p[to];
        } else {
            break;
        }
    }
    cl[old_v] = 'b';
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::set<edge> > g_t(n);
    while (m-- > 0) {
        size_t from, to;
        int64_t dist;
        std::cin >> from >> to >> dist;
        from--;
        to--;
        g_t[to].insert(edge(from, to, dist));
    }
    int64_t ans = 0;
    std::vector<size_t> comp(n),
            rank(n, 0),
            p(n, n);
    for (size_t i = 0; i < n; i++) {
        comp[i] = i;
    }
    std::vector<char> cl(n, 'w');
    std::vector<int64_t> minus(n);
    cl[0] = 'b';
    for (size_t i = 1; i < n; i++) {
        if (cl[i] == 'w') {
            try {
                dfs(i, g_t, cl, p, minus, comp, rank, ans);
            } catch (std::exception &e) {
                std::cout << "NO" << std::endl;
                return 0;
            }
        }
    }
    std::cout << "YES" << std::endl;
    std::cout << ans << std::endl;
    return 0;
}
