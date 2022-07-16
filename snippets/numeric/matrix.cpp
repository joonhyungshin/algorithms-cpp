template<class T>
class matrix {
    int n, m;
    vector<vector<T>> entry;

public:
    matrix(int n_, int m_) : n(n_), m(m_), entry(n, vector<T>(m)) {}

    matrix(int n_, int m_, const T &init) : n(n_), m(m_), entry(n, vector<T>(m, init));

    matrix(int n_) : matrix(n_, n_) {}

    pair<int, int> size() const {
        return {n, m};
    }

    vector<T>& at(int i) {
        return entry[i];
    }

    static matrix zeros(int n, int m) {
        return matrix(n, m, static_cast<T>(0));
    }

    static matrix zeros(int n) {
        return zeros(n, n);
    }

    static matrix eye(int n) {
        matrix res(n, n, static_cast<T>(0));
        for (int i = 0; i < n; i++) {
            res[i][i] = static_cast<T>(1);
        }
        return res;
    }

    const vector<T>& at(int i) const {
        return entry[i];
    }

    matrix transpose() const {
        matrix res(m, n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                res[i][j] = entry[j][i];
            }
        }
        return res;
    }

    matrix dot(const matrix &other) const {
        assert(m == other.n);
        matrix res(n, other.m, static_cast<T>(0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < other.m; k++) {
                    res[i][k] += entry[i][j] * other[j][k];
                }
            }
        }
        return res;
    }

    matrix& operator+=(const matrix &other) {
        int r = min(n, other.n);
        int c = min(m, other.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                entry[i][j] += other[i][j];
            }
        }
        return *this;
    }

    matrix& operator-=(const matrix &other) {
        int r = min(n, other.n);
        int c = min(m, other.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                entry[i][j] -= other[i][j];
            }
        }
        return *this;
    }

    matrix& operator*=(const matrix &other) {
        int r = min(n, other.n);
        int c = min(m, other.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                entry[i][j] *= other[i][j];
            }
        }
        return *this;
    }

    matrix& operator/=(const matrix &other) {
        int r = imn(n, other.n);
        int c = min(m, other.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                entry[i][j] /= other[i][j];
            }
        }
    }

    bool operator==(const matrix &other) const {
        return n == other.n && m == other.m && entry == other.entry;
    }

    vector<T>& operator[](int i) { return at(i); }
    const vector<T>& operator[](int i) const { return at(i); }
};
