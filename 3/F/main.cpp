#include <iostream>
#include <vector>
#include <algorithm>

void dfs(size_t v, size_t from, std::vector<std::vector<size_t> > &g, std::vector<char> &visit,
         std::vector<size_t> &match, std::vector<std::vector<char> > &possible) {
    visit[v] = true;
    for (size_t to : g[v]) {
        possible[from][to] = true;
        size_t TO = match[to];
        if (!visit[TO]) {
            dfs(TO, from, g, visit, match, possible);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<std::vector<size_t> > g(n);
    std::vector<size_t> match(n), px(n);
    std::vector<std::vector<char> > way(n, std::vector<char>(n, false));
    for (size_t i = 0; i < n; i++) {
        size_t k;
        std::cin >> k;
        while (k-- > 0) {
            size_t to;
            std::cin >> to;
            to--;
            g[i].push_back(to);
            way[i][to] = true;
        }
    }
    for (size_t i = 0; i < n; i++) {
        size_t j;
        std::cin >> j;
        j--;
        match[j] = i;
        px[i] = j;
    }
    std::vector<std::vector<char> > possible(n, std::vector<char>(n, false));
    for (size_t i = 0; i < n; i++) {
        std::vector<char> visit(n, false);
        dfs(i, i, g, visit, match, possible);
    }
    for (size_t i = 0; i < n; i++) {
        std::vector<size_t> ans;
        for (size_t j = 0; j < n; j++) {
            if (match[j] == i || (possible[match[j]][px[i]] && way[i][j])) {
                ans.push_back(j + 1);
            }
        }
        std::cout << ans.size() << " ";
        std::for_each(ans.begin(), ans.end(), [](size_t man) { std::cout << man << " "; });
        std::cout << std::endl;
    }
    return 0;
}
