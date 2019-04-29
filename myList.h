#ifndef DSA_CPP_DENG_MYLIST_H
#define DSA_CPP_DENG_MYLIST_H

#include <cstdlib>
#include "listNode.h"
typedef int Rank;

template <typename T> class List {
private:
    int _size;
    ListNodePosi(T) header;
    ListNodePosi(T) trailer;

protected:
    void init();
    int clear();
    void copyNodes(ListNodePosi(T), int);    // 复制列表中自位置p 起的n 项
    void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int);    // 有序列表区间归并
    void mergeSort(ListNodePosi(T)&, int);    // 对从p 开始连续的n 个节点归并排序
    void selectionSort(ListNodePosi(T), int);    // 对从p 开始连续的n 个节点选择排序
    void insertionSort(ListNodePosi(T), int);    // 对从p 开始连续的n 个节点插入排序

public:
/* 构造函数 */
    List() {
        init();
    }

    List(List<T> const& L);    // 复制列表L
    List(List<T> const& L, Rank r, int n);    // 复制列表L 中自第r 项起的n 项
    List(ListNodePosi(T) p, int n);    // 复制列表中自位置p 起的n 项

/* 析构函数 */
    ~List();

/* 只读访问接口 */
    Rank size() const { return _size; }
    bool empty() const { return _size <= 0; }
    T& operator[](Rank r) const;
    ListNodePosi(T) first() const { return header->succ; }
    ListNodePosi(T) last() const { return header->pred; }
    bool valid(ListNodePosi(T) p) {   // 判断位置p 是否对外合法
        return p && (trailer != p) && (header != p);
    }
    int disordered() const;
    ListNodePosi(T) find(T const& e) const {    // 无序列表查找
        return find(e, _size, trailer);
    }
    ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const;    // 无序区间查找
    ListNodePosi(T) search(T const& e) const {    // 有序列表查找
        return search(e, _size, trailer);
    }
    ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;    // 有序区间查找
    ListNodePosi(T) selectMax() {    // 整体最大者
        return selectMax(header->succ, _size);
    }
    ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);    // 在p 及其前n-1个后继中找出最大者

/* 可写访问接口 */
    ListNodePosi(T) insertAsFirst(T const& e);
    ListNodePosi(T) insertAsLast(T const& e);
    ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);
    ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e);
    T remove(ListNodePosi(T) p);    // 删除合法位置p 除的节点，返回被删除节点
    void merge(List<T>& L) {    // 全列表归并
        merge(first(), _size, L, L.first(), L._size);
    }
    void sort(ListNodePosi(T) p, int n);    // 列表区间排序
    void sort() { sort(first(), _size); }    // 列表整体排序
    int deduplicate();    // 无序去重
    int uniquify();    // 有序去重
    void reverse();    // 前后倒置

/* 遍历 */
    void traverse(void (*)(T&));    // 遍历，依次实施visit 操作（函数指针，只读或局部修改）
    template <typename VST>
        void traverse(VST&);    // 遍历，依次实施visit 操作（函数对象，只读或局部修改）
};

template <typename T> void List<T>::init() {
    header = new ListNode<T>();
    trailer = new ListNode<T>();
    header->succ = trailer;
    header->pred = nullptr;
    trailer->pred = header;
    trailer->succ = nullptr;
    _size = 0;
}

template <typename T> T& List<T>::operator[](Rank r) const {
    ListNodePosi(T) p = first();
    while(0 < r--)
        p = p->succ;
    return p->data;
}

template <typename T> ListNodePosi(T) List<T>::find(T const &e, int n, ListNodePosi(T) p) const {
    while(0 < n--) {
        if (e == (p = p->pred)->data)
            return p;
    }
    return nullptr;
}

template <typename T> ListNodePosi(T) List<T>::insertAsFirst(T const &e) {
    ++_size;
    return header->insertAsSucc(e);
}

template <typename T> ListNodePosi(T) List<T>::insertAsLast(T const &e) {
    ++_size;
    return trailer->insertAsPred(e);
}

template <typename T> ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const &e) {
    ++_size;
    return p->insertAsPred(e);
}

template <typename T> ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const &e) {
    ++_size;
    return p->insertAsSucc(e);
}

