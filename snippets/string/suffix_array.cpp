template<class String = string>
class suffix_array {
public:
    using T = typename String::value_type;
    String &s;
    vector<int> s_int;
    vector<int> aux;
    vector<int> sa;
    vector<int> ptr;
    vector<int> lcp;
    int sigma;

    suffix_array(String &s_) : s(s_), s_int(), aux(), sa(), ptr(), lcp() {}

    void init(const T &msc, const T &mxc) {
        int n = static_cast<int>(s.size());
        s_int.resize(n);
        aux.resize(n);
        sa.resize(n);
        vector<int> ch(mxc - msc + 1);
        for (int i = 0; i < n; i++) {
            ch[s[i] - msc] = 1;
        }
        for (int i = 1; i <= static_cast<int>(mxc - msc); i++) {
            ch[i] += ch[i - 1];
        }
        for (int i = 0; i < n; i++) {
            s_int[i] = ch[s[i] - msc] - 1;
        }
        sigma = ch[mxc - msc];
    }

    void sa_is(const vector<int> &S, int sig, vector<int> &SA) {
        int sz = static_cast<int>(S.size());
        if (sz == 1) {
            SA[0] = 0;
            return;
        }
        vector<bool> T(sz);
        vector<int> B(sig + 1);
        int m = 0;
        for (int i = sz - 2; i >= 0; i--) {
            if (S[i] < S[i + 1]) {
                T[i] = true;
            } else if (S[i] == S[i + 1]) {
                T[i] = T[i + 1];
            } else {
                m += T[i + 1];
            }
        }
        fill(SA.begin(), SA.end(), -1);
        for (int i = 0; i < sz; i++) {
            B[S[i] + 1]++;
        }
        for (int i = 1; i <= sig; i++) {
            B[i] += B[i - 1];
        }
        if (m) {
            vector<int> P1(m + 1), S1(m), SA1(m);
            ptr.assign(next(B.begin()), B.end());
            for (int i = 1, j = 0; i < sz; i++) {
                if (T[i] && !T[i - 1]) {
                    aux[i] = j;
                    P1[j++] = i;
                    SA[--ptr[S[i]]] = i;
                }
            }
            P1[m] = sz;
            ptr.assign(B.begin(), prev(B.end()));
            SA[ptr[S[sz - 1]]++] = sz - 1;
            for (int i = 0; i < sz; i++) {
                int j = SA[i] - 1;
                if (j >= 0 && !T[j]) {
                    SA[ptr[S[j]]++] = j;
                }
            }
            ptr.assign(next(B.begin()), B.end());
            for (int i = sz - 1; i >= 0; i--) {
                int j = SA[i] - 1;
                if (j >= 0 && T[j]) {
                    SA[--ptr[S[j]]] = j;
                }
            }
            for (int i = 0, j = 0; i < sz; i++) {
                if (SA[i] > 0 && T[SA[i]] && !T[SA[i] - 1]) {
                    SA1[j++] = aux[SA[i]];
                }
            }
            for (int i = 1; i < m; i++) {
                int q1 = SA1[i - 1];
                int q2 = SA1[i];
                int j1 = P1[q1];
                int n1 = P1[q1 + 1] - j1;
                int j2 = P1[q2];
                int n2 = P1[q2 + 1] - j2;
                S1[q2] = S1[q1];
                if (n1 != n2) {
                    S1[q2]++;
                } else {
                    for (int j = 0; j < n1; j++) {
                        if (S[j1 + j] < S[j2 + j] || T[j1 + j] < T[j2 + j]) {
                            S1[q2]++;
                            break;
                        }
                    }
                }
            }
            int sig1 = S1[SA1[m - 1]] + 1;
            if (sig1 < m) {
                sa_is(S1, sig1, SA1);
            }
            fill(SA.begin(), SA.end(), -1);
            ptr.assign(next(B.begin()), B.end());
            for (int i = m - 1; i >= 0; i--) {
                int j = P1[SA1[i]];
                SA[--ptr[S[j]]] = j;
            }
        }
        ptr.assign(B.begin(), prev(B.end()));
        SA[ptr[S[sz - 1]]++] = sz - 1;
        for (int i = 0; i < sz; i++) {
            int j = SA[i] - 1;
            if (j >= 0 && !T[j]) {
                SA[ptr[S[j]]++] = j;
            }
        }
        ptr.assign(next(B.begin()), B.end());
        for (int i = sz - 1; i >= 0; i--) {
            int j = SA[i] - 1;
            if (j >= 0 && T[j]) {
                SA[--ptr[S[j]]] = j;
            }
        }
    }

    void run(const T &msc = numeric_limits<T>::min(),
             const T &mxc = numeric_limits<T>::max(),
             bool compute_lcp = true) {
        init(msc, mxc);
        sa_is(s_int, sigma, sa);
        if (compute_lcp) {
            int n = static_cast<int>(s.size());
            lcp.resize(n - 1);
            vector<int> rnk(n);
            for (int i = 0; i < n; i++) {
                rnk[sa[i]] = i;
            }
            s_int.push_back(-1);
            for (int i = 0, j = 0; i < n; i++) {
                int r = rnk[i];
                if (r) {
                    while (s_int[i + j] == s_int[sa[r - 1] + j]) {
                        j++;
                    }
                    lcp[r - 1] = j;
                    if (j) {
                        j--;
                    }
                }
            }
            s_int.pop_back();
        }
    }
};
