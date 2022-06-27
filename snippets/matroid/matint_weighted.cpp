template<class T, class M1, class M2>
vector<bool> matint(int n, const vector<T> &w, M1 &m1, M2 &m2,
                    bool only_max_size = true) {
    vector<bool> st(n);
    vector<int> bck(n + 2);
    vector<T> dist(n + 2);
    vector<int> pdist(n + 2);
    vector<bool> inq(n + 2);
    queue<int> q;
    while (true) {
        vector<vector<pair<T, int>>> g(n + 2);
        auto [A, E1] = m1.exchangable(st);
        auto [B, E2] = m2.exchangable(st);
        for (int i : A) {
            g[n].emplace_back(i, -w[i]);
        }
        for (int i : B) {
            g[i].emplace_back(n + 1, 0);
        }
        for (auto &[i, j] : E1) {
            g[j].emplace_back(i, -w[i]);
        }
        for (auto &[i, j] : E2) {
            g[i].emplace_back(j, w[j]);
        }
        fill(bck.begin(), bck.end(), -1);
        fill(dist.begin(), dist.end(), numeric_limits<T>::max());
        fill(pdist.begin(), pdist.end(), numeric_limits<int>::max());
        fill(inq.begin(), inq.end(), false);
        dist[n] = static_cast<T>(0);
        pdist[n] = 0;
        inq[n] = true;
        q.push(n);
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            inq[node] = false;
            T d = dist[node];
            int pd = pdist[node];
            for (auto &[nxt, cost] : g[node]) {
                if (dist[nxt] > d + cost
                    || (dist[nxt] == d + cost && pdist[nxt] > pd + 1)) {
                    dist[nxt] = d + cost;
                    pdist[nxt] = pd + 1;
                    bck[nxt] = node;
                    if (!inq[nxt]) {
                        q.push(nxt);
                        inq[nxt] = true;
                    }
                }
            }
        }
        int lst = bck[n + 1];
        if (lst == -1 || (!only_max_size && dist[n + 1] < 0)) {
            break;
        }
        while (lst != n) {
            st[lst] = !st[lst];
            lst = bck[lst];
        }
    }
    return st;
};
