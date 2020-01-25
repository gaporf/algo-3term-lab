#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

static const int64_t INF = 1000000000000000000;

struct edge {
    size_t to;
    int64_t flow, capacity, cost;

    edge(size_t to, int64_t flow, int64_t capacity, int64_t cost) : to(to),
                                                                    flow(flow),
                                                                    capacity(capacity),
                                                                    cost(cost) {}
};

void bellman_ford(std::vector<std::vector<size_t> > &g, std::vector<edge> &edges,
                  std::vector<int64_t> &p, std::vector<size_t> &from) {
    p[0] = 0;
    for (size_t k = 0; k < g.size(); k++) {
        for (size_t v = 0; v < g.size(); v++) {
            for (size_t num : g[v]) {
                if (edges[num].flow < edges[num].capacity) {
                    size_t to = edges[num].to;
                    int64_t dist = edges[num].cost;
                    if (p[v] != INF && p[to] > p[v] + dist) {
                        p[to] = p[v] + dist;
                        from[to] = v;
                    }
                }
            }
        }
    }
}

int64_t dfs(size_t v, int64_t cur_flow, std::vector<std::vector<size_t> > &g,
            std::vector<edge> &edges, std::vector<int64_t> &p, std::vector<size_t> &from) {
    if (v == g.size() - 1 || cur_flow == 0) {
        return cur_flow;
    }
    for (size_t num : g[v]) {
        size_t to = edges[num].to;
        int64_t dist = edges[num].cost;
        if (edges[num].flow < edges[num].capacity && from[to] == v && p[to] == p[v] + dist) {
            int64_t add_flow = dfs(to, std::min(cur_flow, edges[num].capacity - edges[num].flow), g,
                                   edges, p, from);
            if (add_flow > 0) {
                edges[num].flow += add_flow;
                edges[num ^ 1].flow -= add_flow;
                return add_flow;
            }
        }
    }
    return 0;
}

void dijkstra(std::vector<std::vector<size_t> > &g, std::vector<edge> &edges,
              std::vector<int64_t> &p, std::vector<int64_t> &d, std::vector<size_t> &from) {
    d[0] = 0;
    std::set<std::pair<int64_t, size_t> > st;
    st.insert({0, 0});
    while (!st.empty()) {
        size_t v = st.begin()->second;
        st.erase(st.begin());
        for (size_t num : g[v]) {
            size_t to = edges[num].to;
            int64_t dist = edges[num].cost;
            if (edges[num].flow < edges[num].capacity && p[to] != INF &&
                d[to] > d[v] + dist - p[to] + p[v]) {
                if (d[to] != INF) {
                    st.erase(st.find({d[to], to}));
                }
                d[to] = d[v] + dist - p[to] + p[v];
                from[to] = v;
                st.insert({d[to], to});
            }
        }
    }
}

bool dfs(size_t v, std::vector<char> &visit, std::vector<std::vector<size_t> > &g,
         std::vector<size_t> &match) {
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
    size_t n, k;
    std::cin >> n >> k;
    std::vector<std::vector<size_t> > g(2 * n + 2);
    std::vector<edge> edges;
    for (size_t i = 1; i <= n; i++) {
        g[0].push_back(edges.size());
        edges.emplace_back(i, 0, k, 0);
        g[i].push_back(edges.size());
        edges.emplace_back(0, 0, 0, 0);
    }
    for (size_t j = n + 1; j <= 2 * n; j++) {
        g[j].push_back(edges.size());
        edges.emplace_back(2 * n + 1, 0, k, 0);
        g.back().push_back(edges.size());
        edges.emplace_back(j, 0, 0, 0);
    }
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = n + 1; j <= 2 * n; j++) {
            int64_t cost;
            std::cin >> cost;
            g[i].push_back(edges.size());
            edges.emplace_back(j, 0, 1, cost);
            g[j].push_back(edges.size());
            edges.emplace_back(i, 0, 0, -cost);
        }
    }
    std::vector<int64_t> p(2 * n + 2, INF);
    std::vector<size_t> from(2 * n + 2, 2 * n + 2);
    bellman_ford(g, edges, p, from);
    while (true) {
        dfs(0, INF, g, edges, p, from);
        std::vector<int64_t> d(2 * n + 2, INF);
        from.assign(2 * n + 2, 2 * n + 2);
        dijkstra(g, edges, p, d, from);
        if (d.back() == INF) {
            break;
        }
        for (size_t i = 0; i < 2 * n + 2; i++) {
            if (d[i] != INF) {
                p[i] += d[i];
            } else {
                p[i] = INF;
            }
        }
    }
    int64_t ans = 0;
    for (edge &e : edges) {
        if (e.flow > 0) {
            ans += e.flow * e.cost;
        }
    }
    std::vector<std::vector<size_t> > f_g(n);
    for (size_t i = 1; i <= n; i++) {
        for (size_t num : g[i]) {
            size_t to = edges[num].to;
            if (to >= n + 1 && to <= 2 * n && edges[num].flow == 1) {
                f_g[i - 1].push_back(to - n - 1);
            }
        }
    }
    std::cout << ans << std::endl;
    while (k-- > 0) {
        std::vector<size_t> match(n, n);
        for (size_t i = 0; i < n; i++) {
            std::vector<char> visit(n, false);
            dfs(i, visit, f_g, match);
        }
        std::vector<size_t> cur_ans(n);
        for (size_t i = 0; i < n; i++) {
            cur_ans[match[i]] = i;
        }
        for (size_t i = 0; i < n; i++) {
            std::cout << cur_ans[i] + 1 << " ";
            f_g[i].erase(std::find(f_g[i].begin(), f_g[i].end(), cur_ans[i]));
        }
        std::cout << std::endl;
    }
    return 0;
}
