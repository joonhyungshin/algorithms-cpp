template<class T, class Container = vector<int>>
class graph {
	static_assert(is_same<typename Container::value_type, int>::value);
public:
	struct edge {
		int u;
		int v;
		T cost;
	};
	int n;
	vector<edge> edges;
	vector<Container> g;

	graph(int n_) : n(n_), edges(), g(n_) {}

	virtual void add(int u, int v, T cost) = 0;
};