template <typename T> void List<T>::copyNodes(ListNodePosi(T) p, int n) {
    init();
    while(n--) {
        insertAsLast(p->data);
        p = p -> succ;
    }
}

template <typename T> List<T>::List(List<T> const &L) {
    copyNodes(L.first(), L._size);
}

template <typename T> List<T>::List(ListNodePosi(T) p, int n) {
    copyNodes(p, n);
}

template <typename T> List<T>::List(List<T> const &L, int r, int n) {
    copyNodes(L[r], n);
}

template <typename T> T List<T>::remove(ListNode<T> *p) {
    T e = p->data;
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p;
    --_size;
    return e;
}

template <typename T> List<T>::~List() {
    clear();
    delete &header;
    delete &trailer;
}

template <typename T> int List<T>::clear() {
    int oldSize = _size;
    while(0 < _size)
        remove(header->succ);
    return oldSize;
}

template <typename T> int List<T>::deduplicate() {
    if(_size < 2)
        return 0;
    int oldSize = _size;
    ListNodePosi(T) p = header;
    Rank r = 0;
    while(trailer != (p = p->succ)) {
        ListNodePosi(T) q = find(p->data, r, p);
        q ? remove(q) : r++;
    }
    return oldSize - _size;
}

template <typename T> void List<T>::traverse(void (*visit)(T&)) {
    for(ListNodePosi(T) p = header->succ; p != trailer; p = p ->ssucc)
        visit(p->data);
}

template <typename T> template <typename VST> void List<T>::traverse(VST& visit) {
    for(ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
        visit(p->data);
}

template <typename T> int List<T>::uniquify() {
    if(_size < 2)
        return 0;
    int oldSize = _size;
    ListNodePosi(T) p;
    ListNodePosi(T) q;
    for(p = header, q = p->succ; trailer != q; p = q, q = q->succ) {
        if(p->data == q->data) {
            remove(q);
            q = p;
        }
    }
    return oldSize - _size;
}

template <typename T> ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {
    while(0 <= n--)
        if(((p = p->pred)->data) <= e)
            break;
    return p;
}

template <typename T> void List<T>::sort(ListNodePosi(T) p, int n) {
    switch(rand() % 3) {
        case 1:
            insertionSort(p, n);
            break;
        case 2:
            selectionSort(p, n);
            break;
        default:
            mergeSort(p, n);
            break;
    }
}

template <typename T> void List<T>::insertionSort(ListNodePosi(T) p, int n) {
    for(int r = 0; r < n; ++r) {
        insertAfter(search(p->data, r, p), p->data);
        p = p->succ;
        remove(p->pred);
    }
}

template <typename T> void List<T>::selectionSort(ListNodePosi(T) p, int n) {
    ListNodePosi(T) head = p->pred;
    ListNodePosi(T) tail = p;
    for(int i = 0; i < n; i++)
        tail = tail->succ;
    while(1 < n--) {
        ListNodePosi(T) max = selectMax(head->succ, n);
        insertBefore(tail, remove(max));
        tail = tail->pred;
    }
}

template <typename T> ListNodePosi(T) List<T>::selectMax(ListNode<T> *p, int n) {
    ListNodePosi(T) max = p;
    for(ListNodePosi(T) cur = p; 1 < n; --n) {
        if(!lt((cur = cur->succ)->data, max->data)) {
            max = cur;
        }
    }
    return max;
}

template <typename T> void List<T>::mergeSort(ListNodePosi(T)& p, int n) {
    if(n < 2)
        return;
    int m = n >> 1;
    ListNodePosi(T) q = p;
    for(int i = 0; i < m; i++)
        q = q->succ;
    mergeSort(p, m);
    mergeSort(q, n - m);
    merge(p, m, *this, q, n - m);
}

template <typename T> void List<T>::merge(ListNodePosi(T)& p, int n,
        List<T>& L, ListNodePosi(T) q, int m) {
    ListNodePosi(T) pp = p->pred;
    while(0 < m) {
        if((0 < n) && (p->data <= q->data)) {
            if(q == (p = p->succ)) {
                break;
            }
            --n;
        } else {
            insertBefore((p, L.remove(q = q->succ)->pred));
            --m;
        };
    }
    p = pp->succ;
}

#endif //DSA_CPP_DENG_MYLIST_H
