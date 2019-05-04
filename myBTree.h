#ifndef DSA_CPP_DENG_MYBTREE_H
#define DSA_CPP_DENG_MYBTREE_H

#include "BTNode.h"
using namespace std;

/**
 * B-树模板类
 */
template <typename T> class BTree {
protected:
    int _size;  // 关键码数
    int _order;  // 阶数，默认为３
    BTNode<T>* _root;
    BTNode<T>* _hot;  // 最后访问的非空（除非树空）节点

    // todo
    void solveOverflow(BTNode<T>*);
    void solveUnderflow(BTNode<T>*);

public:
    BTree(int order = 3) : _order(3), _size(0) {
        _root = new BTNode<T>();
    }

    ~BTree() {
        if(_root)
            // todo
            release(_root);
    }

    int const order() {
        return _order;
    }

    int const size() {
        return _size;
    }

    BTNode<T>*& root() {
        return _root;
    }

    bool empty() {
        return !_root;
    }

    BTNode<T>* search(const T& e);
    bool insert(const T& e);
    // todo
    bool remove(const T& e);
};

/**
 * 与二叉搜索树的实现类似，这里也约定查找结果由返回的节点位置指代：成功时返回目标关
 * 键码所在的节点，上层调用过程可在该节点内进一步查找以确定准确的命中位置；失败时返回对
 * 应外部节点，其父节点则由变量 _hot 指代。
 */
template<typename T>
BTNode<T> *BTree<T>::search(const T &e) {
    BTNode<T>* v = _root;
    _hot = nullptr;
    while(v) {
        int r = v->key.search(e);
        if(0 <= r && e == v->key[r])
            return v;
        _hot = v;
        // 此处的[r + 1]利用了ＢＳＴ的性质，体现了 key 与 child　的对应关系
        v = v->child[r + 1];  // 转入对应子树（_hot 指向其父）－－需做 I/O，最费时间
    }
    return nullptr;
}

template<typename T>
bool BTree<T>::insert(const T &e) {
    BTNode<T>* v = search(e);
    if(v)
        return false;
    // 此时 v 必然为 nullptr，_hot 才是该被插入新关键码的节点
    int r = _hot->key.search(e);
    _hot->key.insert(r + 1, e);
    _hot->child.insert(r + 2, nullptr);
    ++_size;
    // 对 _hot 做是否上溢（关键码总数大于 m - 1 ）的判断，若是则对其执行分裂操作
    solveOverflow(_hot);
    return true;
}

template<typename T>
void BTree<T>::solveOverflow(BTNode<T> *v) {
    if(v->child.size() <= _order)
        return;
    // 轴点(此时应有 _order = key.size() = child.size() - 1)
    int s = _order >> 1;
    // 注意：新节点已有一个空孩子
    BTNode<T>* u = new BTNode<T>();
    // v 右侧的 _order-s-1 个孩子指针及关键码分裂为右侧节点 u
    for (int i = 0; i < _order - s - 1; i++) {
        u->child.insert(i, v->child.remove(s + 1));
        u->key.insert(i, v->key.remove(s + 1));
    }
    // 移动 v 最靠右的孩子到 u
    u->child.insert(_order - s - 1, v->child.remove(s + 1));
    // 统一 u 孩子的父节点为 u
    if(u->child[0])
        for (int i = 0; i < _order - s; i++)
            u->child[i]->parent = u;
    // v 当前的父节点 p
    BTNode<T>* p = v->parent;
    // 若p为空则创建之
    if(!p) {
        p = new BTNode<T>*;
        // v 置于 u 的左边
        p->child[0] = v;
        v->parent = p;
    }
    // 得到该被上升的关键码在 p 中的秩
    int r = p->key.search(v->key[0]) + 1;
    // 上升该关键码
    p->key.insert(r, v->key.remove(s));
    // u 置于 v 的右边
    p->child.insert(r + 1, u);
    u->key = p;
    // 如有必要则继续分裂——至多递归 O(logn) 次
    solveOverflow(p);
}

#endif //DSA_CPP_DENG_MYBTREE_H
