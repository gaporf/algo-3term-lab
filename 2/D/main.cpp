#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

const size_t INF = 1000000000;

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<size_t, size_t> > > g(n);
    while (m-- > 0) {
        size_t from, to, dist;
        std::cin >> from >> to >> dist;
        from--;
        to--;
        g[from].emplace_back(to, dist);
        g[to].emplace_back(from, dist);
    }
    std::vector<size_t> dp(n, INF);
    dp[0] = 0;
    std::set<std::pair<size_t, size_t> > st;
    st.insert(std::make_pair(0, 0));
    while (!st.empty()) {
        size_t v = st.begin()->second;
        st.erase(st.begin());
        for (std::pair<size_t, size_t> &to : g[v]) {
            if (dp[to.first] > dp[v] + to.second) {
                if (dp[to.first] != INF) {
                    st.erase(st.find(std::make_pair(dp[to.first], to.first)));
                }
                dp[to.first] = dp[v] + to.second;
                st.insert(std::make_pair(dp[to.first], to.first));
            }
        }
    }
    std::for_each(dp.begin(), dp.end(), [](size_t &dist) { std::cout << dist << " "; });
    std::cout << std::endl;
    return 0;
}
