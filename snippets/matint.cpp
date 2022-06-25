template<class M1, class M2>
vector<bool> matint(int n, M1 &m1, M2 &m2) {
    vector<bool> st(n);
    vector<int> q(n + 2);
    vector<int> bck(n + 2);
    while (true) {
        vector<vector<int>> graph(n + 2);
        auto [A, E1] = m1.exchangable(st);
        auto [B, E2] = m2.exchangable(st);
        for (int i : A) {
            graph[n].push_back(i);
        }
        for (int i : B) {
            graph[i].push_back(n + 1);
        }
        for (auto &[i, j] : E1) {
            graph[j].push_back(i);
        }
        for (auto &[i, j] : E2) {
            graph[i].push_back(j);
        }
        fill(bck.begin(), bck.end(), -1);
        q[0] = n;
        int l = 0, r = 1;
        while (l < r) {
            int u = q[l++];
            bool ok = false;
            for (int v : graph[u]) {
                if (bck[v] == -1) {
                    bck[v] = u;
                    if (v == n + 1) {
                        ok = true;
                        break;
                    } else {
                        q[r++] = v;
                    }
                }
            }
            if (ok) {
                break;
            }
        }
        int lst = bck[n + 1];
        if (lst == -1) {
            break;
        }
        while (lst != n) {
            st[lst] = !st[lst];
            lst = bck[lst];
        }
    }
    return st;
};
