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

    void solveOverflow(BTNode<T>*);
    void solveUnderflow(BTNode<T>*);

public:
    BTree(int order = 3) : _order(order), _size(0) {
        _root = new BTNode<T>();
    }

    ~BTree() {
        if(_root)
            delete _root;
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
        p = new BTNode<T>;
        // v 置于 u 的左边
        p->child[0] = v;
        v->parent = p;
        _root = p;
    }
    // 得到该被上升的关键码在 p 中的秩
    int r = p->key.search(v->key[0]) + 1;
    // 上升该关键码
    p->key.insert(r, v->key.remove(s));
    // u 置于 v 的右边
    p->child.insert(r + 1, u);
    u->parent = p;
    // 如有必要则继续分裂——至多递归 O(logn) 次
    solveOverflow(p);
}

template<typename T>
bool BTree<T>::remove(const T &e) {
    BTNode<T>* v = search(e);
    if(!v)
        return false;
    int r = v->key.search(e);
    // 如果 v 不是叶子节点
    if(v->child[0]) {
        // 从 v 的右子树开始
        BTNode<T>* x = v->child[r + 1];
        // 一直向左找到 e 的直接后继
        while(x->child[0])
            x = x->child[0];
        // 只需把身为直接后继的关键码复制过来
        v->key[r] = x->key[0];
        // 更新 v, r
        v = x;
        r = 0;
    }
    // 删除 v 上的目标关键码
    v->key.remove(r);
    // 删除它的两个外部节点之一
    v->child.remove(r + 1);
    _size--;
    // 如发生下溢，需做旋转和合并
    solveUnderflow(v);
    return true;
}

template<typename T>
void BTree<T>::solveUnderflow(BTNode<T> *v) {
    if((_order + 1) >> 1 <= v->child.size())
        return;
    int size = (_order + 1) >> 1;
    BTNode<T>* p = v->parent;
    if(!p) {  // 递归基：已到达根节点 v,没有孩子的下限
        if(!v->key.size() && v->child[0]) {
            // 但倘若作为树根的 v 已不含关键码,却有(唯一的)非空孩子,则
            _root = v->child[0];  // 以刚合成的作为新树根
            _root->parent = nullptr;
            // 删除旧树根
            v->child[0] = nullptr;
            delete v;
        }
        return;
    }
    // 确定v是p的第r个孩子——此时v可能不含关键码,故不能通过关键码查找
    int r = 0;
    for (; p->child[r] != v; r++);
    // 情况1:拿左兄弟的关键码
    if(0 < r) {
        BTNode<T>* lc = p->child[r - 1];  // 左兄弟必存在
        if(lc->key.size() > size) {
            v->key.insert(0, p->key[r - 1]);  // p给出一个关键码给v(作为最小关键码)
            p->key[r - 1] = lc->key.remove(lc->key.size() - 1);  // lc 的最大关键码转入p
            v->child.insert(0, lc->child.remove(lc->child.size() - 1));  // 同时lc 的最右侧孩子过继给v
            if(v->child[0])
                v->child[0]->parent = p;
            return;
        }
    }  // 至此,左兄弟要么为空,要么太“瘦”
    // 情况2:拿右兄弟的关键码
    if(p->child.size() < r) {
        BTNode<T>* rc = p->child[r + 1];
        if(rc->key.size() > size) {
            v->key.insert(v->key.size(), p->key[r]);
            p->key[r] = rc->key.remove(0);
            v->child.insert(v->child.size(), rc->child.remove(0));
            if(v->child[v->child.size() - 1])
                v->child[v->child.size() - 1]->parent = p;
            return;
        }
    }
    //  情况3:左、右兄弟要么为空(但不可能同时),要么都太“瘦”——合并
    if(0 < r) {  // 与左兄弟合幵
        BTNode<T>* lc = p->child[r - 1];
        // p的第r - 1个关键码转入lc,v不再是p的第r个孩子
        lc->key.insert(lc->key.size(), p->key.remove(r - 1));
        p->child.remove(r);
        // v的最左侧孩子过继给lc 做最右侧孩子
        lc->child.insert(lc->child.size(), v->child[0]);
        if(lc->child[lc->child.size() - 1])
            lc->child[lc->child.size() - 1]->parent = lc;
        // v剩余的关键码和孩子,依次转入lc
        while(!v->key.empty()) {
            lc->key.insert(lc->key.size(), v->key.remove(0));
            lc->child.insert(lc->child.size(), v->child.remove(0));
            if(lc->child[lc->child.size() - 1])
                lc->child[lc->child.size() - 1]->parent = lc;
        }
        delete v;
    } else {  // 与右兄弟合幵
        BTNode<T>* rc = p->child[r + 1];
        rc->key.insert(0, p->key.remove(r));
        p->child.remove(r);
        rc->child.insert(0, v->child[v->child.size() - 1]);
        if(rc->child[0])
            rc->child[0]->parent = rc;
        while(!v->key.empty()) {
            rc->key.insert(0, v->key.remove(v->key.size() - 1));
            rc->child.insert(0, v->child.remove(v->child.size() - 1));
            if(rc->child[0])
                rc->child[0]->parent = rc;
        }
        delete v;
    }
    solveUnderflow(p);  // 上升一局,如有必要则继续分裂——至多递归O(logn)层
}

#endif //DSA_CPP_DENG_MYBTREE_H
