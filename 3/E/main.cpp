#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct action {
    action(size_t type, size_t x, size_t y, char color) : type(type), x(x), y(y), color(color) {}

    action &operator=(action const &rhs) = default;

    size_t type;
    size_t x, y;
    char color;
};

bool dfs(size_t v, std::vector<char> &visit,
         std::vector<std::vector<size_t> > &g, std::vector<size_t> &match) {
    if (visit[v]) {
        return false;
    }
    visit[v] = true;
    for (size_t to : g[v]) {
        if (match[to] == g.size() || dfs(match[to], visit, g, match)) {
            match[to] = v;
            return true;
        }
    }
    return false;
}

void dfs(size_t v, size_t part,
         std::vector<std::vector<size_t> > &g, std::vector<std::vector<size_t> > &gg,
         std::vector<char> &visit_first, std::vector<char> &visit_second) {
    if (part == 0) {
        visit_first[v] = true;
        for (size_t &to : g[v]) {
            if (!visit_second[to]) {
                dfs(to, 1, g, gg, visit_first, visit_second);
            }
        }
    } else {
        visit_second[v] = true;
        for (size_t &to : gg[v]) {
            if (!visit_first[to]) {
                dfs(to, 0, g, gg, visit_first, visit_second);
            }
        }
    }
}

void solve(size_t n, size_t m, std::vector<std::string> &chess, std::vector<action> &ans, char c) {
    size_t N = std::max(n, m);
    std::vector<std::vector<size_t> > g(2 * N), gg(2 * N);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            if (((i + j) % 2 == 0 && chess[i][j] != c) || ((i + j) % 2 == 1 && chess[i][j] == c)) {
                g[i + j].push_back(N + i - j);
            }
        }
    }
    std::vector<size_t> match(2 * N, 2 * N);
    std::vector<char> visit(2 * N);
    for (size_t i = 0; i < 2 * N; i++) {
        visit.assign(2 * N, false);
        dfs(i, visit, g, match);
    }
    std::vector<char> is_match(2 * N, false);
    for (size_t i = 0; i < 2 * N; i++) {
        if (match[i] != 2 * N) {
            is_match[match[i]] = true;
            g[match[i]].erase(std::find(g[match[i]].begin(), g[match[i]].end(), i));
            gg[i].push_back(match[i]);
        }
    }
    std::vector<char> visit_first(2 * N, false),
            visit_second(2 * N, false);
    for (size_t i = 0; i < 2 * N; i++) {
        if (!is_match[i] && !visit_first[i]) {
            dfs(i, 0, g, gg, visit_first, visit_second);
        }
    }
    for (size_t i = 0; i < 2 * N; i++) {
        if (!visit_first[i]) {
            char d;
            if (i % 2 == 0) {
                d = c;
            } else {
                d = (c == 'W') ? 'B' : 'W';
            }
            if (i < n) {
                ans.emplace_back(1, i + 1, 1, d);
            } else {
                ans.emplace_back(1, n, i - n + 2, d);
            }
        }
        if (visit_second[i]) {
            char d;
            if ((i + N) % 2 == 0) {
                d = c;
            } else {
                d = (c == 'W') ? 'B' : 'W';
            }
            if (i < N) {
                ans.emplace_back(2, 1, (N - i) + 1, d);
            } else {
                ans.emplace_back(2, (i - N) + 1, 1, d);
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::string> chess(n);
    std::for_each(chess.begin(), chess.end(), [](std::string &str) {
        std::cin >> str;
    });
    std::vector<action> first,
            second;
    solve(n, m, chess, first, 'W');
    solve(n, m, chess, second, 'B');
    if (first.size() > second.size()) {
        first = second;
    }
    std::cout << first.size() << std::endl;
    std::for_each(first.begin(), first.end(), [](action &act) {
        std::cout << act.type << " " << act.x << " " << act.y << " " << act.color << std::endl;
    });
    return 0;
}
