template<class F, class Container = vector<int>, class... Args>
class dinic {
public:
    flow_graph<F, Container, Args...> &g;
    vector<int> dist;
    vector<int> q;
    vector<typename Container::iterator> state;
    int s, t;

    dinic(flow_graph<F, Container, Args...> &g_) : g(g_), dist(g_.n), q(g_.n), state(g_.n) {}

    bool bfs() {
        fill(dist.begin(), dist.end(), -1);
        q[0] = t;
        dist[t] = 0;
        int l = 0, r = 1;
        while (l < r) {
            int node = q[l++];
            for (int idx : g.g[node]) {
                auto &e = g.edges[idx];
                auto &re = g.edges[idx ^ 1];
                if (re.flow < re.cap && dist[e.to] == -1) {
                    dist[e.to] = dist[node] + 1;
                    if (e.to == s) {
                    	return true;
                    }
                    q[r++] = e.to;
                }
            }
        }
        return false;
    }

    F dfs(int node, F cap_so_far) {
        if (node == t) {
            return cap_so_far;
        }
        for (auto &it = state[node]; it != g.g[node].end(); it = next(it)) {
            auto &e = g.edges[*it];
            if (e.flow < e.cap && dist[e.to] == dist[node] - 1) {
                F f = dfs(e.to, min(cap_so_far, e.cap - e.flow));
                if (f > 0) {
	                e.flow += f;
	                g.edges[*it ^ 1].flow -= f;
	                return f;
	            }
            }
        }
        return 0;
    }

    F run(int s_, int t_) {
        s = s_;
        t = t_;
        F res = 0;
        while (bfs()) {
            for (int i = 0; i < g.n; i++) {
                state[i] = g.g[i].begin();
            }
            while (true) {
            	F f = dfs(s, numeric_limits<F>::max());
            	res += f;
            	if (f == 0) {
            		break;
            	}
            }
        }
        return res;
    }
};
