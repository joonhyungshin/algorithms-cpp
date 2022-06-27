template<class T, class... Args>
vector<T> spfa(const graph<T, Args...> &g, int root) {
    vector<T> dist(g.n, numeric_limits<T>::max());
    T neg_inf = numeric_limits<T>::min();
    dist[root] = static_cast<T>(0);
    vector<int> q;
    vector<bool> inq(g.n, false);
    vector<int> inq_cnt(g.n);
    q.push_back(root);
    inq[root] = true;
    inq_cnt[root] = 1;
    for (int i = 0; i < static_cast<int>(q.size()); i++) {
        int node = q[i];
        inq[node] = false;
        T d = dist[node];
        for (int idx : g.g[node]) {
            auto &[u, v, cost] = g.edges[idx];
            int nxt = node ^ u ^ v;
            T updated_cost = (d == neg_inf ? neg_inf : d + cost);
            if (dist[nxt] > updated_cost) {
                dist[nxt] = updated_cost;
                if (!inq[nxt]) {
                    q.push_back(nxt);
                    inq[nxt] = true;
                    inq_cnt[nxt]++;
                    if (inq_cnt[nxt] >= g.n) {
                        dist[nxt] = neg_inf;
                    }
                }
            }
        }
    }
    return dist;
};
