template<class T, class... Args>
class digraph : public graph<T, Args...> {
public:
    using graph<T, Args...>::n;
	using graph<T, Args...>::edges;
    using graph<T, Args...>::g;

    digraph(int n_) : graph<T, Args...>(n_) {}

    void add(int from, int to, T cost = static_cast<T>(1)) {
        g[from].push_back(static_cast<int>(edges.size()));
        edges.push_back({from, to, cost});
    }
};
