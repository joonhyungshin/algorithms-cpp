template<class Node>
class segtree {
public:
    int n;
    vector<Node> tree;

    segtree(int n_) : n(n_), tree((n_ << 1) - 1) {}

    template<class Container>
    void build(int x, int l, int r, const Container &v) {
        if (l == r) {
            tree[x].init(l, v[l]);
            return;
        }
        int y = (l + r) >> 1;
        int z = x + ((y - l + 1) << 1);
        build(x + 1, l, y, v);
        build(z, y + 1, r, v);
        tree[x].pull(l, r, tree[x + 1], tree[z]);
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
        tree[x].pull(l, r, tree[x + 1], tree[z]);
    }

    Node get(int x, int l, int r, int s, int e) {
        if (s <= l && r <= e) {
            return tree[x];
        }
        int y = (l + r) >> 1;
        int z = x + ((y - l + 1) << 1);
        tree[x].push(l, r, tree[x + 1], tree[z]);
        Node res{};
        if (e <= y) {
            res = get(x + 1, l, y, s, e);
        } else {
            if (s > y) {
                res = get(z, y + 1, r, s, e);
            } else {
                res.pull(max(l, s), min(r, e), get(x + 1, l, y, s, e), get(z, y + 1, r, s, e));
            }
        }
        tree[x].pull(l, r, tree[x + 1], tree[z]);
        return res;
    }

    int find_first_knowingly(int x, int l, int r, const function<bool(const Node&)> &f) {
        if (l == r) {
            return l;
        }
        int y = (l + r) >> 1;
        int z = x + ((y - l + 1) << 1);
        tree[x].push(l, r, tree[x + 1], tree[z]);
        int res;
        if (f(tree[x + 1])) {
            res = find_first_knowingly(x + 1, l, y, f);
        } else {
            res = find_first_knowingly(z, y + 1, r, f);
        }
        tree[x].pull(l, r, tree[x + 1], tree[z]);
        return res;
    }

    int find_first(int x, int l, int r, int s, int e, const function<bool(const Node&)> &f) {
        if (s <= l && r <= e) {
            if (!f(tree[x])) {
                return -1;
            }
            return find_first_knowingly(x, l, r, f);
        }
        int y = (l + r) >> 1;
        int z = x + ((y - l + 1) << 1);
        tree[x].push(l, r, tree[x + 1], tree[z]);
        int res = -1;
        if (s <= y) {
            res = find_first(x + 1, l, y, s, e, f);
        }
        if (y < e && res == -1) {
            res = find_first(z, y + 1, r, s, e, f);
        }
        tree[x].pull(l, r, tree[x + 1], tree[z]);
        return res;
    }

    int find_last_knowingly(int x, int l, int r, const function<bool(const Node&)> &f) {
        if (l == r) {
            return l;
        }
        int y = (l + r) >> 1;
        int z = x + ((y - l + 1) << 1);
        tree[x].push(l, r, tree[x + 1], tree[z]);
        int res;
        if (f(tree[z])) {
            res = find_last_knowingly(z, y + 1, r, f);
        } else {
            res = find_last_knowingly(x + 1, l, y, f);
        }
        tree[x].pull(l, r, tree[x + 1], tree[z]);
        return res;
    }

    int find_last(int x, int l, int r, int s, int e, const function<bool(const Node&)> &f) {
        if (s <= l && r <= e) {
            if (!f(tree[x])) {
                return -1;
            }
            return find_last_knowingly(x, l, r, f);
        }
        int y = (l + r) >> 1;
        int z = x + ((y - l + 1) << 1);
        tree[x].push(l, r, tree[x + 1], tree[z]);
        int res = -1;
        if (y < e) {
            res = find_last(z, y + 1, r, s, e, f);
        }
        if (s <= y && res == -1) {
            res = find_last(x + 1, l, y, s, e, f);
        }
        tree[x].pull(l, r, tree[x + 1], tree[z]);
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

    int find_first(int s, int e, const function<bool(const Node&)> &f) {
        return find_first(0, 0, n - 1, s, e, f);
    }

    int find_last(int s, int e, const function<bool(const Node&)> &f) {
        return find_last(0, 0, n - 1, s, e, f);
    }
};

struct segnode {
	int x;

	segnode() : x() {}

	void init(int idx, int x_) {}

	void pull(int l, int r, const segnode &ln, const segnode &rn) {}

	void update(int l, int r, int x_) {}

	void push(int l, int r, segnode &ln, segnode &rn) {}
};
