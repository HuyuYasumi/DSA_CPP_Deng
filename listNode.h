#ifndef DSA_CPP_DENG_LISTNODE_H
#define DSA_CPP_DENG_LISTNODE_H

#define ListNodePosi(T) ListNode<T>*

template <typename T> struct ListNode {
    T data;
    ListNodePosi(T) pred;
    ListNodePosi(T) succ;

/* 构造函数 */
    ListNode() = default;

    ListNode(T e, ListNodePosi(T) p = nullptr, ListNodePosi(T) s = nullptr):
        data(e), pred(p), succ(s) {}

/* 操作接口 */
    ListNodePosi(T) insertAsPred(T const& e);

    ListNodePosi(T) insertAsSucc(T const& e);
};

template <typename T> ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) {
    ListNodePosi(T) x = new ListNode(e, pred, this);
    pred->succ = x;
    return pred = x;
}

template <typename T> ListNodePosi(T) ListNode<T>::insertAsSucc(T const &e) {
    ListNodePosi(T) x = new ListNode(e, this, succ);
    succ->pred = x;
    return succ = x;
}

#endif //DSA_CPP_DENG_LISTNODE_H
