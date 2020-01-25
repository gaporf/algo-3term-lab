#include <iostream>
#include <vector>
#include <algorithm>

struct edge {
    size_t from, to;
    uint32_t dist;

    edge() = default;

    edge(size_t from, size_t to, uint32_t dist) : from(from), to(to), dist(dist) {}

    bool operator<(edge &rhs) {
        return dist < rhs.dist;
    }
};

size_t get_comp(size_t v, std::vector<size_t> &comp) {
    if (v != comp[v]) {
        return comp[v] = get_comp(comp[v], comp);
    } else {
        return v;
    }
}

bool is_same_comp(size_t v, size_t u, std::vector<size_t> &comp) {
    return get_comp(v, comp) == get_comp(u, comp);
}

void unite(size_t v, size_t u, std::vector<size_t> &comp, std::vector<size_t> &rank) {
    v = get_comp(v, comp);
    u = get_comp(u, comp);
    if (rank[v] < rank[u]) {
        std::swap(v, u);
    }
    comp[u] = v;
    if (rank[v] == rank[u]) {
        rank[v]++;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    size_t n, m;
    std::cin >> n >> m;
    std::vector<edge> edges(m);
    std::for_each(edges.begin(), edges.end(), [](edge &e) {
        std::cin >> e.from >> e.to >> e.dist;
        e.from--;
        e.to--;
    });
    std::sort(edges.begin(), edges.end());
    uint32_t ans = 0;
    std::vector<size_t> comp(n),
            rank(n, 0);
    for (size_t i = 0; i < n; i++) {
        comp[i] = i;
    }
    for (edge &e : edges) {
        size_t v = e.from,
                u = e.to;
        if (!is_same_comp(v, u, comp)) {
            ans += e.dist;
            unite(v, u, comp, rank);
        }
    }
    std::cout << ans << std::endl;
    return 0;
}
