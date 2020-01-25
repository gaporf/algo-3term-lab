#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

static const int32_t INF = 1000000000;

struct edge {
    size_t to;
    int32_t flow, capacity, cost;

    edge(size_t to, int32_t flow, int32_t capacity, int32_t cost) : to(to),
                                                                    flow(flow),
                                                                    capacity(capacity),
                                                                    cost(cost) {}
};

void bellman_ford(std::vector<std::vector<size_t> > &g, std::vector<edge> &edges,
                  std::vector<int32_t> &p, std::vector<size_t> &from) {
    p[0] = 0;
    std::set<std::pair<int32_t, size_t> > st;
    st.insert({0, 0});
    while (!st.empty()) {
        size_t v = st.begin()->second;
        st.erase(st.begin());
        for (size_t num : g[v]) {
            size_t to = edges[num].to;
            int32_t dist = edges[num].cost;
            if (edges[num].flow < edges[num].capacity && p[to] > p[v] + dist) {
                if (p[to] != INF) {
                    st.erase(st.find({p[to], to}));
                }
                from[to] = v;
                p[to] = p[v] + dist;
                st.insert({p[to], to});
            }
        }
    }
}

int32_t dfs(size_t v, int32_t cur_flow, std::vector<std::vector<size_t> > &g,
            std::vector<edge> &edges, std::vector<int32_t> &p, std::vector<size_t> &from) {
    if (v == g.size() - 1 || cur_flow == 0) {
        return cur_flow;
    }
    for (size_t num : g[v]) {
        size_t to = edges[num].to;
        if (edges[num].flow < edges[num].capacity && from[to] == v) {
            int32_t add_flow = dfs(to, std::min(cur_flow, edges[num].capacity - edges[num].flow),
                                   g, edges, p, from);
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
              std::vector<int32_t> &p, std::vector<int32_t> &d, std::vector<size_t> &from) {
    d[0] = 0;
    std::set<std::pair<int32_t, size_t> > st;
    st.insert({0, 0});
    while (!st.empty()) {
        size_t v = st.begin()->second;
        st.erase(st.begin());
        for (size_t num : g[v]) {
            size_t to = edges[num].to;
            int32_t dist = edges[num].cost;
            if (edges[num].flow < edges[num].capacity && d[to] > d[v] + dist - p[to] + p[v]) {
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
    size_t n;
    std::cin >> n;
    std::vector<std::vector<size_t> > g(2 * n + 2);
    std::vector<edge> edges;
    for (size_t i = 1; i <= n; i++) {
        g[0].push_back(edges.size());
        edges.emplace_back(i, 0, 1, 0);
        g[i].push_back(edges.size());
        edges.emplace_back(0, 0, 0, 0);
    }
    for (size_t j = n + 1; j <= 2 * n; j++) {
        g[j].push_back(edges.size());
        edges.emplace_back(2 * n + 1, 0, 1, 0);
        g.back().push_back(edges.size());
        edges.emplace_back(j, 0, 0, 0);
    }
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = n + 1; j <= 2 * n; j++) {
            int32_t cost;
            std::cin >> cost;
            g[i].push_back(edges.size());
            edges.emplace_back(j, 0, 1, cost);
            g[j].push_back(edges.size());
            edges.emplace_back(i, 0, 0, -cost);
        }
    }
    std::vector<int32_t> p(2 * n + 2, INF);
    std::vector<size_t> from(2 * n + 2, 2 * n + 2);
    bellman_ford(g, edges, p, from);
    while (true) {
        dfs(0, INF, g, edges, p, from);
        std::vector<int32_t> d(2 * n + 2, INF);
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
    int32_t ans = 0;
    std::vector<std::pair<size_t, size_t> > ans_v;
    for (size_t i = 1; i <= n; i++) {
        for (size_t num : g[i]) {
            if (edges[num].flow == 1 && edges[num].to >= n + 1 && edges[num].to <= 2 * n) {
                ans += edges[num].cost;
                ans_v.emplace_back(i, edges[num].to - n);
            }
        }
    }
    std::cout << ans << std::endl;
    std::for_each(ans_v.begin(), ans_v.end(), [](auto v) {
        std::cout << v.first << " " << v.second << std::endl;
    });
    std::cout << std::endl;
    return 0;
}
