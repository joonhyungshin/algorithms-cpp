template<class T, class... Args>
class undigraph : public graph<T, Args...> {
public:
    using graph<T, Args...>::n;
	using graph<T, Args...>::edges;
    using graph<T, Args...>::g;

	undigraph(int n_) : graph<T, Args...>(n_) {}

	void add(int u, int v, T cost = static_cast<T>(1)) {
		int idx = static_cast<int>(edges.size());
		g[u].push_back(idx);
		g[v].push_back(idx);
		edges.push_back({u, v, cost});
	}
};
