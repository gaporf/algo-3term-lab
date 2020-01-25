#include <iostream>
#include <fstream>
#include <vector>

void dfs(size_t v, std::vector<std::vector<size_t> > &g,
         std::vector<bool> &cl, std::vector<bool> &visit) {
    visit[v] = true;
    for (size_t &to : g[v]) {
        if (!visit[to]) {
            cl[to] = !cl[v];
            dfs(to, g, cl, visit);
        } else if (cl[v] == cl[to]) {
            throw std::runtime_error("Found odd cycle");
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("bipartite.in");
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
    in.close();
    std::ofstream out("bipartite.out");
    std::vector<bool> visit(n, false),
            cl(n);
    for (size_t i = 0; i < n; i++) {
        if (!visit[i]) {
            try {
                cl[i] = false;
                dfs(i, g, cl, visit);
            } catch (std::exception &e) {
                out << "NO" << std::endl;
                out.close();
                return 0;
            }
        }
    }
    out << "YES" << std::endl;
    out.close();
    return 0;
}
