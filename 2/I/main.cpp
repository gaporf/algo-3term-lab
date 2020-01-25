#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

const int32_t INF = 2000000000;

struct edge {
    size_t from, to;
    int32_t w;

    edge() = default;

    edge(size_t from, size_t to, int32_t w) : from(from), to(to), w(w) {}

    bool operator<(edge &rhs) {
        return w < rhs.w;
    }
};

void dfs(size_t v, std::vector<std::multiset<size_t> > &g, std::vector<char> &visit) {
    visit[v] = true;
    for (auto it = g[v].begin(); it != g[v].end(); it++) {
        size_t to = *it;
        if (!visit[to]) {
            dfs(to, g, visit);
        }
    }
}

bool check(std::vector<std::multiset<size_t> > &g, std::vector<char> &visit) {
    size_t n = g.size();
    visit.assign(n, false);
    dfs(0, g, visit);
    for (char v : visit) {
        if (!v) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<edge> edges(m);
    std::for_each(edges.begin(), edges.end(), [](edge &e) {
        std::cin >> e.from >> e.to >> e.w;
        e.from--;
        e.to--;
    });
    std::sort(edges.begin(), edges.end());
    std::vector<std::multiset<size_t> > g(n);
    std::vector<char> visit(n);
    int32_t ans = INF;
    for (size_t l = 0, r = 0; l < m; l++) {
        while (r < m && !check(g, visit)) {
            size_t from = edges[r].from,
                    to = edges[r].to;
            g[from].insert(to);
            g[to].insert(from);
            r++;
        }
        if (!check(g, visit)) {
            break;
        }
        ans = std::min(ans, edges[r - 1].w - edges[l].w);
        size_t from = edges[l].from,
                to = edges[l].to;
        g[from].erase(g[from].find(to));
        g[to].erase(g[to].find(from));
    }
    if (ans != INF) {
        std::cout << "YES" << std::endl;
        std::cout << ans << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}
