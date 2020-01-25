#include <iostream>
#include <vector>
#include <algorithm>

void dfs_first(size_t v, std::vector<std::vector<size_t> > &g,
               std::vector<char> &cl, std::vector<size_t> &top_sort) {
    cl[v] = 'b';
    for (size_t &to : g[v]) {
        if (cl[to] == 'w') {
            dfs_first(to, g, cl, top_sort);
        }
    }
    top_sort.push_back(v);
}

void dfs_second(size_t v, std::vector<std::vector<size_t> > &g,
                std::vector<char> &cl, std::vector<size_t> &num, size_t cond) {
    cl[v] = 'b';
    num[v] = cond;
    for (size_t &to : g[v]) {
        if (cl[to] == 'w') {
            dfs_second(to, g, cl, num, cond);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<size_t> > g(n), g_t(n);
    while (m-- > 0) {
        size_t from, to;
        std::cin >> from >> to;
        from--;
        to--;
        g[from].push_back(to);
        g_t[to].push_back(from);
    }
    std::vector<char> cl(n, 'w');
    std::vector<size_t> top_sort;
    for (size_t i = 0; i < n; i++) {
        if (cl[i] == 'w') {
            dfs_first(i, g, cl, top_sort);
        }
    }
    std::reverse(top_sort.begin(), top_sort.end());
    cl.assign(n, 'w');
    std::vector<size_t> num(n);
    size_t cond = 1;
    for (size_t &i : top_sort) {
        if (cl[i] == 'w') {
            dfs_second(i, g_t, cl, num, cond++);
        }
    }
    std::cout << cond - 1 << std::endl;
    for (size_t &i : num) {
        std::cout << i << " ";
    }
    return 0;
}
