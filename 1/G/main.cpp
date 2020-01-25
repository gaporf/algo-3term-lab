#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

void dfs(size_t v, std::vector<std::vector<size_t> > &g,
         std::vector<size_t> &num, size_t comp) {
    num[v] = comp;
    for (size_t &to : g[v]) {
        if (num[to] == 0) {
            dfs(to, g, num, comp);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("components.in");
    size_t n, m;
    in >> n >> m;
    std::vector<std::vector<size_t> > g(n);
    while (m-- > 0) {
        size_t from, to;
        in >> from >> to;
        from--;
        to--;
        g[from].push_back(to);
        g[to].push_back(from);
    }
    size_t comp = 1;
    std::vector<size_t> num(n, 0);
    for (size_t i = 0; i < n; i++) {
        if (num[i] == 0) {
            dfs(i, g, num, comp++);
        }
    }
    std::ofstream out("components.out");
    out << comp - 1 << std::endl;
    std::for_each(num.begin(), num.end(), [&out](size_t &num) { out << num << " "; });
    in.close();
    out.close();
    return 0;
}
