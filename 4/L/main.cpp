#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

static int64_t const INF = 1000000000000000000;

struct task {
    size_t num, s, t;
    int64_t c;

    task() = default;

    task(size_t num, size_t s, size_t t, int64_t c) : num(num), s(s), t(t), c(c) {}

    bool operator<(task const &rhs) {
        return s < rhs.s;
    }
};

struct edge {
    size_t to;
    int64_t flow, capacity, cost;

    edge(size_t to, int64_t flow, int64_t capacity, int64_t cost) : to(to), flow(flow),
                                                                    capacity(capacity),
                                                                    cost(cost) {}
};

void bellman_ford(std::vector<std::vector<size_t> > &g, std::vector<edge> &edges,
                  std::vector<int64_t> &d, std::vector<size_t> &from) {
    d[0] = 0;
    for (size_t k = 0; k < g.size(); k++) {
        for (size_t v = 0; v < g.size(); v++) {
            for (size_t num : g[v]) {
                if (edges[num].flow < edges[num].capacity) {
                    size_t to = edges[num].to;
                    int64_t dist = edges[num].cost;
                    if (d[v] != INF && d[to] > d[v] + dist) {
                        d[to] = d[v] + dist;
                        from[to] = v;
                    }
                }
            }
        }
    }
}

int64_t dfs(size_t v, size_t t, int64_t cur_flow, std::vector<std::vector<size_t> > &g,
            std::vector<edge> &edges, std::vector<int64_t> &d, std::vector<size_t> &from) {
    if (v == t || cur_flow == 0) {
        return cur_flow;
    }
    for (size_t num : g[v]) {
        size_t to = edges[num].to;
        int64_t dist = edges[num].cost;
        if (edges[num].flow < edges[num].capacity && from[to] == v && d[to] == d[v] + dist) {
            int64_t add_flow = dfs(to, t, std::min(cur_flow, edges[num].capacity - edges[num].flow),
                                   g, edges, d, from);
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

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    int64_t k;
    std::cin >> n >> k;
    std::vector<task> tasks(n);
    for (size_t i = 0; i < n; i++) {
        tasks[i].num = i;
        std::cin >> tasks[i].s >> tasks[i].t >> tasks[i].c;
    }
    std::sort(tasks.begin(), tasks.end());
    std::vector<edge> edges;
    std::vector<std::vector<size_t> > g(2 * n + 2);
    g[0].push_back(edges.size());
    edges.emplace_back(1, 0, k, 0);
    g[1].push_back(edges.size());
    edges.emplace_back(0, 0, 0, 0);
    for (size_t i = 1; i <= n; i++) {
        g[i].push_back(edges.size());
        edges.emplace_back(i + 1, 0, k, 0);
        g[i + 1].push_back(edges.size());
        edges.emplace_back(i, 0, 0, 0);
        g[i].push_back(edges.size());
        edges.emplace_back(i + n + 1, 0, 1, -tasks[i - 1].c);
        g[i + n + 1].push_back(edges.size());
        edges.emplace_back(i, 0, 0, tasks[i - 1].c);
        for (size_t j = i + 1; j <= n + 1; j++) {
            if (j == n + 1 || tasks[j - 1].s >= tasks[i - 1].s + tasks[i - 1].t) {
                g[i + n + 1].push_back(edges.size());
                edges.emplace_back(j, 0, 1, 0);
                g[j].push_back(edges.size());
                edges.emplace_back(i + n + 1, 0, 0, 0);
                break;
            }
        }
    }
    size_t t = n + 1;
    std::vector<int64_t> p(2 * n + 2, INF);
    std::vector<size_t> from(2 * n + 2, 2 * n + 2);
    bellman_ford(g, edges, p, from);
    while (true) {
        dfs(0, t, INF, g, edges, p, from);
        std::vector<int64_t> dist(2 * n + 2, INF);
        from.assign(2 * n + 2, 2 * n + 2);
        dijkstra(g, edges, p, dist, from);
        if (dist[t] == INF) {
            break;
        }
        for (size_t i = 0; i < 2 * n + 2; i++) {
            if (dist[i] != INF) {
                p[i] += dist[i];
            } else {
                p[i] = INF;
            }
        }
    }
    std::vector<int64_t> ans(n);
    for (size_t i = 1; i <= n; i++) {
        for (size_t num : g[i]) {
            if (edges[num].capacity == 1 && edges[num].to >= n + 2) {
                ans[tasks[i - 1].num] = edges[num].flow;
            }
        }
    }
    std::for_each(ans.begin(), ans.end(), [](int64_t v) { std::cout << v << " "; });
    std::cout << std::endl;
    return 0;
}
