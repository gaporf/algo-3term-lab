#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

void find_edge(std::string &last, std::string &cur, std::vector<std::vector<size_t> > &g) {
    if (last.length() < cur.length()) {
        return;
    } else if (last.length() == cur.length()) {
        for (size_t i = 0; i != last.length(); i++) {
            if (last[i] != cur[i]) {
                g[static_cast<size_t>(last[i] - 'a')].push_back(
                        static_cast<size_t>(cur[i] - 'a'));
                return;
            }
        }
        throw std::runtime_error("Incorrect order");
    } else {
        throw std::runtime_error("Incorrect order");
    }
}

void check(std::string &str, std::vector<bool> &is_zero) {
    if (str.length() > 1) {
        is_zero[static_cast<size_t>(str[0] - 'a')] = false;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::ifstream in("tiv.in");
    std::vector<std::vector<size_t> > g(26);
    size_t n;
    in >> n;
    std::vector<bool> is_zero(26, true);
    std::string last;
    in >> last;
    check(last, is_zero);
    n--;
    std::ofstream out("tiv.out");
    while (n-- > 0) {
        std::string cur;
        in >> cur;
        check(cur, is_zero);
        try {
            find_edge(last, cur, g);
        } catch (std::exception &e) {
            out << "No" << std::endl;
            out.close();
            return 0;
        }
        last = cur;
    }
    in.close();
    std::vector<size_t> ans;
    std::vector<size_t> incomes(26, 0);
    for (size_t i = 0; i < 26; i++) {
        for (size_t &to : g[i]) {
            incomes[to]++;
        }
    }
    std::queue<size_t> q;
    bool find = false;
    for (size_t i = 0; i < 26; i++) {
        if (is_zero[i] && incomes[i] == 0) {
            q.push(i);
            for (size_t j = 0; j < 26; j++) {
                if (i != j && incomes[j] == 0) {
                    q.push(j);
                }
            }
            find = true;
            break;
        }
    }
    if (!find) {
        out << "No" << std::endl;
        out.close();
        return 0;
    }
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        ans.push_back(v);
        for (size_t &to : g[v]) {
            incomes[to]--;
            if (incomes[to] == 0) {
                q.push(to);
            }
        }
    }
    if (ans.size() != 26) {
        out << "No" << std::endl;
        out.close();
        return 0;
    }
    out << "Yes" << std::endl;
    std::vector<size_t> real_ans(26);
    for (size_t i = 0; i < 26; i++) {
        real_ans[ans[i]] = i;
    }
    std::for_each(real_ans.begin(), real_ans.end(),
                  [&out](size_t &num) { out << num << " "; });
    out << std::endl;
    out.close();
    return 0;
}
