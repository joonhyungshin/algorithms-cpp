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

    mcmf(flow_graph<F, T, Container> &g_) : g(g_), dist(g_.n), q(), inq(g_.n), vis(g_.n), ptr(g_.n) {}

    void spfa() {
        fill(dist.begin(), dist.end(), numeric_limits<T>::max());
        fill(inq.begin(), inq.end(), false);
        dist[s] = static_cast<T>(0);
        inq[s] = true;
        q.push(s);
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            inq[node] = false;
            T d = dist[node];
            for (int idx : g.g[node]) {
                auto &e = g.edges[idx];
                if (e.flow < e.cap && dist[e.to] > d + e.cost) {
                    dist[e.to] = d + e.cost;
                    if (!inq[e.to]) {
                        q.push(e.to);
                        inq[e.to] = true;
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
        for (auto &it = ptr[node]; it != g.g[node].end(); it = next(it)) {
            auto &e = g.edges[*it];
            if (e.flow < e.cap && !vis[e.to] && dist[e.to] == dist[node] + e.cost) {
                F f = dfs(e.to, min(cap_so_far, e.cap - e.flow));
                if (f > static_cast<F>(0)) {
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
        F max_flow = static_cast<F>(0);
        T min_cost = static_cast<T>(0);
        while (max_flow < flow_ub) {
            spfa();
            if (dist[t] == numeric_limits<T>::max()
                || (!only_max_flow && max_flow >= flow_lb && dist[t] > static_cast<T>(0))) {
                break;
            }
            for (int i = 0; i < n; i++) {
                ptr[i] = g.g[i].begin();
            }
            while (true) {
                fill(vis.begin(), vis.end(), false);
                F f = dfs(s, flow_ub - max_flow);
                max_flow += f;
                min_cost += dist[t] * f;
                if (f == static_cast<F>(0) || max_flow == flow_ub) {
                    break;
                }
            }
        }
        return {max_flow, min_cost};
    }
};