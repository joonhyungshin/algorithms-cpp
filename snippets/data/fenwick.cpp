template<class T>
class fenwick {
private:
	int n;
	vector<T> tree;

public:
	fenwick(int n_) : n(n_), tree(n_, T()) {}

	void update(int i, T x) {
		for (int j = i + 1; j <= n; j += j & -j) {
			tree[j - 1] += x;
		}
	}

	T get(int i) {
		T res = 0;
		for (int j = i + 1; j; j -= j & -j) {
			res += tree[j - 1];
		}
		return res;
	}
};
