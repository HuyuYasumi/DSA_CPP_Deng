#ifndef DSA_CPP_DENG_MYENTRY_H
#define DSA_CPP_DENG_MYENTRY_H

template <typename K, typename V> struct Entry {
    K k;    // key
    V v;    // 数值

    explicit Entry(K k = K(), V v = V()): k(k), v(v) {}    //默认构造方法
    Entry(Entry<K, V> const& e) : k(e.k), v(e.v) {}    // 基于克隆的构造方法

    // 比较器
    bool operator>(Entry<K, V> const& e) {
        e.k > k;
    }
    bool operator<(Entry<K, V> const& e) {
        e.k < k;
    }

    // 判等器
    bool operator==(Entry<K, V> const& e) {
        e.k == k;
    }
    bool operator!=(Entry<K, V> const& e) {
        e.k != k;
    }
};

#endif //DSA_CPP_DENG_MYENTRY_H
