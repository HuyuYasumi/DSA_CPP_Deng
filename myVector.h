#ifndef DSA_CPP_DENG_MYVECTOR_H
#define DSA_CPP_DENG_MYVECTOR_H

#include <cstdlib>
#define DEFAULT_CAPACITY 3

typedef int Rank;

template <typename T> class Vector {
protected:
    Rank _size;
    int _capacity; //容量
    T* _elem; //数据区

    void copyFrom(T* const A, Rank lo, Rank hi);
    void expand(); //扩容
    void shrink(); //压缩
    bool bubble(Rank lo, Rank hi); //交换
    void bubbleSort(Rank lo, Rank hi); //冒泡排序
    Rank max(Rank lo, Rank hi);
    void selectionSort(Rank lo, Rank hi); //选择排序
    void merge(Rank lo, Rank mi, Rank hi); //归并算法
    void mergeSort(Rank lo, Rank hi); //归并排序
    Rank partition(Rank lo, Rank hi); //轴点构造算法
    void quickSort(Rank lo, Rank hi); //快速排序
    void heapSort(Rank lo, Rank hi); //堆排序

public:
/* 构造函数 */
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) {
        _elem = new T[_capacity = c];
        for(_size = 0; _size < s; _elem[_size++] = v);
    }

    Vector(T const* A, Rank lo, Rank hi) {
        copyFrom(A, lo, hi);
    }

    Vector(T const* A, Rank n) {
        copyFrom(A, 0, n);
    }

    Vector(Vector<T> const& V, Rank lo, Rank hi) {
        copyFrom(V._elem, lo, hi);
    }

    Vector(Vector<T> const& V) {
        copyFrom(V._elem, 0, V._size);
    }

/* 析构函数 */
    ~Vector() {
        delete [] _elem;
    }

/* 只读接口 */
    Rank size() const {
        return _size;
    }
    bool empty() const {
        return !_size;
    }
    int disordered() const; //判断向量是否已排序

/* 查找 */
    Rank find(T const& e) const { //无序向量整体查找
        return find(e, 0, (Rank)_size);
    }
    Rank find(T const& e, Rank lo, Rank hi) const; //无序向量区间查找
    Rank search(T const& e) const { //有序向量整体查找
        return (0 >= _size) ? -1 : serach(e, (Rank)0, (Rank)_size);
    }
    Rank search(T const& e, Rank lo, Rank hi) const; //有序向量区间查找

/* 操作符重载 */
    T& operator [] (Rank r) const; //重载下标操作符
    Vector<T> & operator = (Vector<T> const&); //重载赋值操作符

/* 删除 */
    T remove(Rank r);
    int remove(Rank lo, Rank hi);

/* 插入 */
    Rank insert(Rank r, T const& e);
    Rank insert(T const& e) {
        return insert(_size, e);
    }

/* 排序 */
    void sort(Rank lo, Rank hi);
    void sort() {
        sort(0, _size);
    }
    void unsort(Rank lo, Rank hi);
    void unsort() {
        unsort(0, _size);
    }

/* 去重 */
    int deduplicate(); //无序去重
    int uniquify(); //有序去重

/* 遍历 */
    void traverse(void (*)(T&));
    template <typename VST> void traverse(VST&);
}; //Vector

template <typename T>
    void Vector<T>::copyFrom(T * const A, Rank lo, Rank hi) {
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while(lo < hi)
        _elem[_size++] = A[lo++];
}

template <typename T> void Vector<T>::expand() {
    if(_size < _capacity) return;
    _capacity = max(_capacity, DEFAULT_CAPACITY);
    T* oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for(int i = 0; i < _size; i++)
        _elem[i] = oldElem[i];
    delete [] oldElem;
}

template <typename T> T & Vector<T>::operator[](Rank r) const {
    return _elem[r];
}

template <typename T> Rank Vector<T>::insert(Rank r, T const & e) {
    expand();
    for(int i = _size; i > r; i--)
        _elem[i] = _elem[i-1];
    _elem[r] = e;
    ++_size;
    return r;
}

template <typename T> int Vector<T>::remove(Rank lo, Rank hi) {
    if(lo == hi)
        return 0;
    while(hi < _size)
        _elem[lo++] = _elem[hi++];
    _size = lo;
    shrink();
    return hi - lo;
}

template <typename T>
    Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const {
    while((lo < hi--) && e != _elem[hi]);
    return hi;
}

template <typename T> T Vector<T>::remove(Rank r) {
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}

template <typename T> int Vector<T>::deduplicate() {
    int oldSize = _size;
    Rank i = 1;
    while(i < _size)
        (find(_elem[i], 0, i) < 0) ? i++ : remove(i);
    return oldSize - _size;
}

template <typename T> void Vector<T>::traverse(void (*visit)(T&)) {
    for(int i = 0; i < _size; i++)
        visit(_elem[i]);
}

template <typename T> template <typename VST>
    void Vector<T>::traverse(VST &visit) {
    for(int i = 0; i < _size; i++)
        visit(_elem[i]);
}

template <typename T> int Vector<T>::disordered() const {
    int n = 0;
    for(int i = 1; i < _size; i++)
        n += (_elem[i - 1] > _elem[i]);
    return n;
}

template <typename T> int Vector<T>::uniquify() {
    Rank i = 0, j = 0;
    while(++j < _size)
        if(_elem[i] != _elem[j])
            _elem[++i] = _elem[j];
    _size = ++i;
    shrink();
    return j - i;
}

template <typename T>
    Rank Vector<T>::search(T const & e, Rank lo,Rank hi) const {
    return (rand() % 2) ? binSearch(_elem, e, lo, hi)
                        : fibSearch(_elem, e, lo, hi);
}

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
    switch(rand() % 5) {
        case 1:
            bubbleSort(lo, hi);    //起泡排序
            break;
        case 2:
            selectionSort(lo, hi);    //选择排序
            break;
        case 3:
            mergeSort(lo, hi);    //归并排序
            break;
        case 4:
            heapSort(lo, hi);    //堆排序
            break;
        default:
            quickSort(lo, hi);    //快速排序
            break;
    }
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
    while(!bubble(lo , hi--));
}

template <typename T> bool Vector<T>::bubble(Rank lo, Rank hi) {
    Rank last = lo;
    while(++lo < hi)
        if(_elem[lo - 1] > _elem[lo]) {
            last = lo;
            swap(_elem[lo - 1], _elem[lo]);
        }
    return last;
}

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
    if(hi - lo < 2)
        return;
    int mi = (lo + hi) >> 1;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
    T* A = _elem + lo;
    int lb = mi - lo;
    T* B = new T[lb];

    for(Rank i = 0; i < lb; B[i] = A[i++]);

    int lc = hi - mi;
    T* C = _elem + mi;

    for(Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);) {
        if((j < lb) && (lc <= k || (B[j] <= C[k])))
            A[i++] = B[j++];
        if((k < lc) && (lb <= j ||(C[k] < B[j])))
            B[i++] = C[k++];
    }
    delete [] B;
}

template <typename T>
static Rank binSearch(T* A, T const & e, Rank lo, Rank hi) {
    while(lo < hi) {
        Rank mi = (lo + hi) >> 1;
        (e < A[mi]) ? hi = mi : lo = mi + 1;
    }
    return --lo;
}

#endif //DSA_CPP_DENG_MYVECTOR_H
