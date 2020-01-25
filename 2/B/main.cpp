#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, s, f;
    std::cin >> n >> s >> f;
    s--;
    f--;
    std::vector<std::vector<std::pair<size_t, int64_t> > > g(n);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            int64_t v;
            std::cin >> v;
            if (i != j && v != -1) {
                g[i].emplace_back(j, v);
            }
        }
    }
    std::set<std::pair<int64_t, size_t> > st;
    st.insert(std::make_pair(0, s));
    std::vector<int64_t> dp(n, -1);
    dp[s] = 0;
    while (!st.empty()) {
        size_t v = st.begin()->second;
        st.erase(st.begin());
        for (std::pair<size_t, int64_t> &to : g[v]) {
            if (dp[to.first] == -1) {
                dp[to.first] = dp[v] + to.second;
                st.insert(std::make_pair(dp[to.first], to.first));
            } else if (dp[to.first] > dp[v] + to.second) {
                st.erase(st.find(std::make_pair(dp[to.first], to.first)));
                dp[to.first] = dp[v] + to.second;
                st.insert(std::make_pair(dp[to.first], to.first));
            }
        }
    }
    std::cout << dp[f] << std::endl;
    return 0;
}
