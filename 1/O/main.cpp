#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void add_edge(size_t n, size_t v, size_t u, std::vector<std::vector<size_t> > &g) {
    g[(v + 2 * n) % (4 * n)].push_back(u);
    g[(u + 2 * n) % (4 * n)].push_back(v);
}

void dfs_first(size_t v, std::vector<std::vector<size_t> > &g,
               std::vector<bool> &visit, std::vector<size_t> &top_sort) {
    visit[v] = true;
    for (size_t &to : g[v]) {
        if (!visit[to]) {
            dfs_first(to, g, visit, top_sort);
        }
    }
    top_sort.push_back(v);
}

void dfs_second(size_t v, std::vector<std::vector<size_t> > &g,
                std::vector<bool> &visit, std::vector<size_t> &num_of_comp, size_t comp) {
    visit[v] = true;
    num_of_comp[v] = comp;
    for (size_t &to : g[v]) {
        if (!visit[to]) {
            dfs_second(to, g, visit, num_of_comp, comp);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("chip.in");
    size_t n;
    in >> n;
    std::vector<size_t> cl(n),
            numbers(2 * n),
            last_num(n, 2 * n);
    std::vector<std::vector<size_t> > g(4 * n),
            g_t(4 * n);
    for (size_t i = 0; i < n; i++) {
        in >> cl[i];
    }
    for (size_t i = 0; i < 2 * n; i++) {
        in >> numbers[i];
        numbers[i]--;
        if (last_num[numbers[i]] == 2 * n) {
            last_num[numbers[i]] = i;
        } else {
            add_edge(n, i, last_num[numbers[i]], g);
            add_edge(n, i + 2 * n, last_num[numbers[i]] + 2 * n, g);
        }
    }
    in.close();
    for (size_t i = 0; i < 2 * n; i++) {
        if (cl[numbers[i]] == cl[numbers[(i + 1) % (2 * n)]]) {
            add_edge(n, i + 2 * n, ((i + 1) % (2 * n)) + 2 * n, g);
        }
    }
    std::vector<size_t> top_sort;
    for (size_t i = 0; i < 4 * n; i++) {
        for (size_t &to : g[i]) {
            g_t[to].push_back(i);
        }
    }
    std::vector<bool> visit(4 * n, false);
    for (size_t i = 0; i < 4 * n; i++) {
        if (!visit[i]) {
            dfs_first(i, g, visit, top_sort);
        }
    }
    std::reverse(top_sort.begin(), top_sort.end());
    visit.assign(4 * n, false);
    std::vector<size_t> num_of_comp(4 * n);
    size_t comp = 0;
    for (size_t &v : top_sort) {
        if (!visit[v]) {
            dfs_second(v, g_t, visit, num_of_comp, comp++);
        }
    }
    std::ofstream out("chip.out");
    for (size_t i = 0; i < 2 * n; i++) {
        if (num_of_comp[i] == num_of_comp[i + 2 * n]) {
            out << "NO" << std::endl;
            return 0;
        }
    }
    out << "YES" << std::endl;
    std::vector<size_t> ans(n);
    for (size_t i = 0; i < 2 * n; i++) {
        if (num_of_comp[i] > num_of_comp[i + 2 * n]) {
            ans[numbers[i]] = i + 1;
        }
    }
    std::for_each(ans.begin(), ans.end(), [&out](size_t &num) { out << num << " "; });
    return 0;
}
