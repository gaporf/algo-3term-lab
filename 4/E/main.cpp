#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct edge {
    size_t to, num;
    int32_t flow, capacity;

    edge(size_t to, size_t num, int32_t flow, int32_t capacity) : to(to), num(num),
                                                                  flow(flow), capacity(capacity) {}
};

bool bfs(std::vector<std::vector<size_t> > &g, std::vector<edge> &edges, std::vector<size_t> &dp) {
    dp.assign(dp.size(), dp.size());
    std::queue<size_t> q;
    q.push(0);
    dp[0] = 0;
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        for (size_t num : g[v]) {
            if (dp[edges[num].to] == dp.size() && edges[num].flow < edges[num].capacity) {
                dp[edges[num].to] = dp[v] + 1;
                q.push(edges[num].to);
            }
        }
    }
    return dp.back() != dp.size();
}

int32_t dfs(size_t v, int32_t cur_flow, std::vector<std::vector<size_t> > &g,
            std::vector<edge> &edges, std::vector<size_t> &dp, std::vector<size_t> &it) {
    if (v == g.size() - 1 || cur_flow == 0) {
        return cur_flow;
    }
    for (; it[v] < g[v].size(); it[v]++) {
        edge &e = edges[g[v][it[v]]];
        if (dp[e.to] == dp[v] + 1) {
            int32_t add_flow = dfs(e.to, std::min(cur_flow, e.capacity - e.flow), g, edges, dp, it);
            if (add_flow > 0) {
                edges[g[v][it[v]]].flow += add_flow;
                edges[g[v][it[v]] ^ 1].flow -= add_flow;
                return add_flow;
            }
        }
    }
    return 0;
}

bool dfs(size_t v, int32_t cur_flow, std::vector<std::vector<size_t> > &g, std::vector<edge> &edges,
         std::vector<char> &visit, std::pair<int32_t, std::vector<size_t> > &cur_ans) {
    if (v == g.size() - 1 && cur_flow > 0) {
        cur_ans.first = cur_flow;
        return true;
    }
    if (cur_flow == 0) {
        return false;
    }
    visit[v] = true;
    for (size_t num : g[v]) {
        edge &e = edges[num];
        if (!visit[e.to] && e.flow > 0) {
            if (dfs(e.to, std::min(cur_flow, e.flow), g, edges, visit, cur_ans)) {
                cur_ans.second.push_back(e.num);
                e.flow -= cur_ans.first;
                return true;
            }
        }
    }

    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<edge> edges;
    std::vector<std::vector<size_t> > g(n);
    for (size_t i = 1; i <= m; i++) {
        size_t from, to;
        int32_t capacity;
        std::cin >> from >> to >> capacity;
        from--;
        to--;
        g[from].push_back(edges.size());
        edges.emplace_back(to, i, 0, capacity);
        g[to].push_back(edges.size());
        edges.emplace_back(from, i, 0, 0);
    }
    int64_t max_flow = 0;
    std::vector<size_t> dp(n), it(n);
    while (bfs(g, edges, dp)) {
        it.assign(n, 0);
        while (true) {
            int32_t add_flow = dfs(0, std::numeric_limits<int32_t>::max(), g, edges, dp, it);
            if (add_flow == 0) {
                break;
            }
            max_flow += add_flow;
        }
    }
    std::vector<std::pair<int32_t, std::vector<size_t> > > ans;
    while (max_flow > 0) {
        std::pair<int32_t, std::vector<size_t> > cur_ans;
        std::vector<char> visit(n, false);
        dfs(0, std::numeric_limits<int32_t>::max(), g, edges, visit, cur_ans);
        max_flow -= cur_ans.first;
        std::reverse(cur_ans.second.begin(), cur_ans.second.end());
        ans.push_back(cur_ans);
    }
    std::cout << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [](auto v) {
        std::cout << v.first << " " << v.second.size() << " ";
        std::for_each(v.second.begin(), v.second.end(), [](auto num) {
            std::cout << num << " ";
        });
        std::cout << std::endl;
    });
    return 0;
}
