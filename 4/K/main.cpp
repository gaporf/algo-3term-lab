#include <iostream>
#include <vector>
#include <algorithm>

static int32_t const INF = 2000000000;

struct edge {
    size_t to, num;
    int32_t flow, cur_flow, capacity;

    edge(size_t to, size_t num, int32_t flow, int32_t capacity) : to(to), num(num), flow(flow),
                                                                  cur_flow(0), capacity(capacity) {}
};

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m, L;
    std::cin >> n >> m >> L;
    std::vector<size_t> level(n);
    std::vector<std::pair<size_t, size_t> > arr(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> level[i];
        arr[i] = {level[i], i};
    }
    std::sort(arr.begin(), arr.end());
    std::vector<size_t> order, reverse_order;
    std::for_each(arr.begin(), arr.end() - 1, [&order](auto v) {
        order.push_back(v.second);
    });
    std::for_each(arr.rbegin() + 1, arr.rend() - 1, [&reverse_order](auto v) {
        reverse_order.push_back(v.second);
    });
    std::vector<edge> edges;
    std::vector<std::vector<size_t> > g(n), g_t(n);
    for (size_t i = 0; i < m; i++) {
        size_t from, to;
        int32_t capacity;
        std::cin >> from >> to >> capacity;
        from--;
        to--;
        g[from].push_back(edges.size());
        edges.emplace_back(to, i, 0, capacity);
        g_t[to].push_back(edges.size());
        edges.emplace_back(from, m, 0, 0);
    }
    size_t s = arr[0].second,
        t = arr.back().second;
    std::vector<char> is_block(n, false);
    std::vector<int32_t> exc(n, 0);
    exc[arr[0].second] = INF;
    while (true) {
        for (size_t v : order) {
            if (is_block[v]) {
                continue;
            }
            for (size_t num : g[v]) {
                size_t to = edges[num].to;
                if (is_block[to]) {
                    continue;
                }
                int32_t k = std::min(exc[v],
                                     edges[num].capacity - edges[num].flow - edges[num].cur_flow);
                edges[num].cur_flow += k;
                edges[num ^ 1].cur_flow -= k;
                if (v != arr[0].second) {
                    exc[v] -= k;
                }
                exc[to] += k;
            }
            if (exc[v] > 0 && v != s && v != t) {
                is_block[v] = true;
            }
        }
        for (size_t v : reverse_order) {
            for (size_t num : g_t[v]) {
                size_t to = edges[num].to;
                int32_t k = std::min(exc[v], edges[num ^ 1].cur_flow);
                edges[num ^ 1].cur_flow -= k;
                edges[num].cur_flow += k;
                exc[v] -= k;
                if (to != s) {
                    exc[to] += k;
                }
            }
        }
        bool exit = true;
        for (edge &e : edges) {
            e.flow += e.cur_flow;
            if (e.cur_flow > 0) {
                exit = false;
            }
            e.cur_flow = 0;
        }
        if (exit) {
            break;
        }
    }
    std::vector<int32_t> ans(m);
    for (edge &e : edges) {
        if (e.num < m) {
            ans[e.num] = e.flow;
        }
    }
    std::for_each(ans.begin(), ans.end(), [](int32_t flow) { std::cout << flow << std::endl; });
    return 0;
}
