#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

void dfs(size_t v, std::vector<std::vector<size_t> > &g, std::vector<size_t> &degree,
         std::map<std::pair<size_t, size_t>, bool> &exists,
         std::vector<size_t> &ans, std::vector<size_t> &it) {
    while (it[v] < g[v].size()) {
        size_t to = g[v][it[v]++];
        if (exists[std::make_pair(v, to)]) {
            exists[std::make_pair(v, to)] = exists[std::make_pair(to, v)] = false;
            degree[v]--;
            degree[to]--;
            dfs(to, g, degree, exists, ans, it);
            break;
        }
    }
    ans.push_back(v + 1);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<size_t> > g(n);
    std::map<std::pair<size_t, size_t>, bool> exists;
    std::vector<size_t> degree(n, 0),
            it(n, 0);
    while (m-- > 0) {
        size_t from, to;
        std::cin >> from >> to;
        from--;
        to--;
        degree[from]++;
        degree[to]++;
        g[from].push_back(to);
        g[to].push_back(from);
        exists[std::make_pair(from, to)] = exists[std::make_pair(to, from)] = true;
    }
    std::vector<std::vector<size_t> > ans;
    for (size_t i = 0; i < n; i++) {
        if (degree[i] % 2 == 1) {
            std::vector<size_t> cur_ans;
            dfs(i, g, degree, exists, cur_ans, it);
            ans.push_back(cur_ans);
        }
    }
    for (size_t i = 0; i < n; i++) {
        while (degree[i] > 0) {
            std::vector<size_t> cur_ans;
            dfs(i, g, degree, exists, cur_ans, it);
            ans.push_back(cur_ans);
        }
    }
    std::cout << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [](std::vector<size_t> &v) {
        std::for_each(v.rbegin(), v.rend(), [](size_t v) { std::cout << v << " "; });
        std::cout << std::endl;
    });
    return 0;
}
