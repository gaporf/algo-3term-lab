#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void dfs(size_t &i, std::vector<size_t> &vertices, std::vector<size_t> &cnt,
         std::vector<std::pair<size_t, size_t> > &ans, std::vector<size_t> &st) {
    size_t v = vertices[i++];
    if (!st.empty()) {
        ans.emplace_back(v + 1, st.back() + 1);
        for (size_t j = st.size() - 1; j > 0; j--) {
            if (v > st[j]) {
                ans.emplace_back(v + 1, st[j - 1] + 1);
            }
        }
    }
    st.push_back(v);
    cnt[v]--;
    while (cnt[v] > 0) {
        dfs(i, vertices, cnt, ans, st);
        cnt[v]--;
        i++;
    }
    st.pop_back();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("dfs.in");
    size_t n, l;
    in >> n >> l;
    std::vector<std::pair<size_t, size_t> > ans;
    std::vector<size_t> vertices(l),
            cnt(n, 0);
    std::for_each(vertices.begin(), vertices.end(), [&in, &cnt](size_t &v) {
        in >> v;
        v--;
        cnt[v]++;
    });
    in.close();
    size_t i = 0;
    while (i < l) {
        std::vector<size_t> st;
        dfs(i, vertices, cnt, ans, st);
    }
    std::ofstream out("dfs.out");
    out << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [&out](std::pair<size_t, size_t> &v) {
        out << v.first << " " << v.second << std::endl;
    });
    out << std::endl;
    out.close();
    return 0;
}
