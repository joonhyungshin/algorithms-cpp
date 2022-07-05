template<class F, class T = int, class Container = vector<int>>
class mcmf {
public:
    flow_graph<F, T, Container> &g;
    vector<T> dist;
    queue<int> q;
    vector<bool> inq;
    vector<bool> vis;
    vector<typename Container::iterator> ptr;
    int s, t;

    static constexpr F eps = static_cast<F>(1e-9);

    mcmf(flow_graph<F, T, Container> &g_) : g(g_), dist(), q(), inq(), vis(), ptr() {}

    void init() {
        dist.resize(g.n);
        inq.resize(g.n);
        vis.resize(g.n);
        ptr.resize(g.n);
    }

    void spfa() {
        fill(dist.begin(), dist.end(), numeric_limits<T>::max());
        fill(inq.begin(), inq.end(), false);
        dist[t] = static_cast<T>(0);
        inq[t] = true;
        q.push(t);
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            inq[node] = false;
            T d = dist[node];
            for (int idx : g.g[node]) {
                auto &re = g.edges[idx ^ 1];
                if (re.cap - re.flow > eps && dist[re.from] > d + re.cost) {
                    dist[re.from] = d + re.cost;
                    if (!inq[re.from]) {
                        q.push(re.from);
                        inq[re.from] = true;
                    }
                }
            }
        }
    }

    F dfs(int node, F cap_so_far) {
        if (node == t) {
            return cap_so_far;
        }
        vis[node] = true;
        for (auto &it = ptr[node]; it != g.g[node].end(); ++it) {
            auto &e = g.edges[*it];
            if (e.cap - e.flow > eps && !vis[e.to] && dist[e.to] == dist[node] - e.cost) {
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

    pair<F, T> run(int s_, int t_, bool only_max_flow = true,
                   const F flow_lb = numeric_limits<F>::min(),
                   const F flow_ub = numeric_limits<F>::max()) {
        s = s_;
        t = t_;
        init();
        F max_flow = static_cast<F>(0);
        T min_cost = static_cast<T>(0);
        while (flow_ub - max_flow > eps) {
            spfa();
            if (dist[s] == numeric_limits<T>::max()
                || (!only_max_flow && max_flow >= flow_lb && dist[s] > static_cast<T>(0))) {
                break;
            }
            for (int i = 0; i < g.n; i++) {
                ptr[i] = g.g[i].begin();
            }
            while (true) {
                fill(vis.begin(), vis.end(), false);
                F f = dfs(s, flow_ub - max_flow);
                max_flow += f;
                min_cost += dist[s] * f;
                if (f <= eps || flow_ub - max_flow <= eps) {
                    break;
                }
            }
        }
        return {max_flow, min_cost};
    }
};
