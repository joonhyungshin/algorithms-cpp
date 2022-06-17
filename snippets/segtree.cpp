template<class Node>
class segtree {
public:
	int n;
	vector<Node> tree;

	segtree(int n_) : n(n_), tree((n_ << 1) - 1) {}

	template<class Container>
	void build(int x, int l, int r, const Container &v) {
		if (l == r) {
			tree[x].init(v[l]);
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		build(x + 1, l, y, v);
		build(z, y + 1, r, v);
		tree[x].pull(tree[x + 1], tree[z]);
	}

	template<class... T>
	void update(int x, int l, int r, int s, int e, const T&... v) {
		if (s <= l && r <= e) {
			tree[x].update(l, r, v...);
			return;
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		tree[x].push(l, r, tree[x + 1], tree[z]);
		if (s <= y) {
			update(x + 1, l, y, s, e, v...);
		}
		if (e > y) {
			update(z, y + 1, r, s, e, v...);
		}
		tree[x].pull(tree[x + 1], tree[z]);
	}

	Node get(int x, int l, int r, int s, int e) {
		if (s <= l && r <= e) {
			return tree[x];
		}
		int y = (l + r) >> 1;
		int z = x + ((y - l + 1) << 1);
		tree[x].push(l, r, tree[x + 1], tree[z]);
		node res{};
		if (e <= y) {
			res = get(x + 1, l, y, s, e);
		} else {
			if (s > y) {
				res = get(z, y + 1, r, s, e);
			} else {
				res.pull(get(x + 1, l, y, s, e), get(z, y + 1, r, s, e));
			}
		}
		tree[x].pull(tree[x + 1], tree[z]);
		return res;
	}

	template<class Container>
	void build(const Container &v) {
		build(0, 0, n - 1, v);
	}

	template<class... T>
	void update(int s, int e, const T&... v) {
		update(0, 0, n - 1, s, e, v...);
	}

	Node get(int s, int e) {
		return get(0, 0, n - 1, s, e);
	}
};
