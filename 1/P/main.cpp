#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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
    std::ifstream in("cnf.in");
    size_t n, m;
    in >> n >> m;
    std::vector<std::vector<size_t> > g(2 * n);
    while (m-- > 0) {
        size_t x[3];
        for (size_t &i : x) {
            int32_t a;
            in >> a;
            i = std::abs(a) - 1;
            if (a < 0) {
                i += n;
            }
        }
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                if (i != j) {
                    g[(x[i] + n) % (2 * n)].push_back(x[j]);
                }
            }
        }
    }
    in.close();
    std::vector<std::vector<size_t> > g_t(2 * n);
    for (size_t i = 0; i < 2 * n; i++) {
        for (size_t &to : g[i]) {
            g_t[to].push_back(i);
        }
    }
    std::vector<bool> visit(2 * n, false);
    std::vector<size_t> top_sort;
    for (size_t i = 0; i < 2 * n; i++) {
        if (!visit[i]) {
            dfs_first(i, g, visit, top_sort);
        }
    }
    std::reverse(top_sort.begin(), top_sort.end());
    visit.assign(2 * n, false);
    size_t comp = 0;
    std::vector<size_t> num_of_comp(2 * n);
    for (size_t &v : top_sort) {
        if (!visit[v]) {
            dfs_second(v, g_t, visit, num_of_comp, comp++);
        }
    }
    std::ofstream out("cnf.out");
    for (size_t i = 0; i < n; i++) {
        if (num_of_comp[i] == num_of_comp[i + n]) {
            out << "NO" << std::endl;
            out.close();
            return 0;
        }
    }
    out << "YES" << std::endl;
    for (size_t i = 0; i < n; i++) {
        if (num_of_comp[i] < num_of_comp[i + n]) {
            out << "-" << (i + 1) << " ";
        } else {
            out << i + 1 << " ";
        }
    }
    out << std::endl;
    out.close();
    return 0;
}
