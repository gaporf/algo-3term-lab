#include <iostream>
#include <vector>
#include <algorithm>

void dfs(size_t v, std::vector<std::vector<size_t> > &g,
         std::vector<size_t> &ans, std::vector<size_t> &it) {
    while (it[v] < g[v].size()) {
        size_t to = g[v][it[v]++];
        dfs(to, g, ans, it);
    }
    ans.push_back(v);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t d, k;
    std::cin >> d >> k;
    if (k == 1) {
        for (size_t i = 0; i < d; i++) {
            std::cout << i;
        }
        std::cout << std::endl;
        return 0;
    }
    size_t N = 1;
    for (size_t i = 0; i < k; i++) {
        N *= d;
    }
    size_t Nd = N / d;
    std::vector<std::vector<size_t>> g(N);
    std::vector<size_t> degree(N, 0),
            it(N, 0);
    for (size_t i = 0; i < N; i++) {
        size_t from = i / d,
                to = i % (Nd);
        g[from].push_back(to);
        degree[from]++;
        degree[to]++;
    }
    size_t s = 0;
    for (size_t i = 0; i < N; i++) {
        if (degree[i] % 2 == 1) {
            s = i;
        }
    }
    std::vector<size_t> ans;
    dfs(s, g, ans, it);
    for (size_t i = 0; i < k - 1; i++) {
        std::cout << 0;
    }
    std::for_each(ans.rbegin() + 1, ans.rend(), [&d](size_t v) {
       std::cout << v % d;
    });
    return 0;
}
