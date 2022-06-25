template<class T, class F = plus<T>>
class fenwick {
private:
	int n;
	vector<T> tree;
	F f;

public:
	fenwick(int n_, T id_ = static_cast<T>(0)) : n(n_), tree(n_, id_), f() {}
	fenwick(int n_, const F &f_, T id_ = static_cast<T>(0)) : n(n_), tree(n_, id_), f(f_) {}

	void update(int i, T x) {
		for (int j = i + 1; j <= n; j += j & -j) {
			tree[j - 1] = f(tree[j - 1], x);
		}
	}

	T get(int i) {
		T res = tree[i];
		for (int j = i & (i + 1); j; j -= j & -j) {
			res = f(res, tree[j - 1]);
		}
		return res;
	}
};
