#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct edge {
    size_t to;
    int32_t flow, capacity;

    edge(size_t to, int32_t flow, int32_t capacity) : to(to), flow(flow), capacity(capacity) {}
};

bool bfs(std::vector<std::vector<size_t> > &g, std::vector<edge> &edges, std::vector<size_t> &dp) {
    dp.assign(dp.size(), dp.size());
    std::queue<size_t> q;
    q.push(0);
    dp[0] = 0;
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        std::for_each(g[v].begin(), g[v].end(), [&g, &edges, &dp, &v, &q](size_t num) {
            size_t u = edges[num].to;
            int32_t flow = edges[num].flow,
                    capacity = edges[num].capacity;
            if (dp[u] == dp.size() && flow < capacity) {
                dp[u] = dp[v] + 1;
                q.push(u);
            }
        });
    }
    return dp.back() != dp.size();
}

size_t dfs(size_t v, size_t cur_flow, std::vector<std::vector<size_t> > &g,
           std::vector<edge> &edges, std::vector<size_t> &dp, std::vector<size_t> &it) {
    if (v == g.size() - 1 || cur_flow == 0) {
        return cur_flow;
    }
    for (; it[v] < g[v].size(); it[v]++) {
        edge e = edges[g[v][it[v]]];
        if (dp[e.to] == dp[v] + 1) {
            if (dfs(e.to, static_cast<size_t>(e.capacity - e.flow), g, edges, dp, it) > 0) {
                edges[g[v][it[v]]].flow += 1;
                edges[g[v][it[v]] ^ 1].flow -= 1;
                return 1;
            }
        }
    }
    return 0;
}

void find_Hall(size_t s, std::vector<std::vector<size_t> > &g, std::vector<size_t> &match) {
    std::vector<size_t> ans;
    std::vector<char> used(g.size(), false);
    used[s] = true;
    ans.push_back(s);
    std::queue<size_t> q;
    q.push(s);
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (size_t to : g[v]) {
            size_t to_to = match[to];
            if (!used[to_to]) {
                used[to_to] = true;
                ans.push_back(to_to);
                q.push(to_to);
            }
        }
    }
    std::sort(ans.begin(), ans.end());
    std::cout << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [](size_t v) { std::cout << v + 1 << " "; });
    std::cout << std::endl << std::endl;
}

void solve(size_t nl, size_t nr) {
    size_t n = 2 + nl + nr;
    size_t m;
    std::cin >> m;
    std::vector<edge> edges;
    std::vector<std::vector<size_t> > g(n);
    std::vector<std::vector<size_t> > gg(nr);
    for (size_t i = 1; i <= nl; i++) {
        g[0].push_back(edges.size());
        edges.emplace_back(i, 0, 1);
        g[i].push_back(edges.size());
        edges.emplace_back(0, 0, 0);
    }
    while (m-- > 0) {
        size_t l, r;
        std::cin >> l >> r;
        gg[r - 1].push_back(l - 1);
        r += nl;
        g[l].push_back(edges.size());
        edges.emplace_back(r, 0, 1);
        g[r].push_back(edges.size());
        edges.emplace_back(l, 0, 0);
    }
    for (size_t i = nl + 1; i <= nl + nr; i++) {
        g[i].push_back(edges.size());
        edges.emplace_back(n - 1, 0, 1);
        g.back().push_back(edges.size());
        edges.emplace_back(i, 0, 0);
    }
    std::vector<size_t> dp(n), it(n);
    while (bfs(g, edges, dp)) {
        it.assign(n, 0);
        while (dfs(0, 1, g, edges, dp, it) > 0) {}
    }
    std::vector<size_t> match(nl);
    std::vector<char> used(nr, false);
    for (size_t i = 1; i <= nl; i++) {
        for (size_t num : g[i]) {
            edge e = edges[num];
            if (e.to > nl && e.to < n - 1 && e.flow == 1) {
                match[i - 1] = e.to - nl - 1;
                used[e.to - nl - 1] = true;
            }
        }
    }
    for (size_t i = 0; i < nr; i++) {
        if (!used[i]) {
            find_Hall(i, gg, match);
            return;
        }
    }
    std::cout << 0 << std::endl << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t nl, nr;
    while (std::cin >> nl >> nr) {
        solve(nl, nr);
    }
    return 0;
}
