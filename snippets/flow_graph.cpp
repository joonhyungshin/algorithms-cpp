template<class F, class T = int, class Container = vector<int>>
class flow_graph {
    static_assert(is_same<typename Container::value_type, int>::value);
public:
    struct edge {
        int from;
        int to;
        F cap;
        F flow;
        T cost;
    };
    int n;
    vector<edge> edges;
    vector<Container> g;

    flow_graph(int n_) : n(n_), edges(), g(n_) {}

    void add(int u, int v, F forward_cap, F backward_cap, T cost = static_cast<T>(1)) {
        int idx = static_cast<int>(edges.size());
        edges.push_back({u, v, forward_cap, static_cast<F>(0), cost});
        edges.push_back({v, u, backward_cap, static_cast<F>(0), -cost});
        g[u].push_back(idx);
        g[v].push_back(idx ^ 1);
    }

    void clear_flow() {
        for (auto &e : edges) {
            e.flow = static_cast<F>(0);
        }
    }
};
