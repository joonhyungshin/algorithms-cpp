template<class F, class T = int, class Container = vector<int>>
class dinic {
public:
    flow_graph<F, T, Container> &g;
    vector<int> dist;
    vector<int> q;
    vector<typename Container::iterator> state;
    int s, t;

    static constexpr F eps = static_cast<F>(1e-9);

    dinic(flow_graph<F, T, Container> &g_) : g(g_), dist(), q(), state() {}

    void init() {
        dist.resize(g.n);
        q.resize(g.n);
        state.resize(g.n);
    }

    bool bfs() {
        fill(dist.begin(), dist.end(), -1);
        q[0] = t;
        dist[t] = 0;
        int l = 0, r = 1;
        while (l < r) {
            int node = q[l++];
            for (int idx : g.g[node]) {
                auto &re = g.edges[idx ^ 1];
                if (re.cap - re.flow > eps && dist[re.from] == -1) {
                    dist[re.from] = dist[node] + 1;
                    if (re.from == s) {
                    	return true;
                    }
                    q[r++] = re.from;
                }
            }
        }
        return false;
    }

    F dfs(int node, F cap_so_far) {
        if (node == t) {
            return cap_so_far;
        }
        for (auto &it = state[node]; it != g.g[node].end(); ++it) {
            auto &e = g.edges[*it];
            if (e.cap - e.flow > eps && dist[e.to] == dist[node] - 1) {
                F f = dfs(e.to, min(cap_so_far, e.cap - e.flow));
                if (f > eps) {
                    e.flow += f;
                    g.edges[*it ^ 1].flow -= f;
                    return f;
                }
            }
        }
        return static_cast<F>(0);
    }

    F run(int s_, int t_) {
        s = s_;
        t = t_;
        init();
        F res = static_cast<F>(0);
        while (bfs()) {
            for (int i = 0; i < g.n; i++) {
                state[i] = g.g[i].begin();
            }
            while (true) {
            	F f = dfs(s, numeric_limits<F>::max());
                if (f <= eps) {
                    break;
                }
            	res += f;
            }
        }
        return res;
    }
};
