template<class T, class... Args>
vector<T> dijkstra(const graph<T, Args...> &g, int root) {
	vector<T> dist(g.n, numeric_limits<T>::max());
	dist[root] = 0;
	priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
	pq.emplace(dist[root], root);
	while (!pq.empty()) {
		auto [d, node] = pq.top();
		pq.pop();
		if (d != dist[node]) {
			continue;
		}
		for (int idx : g.g[node]) {
			auto &[u, v, cost] = g.edges[idx];
			int nxt = node ^ u ^ v;
			if (dist[nxt] > d + cost) {
				dist[nxt] = d + cost;
				pq.emplace(dist[nxt], nxt);
			}
		}
	}
	return dist;
}
