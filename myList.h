#ifndef DSA_CPP_DENG_MYLIST_H
#define DSA_CPP_DENG_MYLIST_H

#include "listNode.h"
typedef int Rank;

template <typename T> class List {
private:
    int _size;
    ListNodePosi(T) header, trailer;

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
        return selectMax(header->succ, _szie);
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
    header = new ListNode<T>;
    trailer = new ListNode<T>;
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

#endif //DSA_CPP_DENG_MYLIST_H
