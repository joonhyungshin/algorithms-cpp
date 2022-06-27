template<class F, class T = int, class... Args>
class mcmf {
public:
    flow_graph<F, T, Args...> &g;
    vector<T> dist;
    vector<int> bck;
    queue<int> q;
    vector<bool> inq;
    int s, t;

    mcmf(flow_graph<F, T, Args...> &g_) : g(g_), dist(), bck(), q(), inq() {}

    void init() {
        dist.resize(g.n);
        bck.resize(g.n);
        inq.resize(g.n);
    }

    pair<F, T> run(int s_, int t_, bool only_max_flow = true,
                   const F flow_lb = numeric_limits<F>::min(),
                   const F flow_ub = numeric_limits<F>::max()) {
        s = s_;
        t = t_;
        init();
        F max_flow = static_cast<F>(0);
        T min_cost = static_cast<T>(0);
        while (max_flow < flow_ub) {
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
                        bck[e.to] = idx;
                        if (!inq[e.to]) {
                            q.push(e.to);
                            inq[e.to] = true;
                        }
                    }
                }
            }
            if (dist[t] == numeric_limits<T>::max()
                || (!only_max_flow && max_flow >= flow_lb && dist[t] > static_cast<T>(0))) {
                break;
            }
            F f = flow_ub - max_flow;
            int cur = t;
            while (cur != s) {
                int idx = bck[cur];
                auto &e = g.edges[idx];
                f = min(f, e.cap - e.flow);
                cur = e.from;
            }
            cur = t;
            while (cur != s) {
                int idx = bck[cur];
                auto &e = g.edges[idx];
                auto &re = g.edges[idx ^ 1];
                e.flow += f;
                re.flow -= f;
                cur = e.from;
            }
            max_flow += f;
            min_cost += dist[t] * f;
        }
        return {max_flow, min_cost};
    }
};
