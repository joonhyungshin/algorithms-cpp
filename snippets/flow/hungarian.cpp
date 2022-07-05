template<class T>
class hungarian {
public:
    int n;
    vector<vector<T>> cost;
    vector<T> rlabel, clabel;
    vector<T> slack;
    vector<int> rslack;
    vector<int> rmatch, cmatch;
    vector<bool> s, t;
    vector<int> prv, q;
    int max_match;

    static constexpr T eps = static_cast<T>(1e-9);

    hungarian(int n_) : n(n_), cost(n, vector<T>(n)),
        rlabel(n), clabel(n),
        slack(n), rslack(n),
        rmatch(n), cmatch(n),
        s(n), t(n),
        prv(n), q(n)
    {}

    void set_cost(int i, int j, const T &c) {
        cost[i][j] = c;
    }

    void init() {
        for (int i = 0; i < n; i++) {
            rlabel[i] = numeric_limits<T>::min();
            for (int j = 0; j < n; j++) {
                rlabel[i] = max(rlabel[i], cost[i][j]);
            }
        }
    }

    void add(int i, int prvi) {
        s[i] = true;
        prv[i] = prvi;
        for (int j = 0; j < n; j++) {
            if (rlabel[i] + clabel[j] - cost[i][j] < slack[j]) {
                slack[j] = rlabel[i] + clabel[j] - cost[i][j];
                rslack[j] = i;
            }
        }
    }

    void update() {
        T delta = numeric_limits<T>::max();
        for (int j = 0; j < n; j++) {
            if (!t[j]) {
                delta = min(delta, slack[j]);
            }
        }
        for (int i = 0; i < n; i++) {
            if (s[i]) {
                rlabel[i] -= delta;
            }
        }
        for (int j = 0; j < n; j++) {
            if (t[j]) {
                clabel[j] += delta;
            }
        }
        for (int j = 0; j < n; j++) {
            if (!t[j]) {
                slack[j] -= delta;
            }
        }
    }

    void augment() {
        if (max_match == n) {
            return;
        }
        int i, j, root, wr = 0, rd = 0;
        fill(prv.begin(), prv.end(), -1);
        fill(s.begin(), s.end(), false);
        fill(t.begin(), t.end(), false);
        for (i = 0; i < n; i++) {
            if (rmatch[i] == -1) {
                q[wr++] = root = i;
                prv[i] = -2;
                s[i] = true;
                break;
            }
        }
        for (j = 0; j < n; j++) {
            slack[j] = rlabel[root] + clabel[j] - cost[root][j];
            rslack[j] = root;
        }
        while (true) {
            while (rd < wr) {
                i = q[rd++];
                for (j = 0; j < n; j++) {
                    if (rlabel[i] + clabel[j] - cost[i][j] <= eps && !t[j]) {
                        if (cmatch[j] == -1) {
                            break;
                        }
                        t[j] = true;
                        q[wr++] = cmatch[j];
                        add(cmatch[j], i);
                    }
                }
                if (j < n) {
                    break;
                }
            }
            if (j < n) {
                break;
            }
            update();
            wr = rd = 0;
            for (j = 0; j < n; j++) {
                if (!t[j] && slack[j] <= eps) {
                    if (cmatch[j] == -1) {
                        i = rslack[j];
                        break;
                    } else {
                        t[j] = true;
                        if (!s[cmatch[j]]) {
                            q[wr++] = cmatch[j];
                            add(cmatch[j], rslack[j]);
                        }
                    }
                }
            }
            if (j < n) {
                break;
            }
        }
        if (j < n) {
            max_match++;
            for (int ii = i, jj = j, tt; ii != -2; ii = prv[ii], jj = tt) {
                tt = rmatch[ii];
                cmatch[jj] = ii;
                rmatch[ii] = jj;
            }
            augment();
        }
    }

    T run() {
        T ret = static_cast<T>(0);
        max_match = 0;
        fill(rmatch.begin(), rmatch.end(), -1);
        fill(cmatch.begin(), cmatch.end(), -1);
        init();
        augment();
        for (int i = 0; i < n; i++){
            ret += cost[i][rmatch[i]];
        }
        return ret;
    }
};
