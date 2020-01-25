#include <iostream>
#include <vector>
#include <math.h>
#include <iomanip>
#include <set>
#include <algorithm>

struct point {
    double x, y;
};

inline double sqr(double a) {
    return a * a;
}

inline double get_dist(point &a, point &b) {
    return std::sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n;
    std::cin >> n;
    std::vector<point> cord(n);
    std::for_each(cord.begin(), cord.end(), [](point &p) { std::cin >> p.x >> p.y; });
    std::vector<double> dp(n, 1e9);
    dp[0] = 0;
    double ans = 0;
    std::vector<bool> visit(n, false);
    for (size_t i = 0; i < n; i++) {
        size_t v = n;
        for (size_t j = 0; j < n; j++) {
            if (!visit[j]) {
                if (v == n || dp[j] < dp[v]) {
                    v = j;
                }
            }
        }
        visit[v] = true;
        ans += dp[v];
        for (size_t to = 0; to < n; to++) {
            if (!visit[to]) {
                double cur_dist = get_dist(cord[v], cord[to]);
                if (dp[to] > cur_dist) {
                    dp[to] = cur_dist;
                }
            }
        }
    }
    std::cout << std::setprecision(100) << ans << std::endl;
    return 0;
}
