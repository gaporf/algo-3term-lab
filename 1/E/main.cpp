#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

void dfs(size_t v, std::vector<std::vector<size_t> > &g,
         std::vector<char> &cl, std::stack<size_t> &st) {
    cl[v] = 'g';
    st.push(v);
    for (auto &to : g[v]) {
        if (cl[to] == 'w') {
            dfs(to, g, cl, st);
        } else if (cl[to] == 'g') {
            std::cout << "YES" << std::endl;
            std::vector<size_t> ans;
            while (st.top() != to) {
                ans.push_back(st.top());
                st.pop();
            }
            ans.push_back(to);
            std::for_each(ans.rbegin(), ans.rend(),
                          [](size_t &num) { std::cout << num + 1 << " "; });
            std::cout << std::endl;
            throw std::runtime_error("Found cycle");
        }
    }
    cl[v] = 'b';
    st.pop();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<size_t> > g(n);
    while (m-- > 0) {
        size_t from, to;
        std::cin >> from >> to;
        g[--from].push_back(--to);
    }
    std::vector<char> cl(n, 'w');
    std::stack<size_t> st;
    for (size_t i = 0; i < n; i++) {
        if (cl[i] == 'w') {
            try {
                dfs(i, g, cl, st);
            } catch (std::exception &e) {
                return 0;
            }
        }
    }
    std::cout << "NO" << std::endl;
    return 0;
}
