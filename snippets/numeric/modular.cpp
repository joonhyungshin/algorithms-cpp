template<class ModWrapper>
class modular {
private:
    using T = typename decay<decltype(ModWrapper::value)>::type;
    T value;

public:
    using value_type = T;

    constexpr modular() : value() {}

    constexpr static T mod() {
        return ModWrapper::value;
    }

    template<class V>
    static T norm(const V &x) {
        T val;
        if (x >= static_cast<V>(0)) {
            val = static_cast<T>(x < mod() ? x : x % mod());
        } else {
            val = static_cast<T>(-x <= mod() ? x : x % mod());
            if (val < static_cast<T>(0)) {
                val += mod();
            }
        }
        return val;
    }

    template<class V>
    modular(const V &x) : value(norm(x)) {}

    template<class V>
    explicit operator V() const {
        return static_cast<V>(value);
    }

    modular &operator+=(const modular &other) {
        if (value < mod() - other.value) {
            value += other.value;
        } else {
            value -= mod() - other.value;
        }
        return *this;
    }

    modular &operator-=(const modular &other) {
        if (value >= other.value) {
            value -= other.value;
        } else {
            value += mod() - other.value;
        }
        return *this;
    }

    template<class V = T>
    typename enable_if<(sizeof(V) <= 4), modular>::type &operator*=(const modular &other) {
        value = norm(static_cast<uint64_t>(value) * static_cast<uint64_t>(other.value));
        return *this;
    }

    template<class V = T>
    typename enable_if<(sizeof(V) > 4 && sizeof(V) <= 8), modular>::type &operator*=(const modular &other) {
        using u128 = unsigned __int128;
        value = norm(static_cast<u128>(value) * static_cast<u128>(other.value));
        return *this;
    }

    modular inverse() const {
        T u = static_cast<T>(0), v = static_cast<T>(1);
        T a = value, m = mod();
        while (a != static_cast<T>(0)) {
            T t = m / a;
            m -= t * a;
            swap(a, m);
            u -= t * v;
            swap(u, v);
        }
        assert(m == 1);
        return modular(u);
    }

    modular &operator/=(const modular &other) {
        return *this *= other.inverse();
    }

    modular operator+(const modular &other) const {
        return modular(*this) += other;
    }

    modular operator-(const modular &other) const {
        return modular(*this) -= other;
    }

    modular operator*(const modular &other) const {
        return modular(*this) *= other;
    }

    modular operator/(const modular &other) const {
        return modular(*this) /= other;
    }

    bool operator==(const modular &other) const {
        return value == other.value;
    }

    bool operator!=(const modular &other) const {
        return !(*this == other);
    }

    template<class V>
    modular pow(const V &b) {
        bool inv = b < static_cast<V>(0);
        modular res(static_cast<T>(1));
        modular x(inv ? inverse() : *this);
        V p = inv ? -b : b;
        while (p > static_cast<V>(0)) {
            if (p & 1) {
                res *= x;
            }
            x *= x;
            p >>= 1;
        }
        return res;
    }

    template<class V> modular &operator+=(const V &other) { return *this += modular(other); }
    template<class V> modular &operator-=(const V &other) { return *this -= modular(other); }
    template<class V> modular &operator*=(const V &other) { return *this *= modular(other); }
    template<class V> modular &operator/=(const V &other) { return *this /= modular(other); }
    template<class V> modular operator+(const V &other) const { return *this + modular(other); }
    template<class V> modular operator-(const V &other) const { return *this - modular(other); }
    template<class V> modular operator*(const V &other) const { return *this * modular(other); }
    template<class V> modular operator/(const V &other) const { return *this / modular(other); }
    modular &operator++() { return *this += 1; }
    modular &operator--() { return *this -= 1; }
    modular operator++(int) { modular res(*this); *this += 1; return res; }
    modular operator--(int) { modular res(*this); *this -= 1; return res; }
    modular operator-() const { return modular(-value); }
    T operator()() const { return value; }

    template<class W>
    friend istream &operator>>(istream &stream, modular<W> &n);
};

template<class V, class W> modular<W> operator+(const V &lhs, const modular<W> &rhs) { return modular<W>(lhs) + rhs; }
template<class V, class W> modular<W> operator-(const V &lhs, const modular<W> &rhs) { return modular<W>(lhs) - rhs; }
template<class V, class W> modular<W> operator*(const V &lhs, const modular<W> &rhs) { return modular<W>(lhs) * rhs; }
template<class V, class W> modular<W> operator/(const V &lhs, const modular<W> &rhs) { return modular<W>(lhs) / rhs; }
template<class V, class W> bool operator==(const V &lhs, const modular<W> &rhs) { return modular<W>(lhs) == rhs; }
template<class V, class W> bool operator!=(const V &lhs, const modular<W> &rhs) { return modular<W>(lhs) != rhs; }

template<class W>
ostream &operator<<(ostream &stream, const modular<W> &n) {
    return stream << n();
}

template<class W>
istream &operator>>(istream &stream, modular<W> &n) {
    typename common_type<typename modular<W>::value_type, int64_t>::type x;
    stream >> x;
    n.value = modular<W>::norm(x);
    return stream;
}
