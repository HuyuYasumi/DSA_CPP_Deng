//
// Created by Natsu on 2018/10/14.
//

#ifndef DSA_CPP_DENG_MYLIST_H
#define DSA_CPP_DENG_MYLIST_H

#include <cstdlib>

#define Posi(T) ListNode<T>*

template <typename T> struct ListNode {
    T data;
    Posi(T) pred;
    Posi(T) succ;

    ListNode() {}

    ListNode(T e, Posi(T) p = NULL, Posi(T) s = NULL):
        data(e), pred(p), succ(s) {}

    Posi(T) insertAsPred(T const& e);

    Posi(T) insertAsSucc(T const& e);
};

#endif //DSA_CPP_DENG_MYLIST_H
