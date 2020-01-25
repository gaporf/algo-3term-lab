#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

struct edge {
    size_t x, y;
    char c;
};

void dfs(size_t v, std::vector<std::vector<std::pair<size_t, bool> > > &g,
         std::vector<bool> &visit, std::vector<bool> &cl) {
    visit[v] = true;
    for (std::pair<size_t, bool> &to : g[v]) {
        if (!visit[to.first]) {
            cl[to.first] = cl[v] ^ to.second;
            dfs(to.first, g, visit, cl);
        } else if (cl[to.first] != (cl[v] ^ to.second)) {
            throw std::runtime_error("Incorrect way");
        }
    }
}

bool check(size_t m, size_t n, std::vector<edge> &arr) {
    std::vector<std::vector<std::pair<size_t, bool> > > g(n);
    std::for_each(arr.begin(), arr.begin() + m, [&g](edge &e) {
        g[e.x].emplace_back(e.y, e.c == 'L');
        g[e.y].emplace_back(e.x, e.c == 'L');
    });
    std::vector<bool> visit(n, false),
            cl(n, false);
    for (size_t i = 0; i < n; i++) {
        if (!visit[i]) {
            try {
                dfs(i, g, visit, cl);
            } catch (std::exception &e) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("truth.in");
    size_t n, m;
    in >> n >> m;
    std::vector<edge> arr(m);
    std::for_each(arr.begin(), arr.end(), [&in](edge &e) {
        in >> e.x >> e.y >> e.c;
        e.x--;
        e.y--;
    });
    in.close();
    int32_t l = -1,
            r = static_cast<int32_t>(m) + 1;
    while (l + 1 < r) {
        int32_t mid = (l + r) / 2;
        if (check(mid, n, arr)) {
            l = mid;
        } else {
            r = mid;
        }
    }
    std::ofstream out("truth.out");
    out << l << std::endl;
    out.close();
    return 0;
}
