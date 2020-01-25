#include <iostream>
#include <vector>
#include <set>

static const int64_t INF = std::numeric_limits<int64_t>::max();

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
            if (edges[num].flow < edges[num].capacity && p[to] != INF
                && d[to] > d[v] + dist - p[to] + p[v]) {
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

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<edge> edges;
    std::vector<std::vector<size_t> > g(n);
    while (m-- > 0) {
        size_t from, to;
        int64_t capacity, cost;
        std::cin >> from >> to >> capacity >> cost;
        from--;
        to--;
        g[from].push_back(edges.size());
        edges.emplace_back(to, 0, capacity, cost);
        g[to].push_back(edges.size());
        edges.emplace_back(from, 0, 0, -cost);
    }
    std::vector<int64_t> p(n, INF);
    std::vector<size_t> from(n, n);
    bellman_ford(g, edges, p, from);
    while (true) {
        dfs(0, INF, g, edges, p, from);
        std::vector<int64_t> d(n, INF);
        from.assign(n, n);
        dijkstra(g, edges, p, d, from);
        if (d.back() == INF) {
            break;
        }
        for (size_t i = 0; i < n; i++) {
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
    std::cout << ans << std::endl;
    return 0;
}
