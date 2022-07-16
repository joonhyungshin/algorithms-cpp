class dsu {
public:
	vector<int> par;
	vector<int> sz;

	dsu(int n_) : par(n_), sz(n_, 1) {
		iota(par.begin(), par.end(), 0);
	}

	int find(int u) {
		return u == par[u] ? u : par[u] = find(par[u]);
	}

	bool uni(int u, int v) {
		u = find(u);
		v = find(v);
		if (u == v) {
			return false;
		}
		if (sz[u] < sz[v]) {
			swap(u, v);
		}
		par[v] = u;
		sz[u] += sz[v];
		return true;
	}

	int size(int u) {
		return sz[find(u)];
	}
};
