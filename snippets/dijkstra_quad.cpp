template<class T, class... Args>
vector<T> dijkstra(const graph<T, Args...> &g, int root) {
	vector<T> dist(g.n, numeric_limits<T>::max());
	vector<bool> vis(g.n);
	dist[root] = 0;
	for (int i = 0; i < g.n - 1; i++) {
		T min_d = numeric_limits<T>::max();
		int node = -1;
		for (int j = 0; j < g.n; j++) {
			if (!vis[j] && min_d > dist[j]) {
				min_d = dist[j];
				node = j;
			}
		}
		if (node == -1) {
			break;
		}
		vis[node] = true;
		for (int idx : g.g[node]) {
			auto &[u, v, cost] = g.edges[idx];
			int nxt = node ^ u ^ v;
			if (dist[nxt] > min_d + cost) {
				dist[nxt] = min_d + cost;
			}
		}
	}
	return dist;
}
