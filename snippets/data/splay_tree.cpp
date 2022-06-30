template<class T>
class splay_tree {
public:
    struct node {
        node *l, *r, *p;
        T x;
    };

    struct iterator {
        using iterator_category = bidirectional_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        node *x;

        iterator(node *x_) : x(x_) {}

        iterator parent() const {
            return iterator(x->p);
        }

        iterator left() const {
            return iterator(x->l);
        }

        iterator right() const {
            return iterator(x->r);
        }

        iterator &operator++() {
            if (x->r) {
                x = x->r;
                while (x->l) {
                    x = x->l;
                }
            } else {
                while (x->p && x->p->r == x) {
                    x = x->p;
                }
                x = x->p;
            }
            return *this;
        }

        iterator &operator--() {
            if (x->l) {
                x = x->l;
                while (x->r) {
                    x = x->r;
                }
            } else {
                while (x->p && x->p->l == x) {
                    x = x->p;
                }
                x = x->p;
            }
            return *this;
        }

        reference operator*() { return x->x; }
        pointer operator->() { return &x->x; }

        iterator operator++(int) { iterator ret(*this); ++(*this); return ret; }
        iterator operator--(int) { iterator ret(*this); --(*this); return ret; }
        bool operator==(const iterator &other) const { return x == other.x; }
        bool operator!=(const iterator &other) const { return x != other.x; }
    };

    node *tree;

    splay_tree() : tree() {}

    void rotate(node *x) {
        node *p = x->p;
        node *b;
        if (x == p->l) {
            p->l = b = x->r;
            x->r = p;
        } else {
            p->r = b = x->l;
            x->l = p;
        }
        x->p = p->p;
        p->p = x;
        if (b) {
            b->p = p;
        }
        (x->p ? p == x->p->l ? x->p->l : x->p->r : tree) = x;
    }

    void splay(node *x) {
        while (x->p) {
            node *p = x->p;
            node *g = p->p;
            if (g) {
                rotate((x == p->l) == (p == g->l) ? p : x);
            }
            rotate(x);
        }
    }

    iterator root() const {
        return iterator(tree);
    }

    iterator begin() {
        if (!tree) {
            return iterator(nullptr);
        }
        node *x = tree;
        while (x->l) {
            x = x->l;
        }
        splay(x);
        return root();
    }

    iterator end() const {
        return iterator(nullptr);
    }

    iterator next(const iterator &it) {
        splay(it.x);
        node *x = tree->r;
        while (x->l) {
            x = x->l;
        }
        splay(x);
        return root();
    }

    iterator prev(const iterator &it) {
        splay(it.x);
        node *x = tree->l;
        while (x->r) {
            x = x->r;
        }
        splay(x);
        return root();
    }

    iterator insert(const iterator &it, const T &x_) {
        if (!tree) {
            node *x = new node;
            x->l = x->r = x->p = nullptr;
            x->x = x_;
            tree = x;
            return root();
        }
        node *p, **pp;
        if (it == end()) {
            p = tree;
            while (p->r) {
                p = p->r;
            }
            pp = &p->r;
        } else {
            splay(it.x);
            if (tree->l) {
                p = tree->l;
                while (p->r) {
                    p = p->r;
                }
                pp = &p->r;
            } else {
                p = tree;
                pp = &p->l;
            }
        }
        node *x = new node;
        x->l = x->r = nullptr;
        x->p = p;
        x->x = x_;
        *pp = x;
        splay(x);
        return root();
    }

    iterator erase(const iterator &it) {
        splay(it.x);
        node *p = tree;
        if (p->r) {
            if (p->l) {
                tree = p->r;
                tree->p = nullptr;
                node *x = tree;
                while (x->l) {
                    x = x->l;
                }
                x->l = p->l;
                p->l->p = x;
                splay(x);
                delete p;
                return root();
            }
            tree = p->r;
            tree->p = nullptr;
            delete p;
            return root();
        }
        if (p->l) {
            tree = p->l;
            tree->p = nullptr;
            delete p;
            return end();
        }
        delete p;
        tree = nullptr;
        return end();
    }
};
