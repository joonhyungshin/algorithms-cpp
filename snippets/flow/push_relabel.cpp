template<class F, class T, class... Args>
class push_relabel {
public:
    flow_graph<F, T, Args...> &g;
    vector<F> extra;
    vector<int> hei, arc, prv, nxt, act, bot;
    queue<int> q;
    int high, cut, work, gp;
    int s, t;

    push_relabel(flow_graph<F, T, Args...> &g_, int gp_ = 4) : g(g_),
        extra(),
        hei(), arc(),
        prv(), nxt(),
        act(), bot(), q(),
        gp(gp_)
    {}

    void init() {
        extra.resize(g.n);
        hei.resize(g.n);
        arc.resize(g.n);
        prv.resize(g.n * 3);
        nxt.resize(g.n * 3);
        act.resize(g.n * 2);
        bot.resize(g.n);
    }

    void relabel(int v, int h) {
        prv[nxt[prv[v]] = nxt[v]] = prv[v];
        hei[v] = h;
        if (extra[v] > 0) {
            bot[v] = act[h];
            act[h] = v;
            high = max(high, h);
        }
        if (h < g.n) {
            cut = max(cut, h + 1);
        }
        nxt[v] = nxt[prv[v] = h += g.n];
        prv[nxt[nxt[h] = v]] = v;
    }

    void global() {
        int n = g.n;
        fill(hei.begin(), hei.end(), n * 2);
        fill(act.begin(), act.end(), -1);
        iota(prv.begin(), prv.end(), 0);
        iota(nxt.begin(), nxt.end(), 0);
        hei[t] = high = cut = work = 0;
        hei[s] = n;
        for (int x : {t, s}) {
            q.push(x);
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                for (int idx : g.g[v]) {
                    auto &e = g.edges[idx];
                    auto &re = g.edges[idx ^ 1];
                    if (hei[e.to] == n * 2 && re.flow < re.cap) {
                        q.push(e.to);
                        relabel(e.to, hei[v] + 1);
                    }
                }
            }
        }
    }

    void push(int v, int idx, bool z) {
        auto &e = g.edges[idx];
        auto &re = g.edges[idx ^ 1];
        auto f = min(extra[v], e.cap - e.flow);
        if (f > 0) {
            if (z && !extra[e.to]) {
                bot[e.to] = act[hei[e.to]];
                act[hei[e.to]] = e.to;
            }
            e.flow += f;
            re.flow -= f;
            extra[v] -= f;
            extra[e.to] += f;
        }
    }

    void discharge(int v) {
        int n = g.n;
        int h = n * 2, k = hei[v];
        for(int j = 0; j < static_cast<int>(g.g[v].size()); j++) {
            int idx = g.g[v][arc[v]];
            auto& e = g.edges[idx];
            if (e.flow < e.cap) {
                if (k == hei[e.to] + 1) {
                    push(v, idx, 1);
                    if (extra[v] <= 0) {
                        return;
                    }
                } else {
                    h = min(h, hei[e.to] + 1);
                }
            }
            if (++arc[v] >= static_cast<int>(g.g[v].size())) {
                arc[v] = 0;
            }
        }
        if (k < n && nxt[k + n] == prv[k + n]) {
            for (int j = k; j < cut; j++) {
                while (nxt[j + n] < n) {
                    relabel(nxt[j + n], n);
                }
            }
            cut = k;
        } else {
            relabel(v, h);
            work++;
        }
    }

    F run(int s_, int t_) {
        s = s_;
        t = t_;
        init();
        fill(extra.begin(), extra.end(), 0);
        fill(arc.begin(), arc.end(), 0);
        for (int idx : g.g[s]) {
            auto &e = g.edges[idx];
            extra[s] = e.cap;
            push(s, idx, false);
        }
        global();
        for (; high; high--) {
            while (act[high] != -1) {
                int v = act[high];
                act[high] = bot[v];
                if (v != s && hei[v] == high) {
                    discharge(v);
                    if (work > gp * g.n) global();
                }
            }
        }
        return extra[t];
    }
};
