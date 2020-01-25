#include <iostream>
#include <vector>
#include <queue>

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<size_t> > g(n);
    std::vector<size_t> incomes(n, 0);
    while (m-- > 0) {
        size_t from, to;
        std::cin >> from >> to;
        g[--from].push_back(--to);
        incomes[to]++;
    }
    std::queue<size_t> q;
    for (size_t i = 0; i < n; i++) {
        if (incomes[i] == 0) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        if (q.size() != 1) {
            std::cout << "NO" << std::endl;
            return 0;
        }
        size_t v = q.front();
        q.pop();
        for (size_t &to : g[v]) {
            incomes[to]--;
            if (incomes[to] == 0) {
                q.push(to);
            }
        }
    }
    std::cout << "YES" << std::endl;
    return 0;
}
