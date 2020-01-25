#include <iostream>
#include <fstream>
#include <vector>

void dfs(size_t v, size_t n, std::vector<std::vector<int32_t> > &g,
         std::vector<char> &visit, int32_t len) {
    visit[v] = 'b';
    for (size_t i = 0; i < n; i++) {
        if (visit[i] == 'w' && g[v][i] <= len) {
            dfs(i, n, g, visit, len);
        }
    }
}

void another_dfs(size_t v, size_t n, std::vector<std::vector<int32_t> > &g,
                 std::vector<char> &visit, int32_t len) {
    visit[v] = 'b';
    for (size_t i = 0; i < n; i++) {
        if (visit[i] == 'w' && g[i][v] <= len) {
            another_dfs(i, n, g, visit, len);
        }
    }
}

bool check(size_t n, int32_t len, std::vector<std::vector<int32_t> > &g) {
    std::vector<char> visit(n, 'w');
    dfs(0, n, g, visit, len);
    for (size_t i = 0; i < n; i++) {
        if (visit[i] == 'w') {
            return false;
        }
    }
    visit.assign(n, 'w');
    another_dfs(0, n, g, visit, len);
    for (size_t i = 0; i < n; i++) {
        if (visit[i] == 'w') {
            return false;
        }
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("avia.in");
    size_t n;
    in >> n;
    std::vector<std::vector<int32_t> > g(n, std::vector<int32_t>(n));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            in >> g[i][j];
        }
    }
    in.close();
    int32_t l = -1,
            r = 1000000000;
    while (l + 1 < r) {
        int32_t mid = (r + l) / 2;
        if (check(n, mid, g)) {
            r = mid;
        } else {
            l = mid;
        }
    }
    std::ofstream out("avia.out");
    out << r << std::endl;
    out.close();
    return 0;
}
