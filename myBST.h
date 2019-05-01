#ifndef DSA_CPP_DENG_MYBST_H
#define DSA_CPP_DENG_MYBST_H

#include <iostream>
#include "myBinTree.h"
using namespace std;

template <typename T> class BST : public BinTree<T> {
public:
    //  以virtual修饰，强制要求所有子类（BTS）变种根据自身特性对其重写
    virtual BinNodePosi(T)& search(const T& e);    // 搜索
    virtual BinNodePosi(T)& insert(const T& e);    // 插入
    virtual bool remove(const T& e);    // 删除

protected:
    BinNodePosi(T) _hot;    //BST::search()最后访问的非空（除非树空）的节点位置

    BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
            BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));    // 按照“3+4”结构联接3个节点和四颗子树

    BinNodePosi(T) rotateAt(BinNodePosi(T) v);    // 对x及其父母、祖先做统一旋转调整
};

/**
 * 全局静态模板函数,适用于AVL、Splay、RedBlack等各种BST
 */
template <typename T> static BinNodePosi(T)& searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot) {
    if(!v || v->data == e)
        return v;
    hot = v;
    return searchIn((v->data > e) ? v->lChild : v->rChild, e, hot);
}

/**
 * 全局静态模板函数,适用于AVL、Splay、RedBlack等各种BST
 * BST节点删除算法：删除x 指定的节点
 * x 由此前的调用决定，并确认非NULL 后方调用次算法，故必删除成功
 * hot 最后指向接替者的父亲
 * @return 返回实际被删除节点的接替者，可能为nullptr
 */
template <typename T> static BinNodePosi(T) removeAtStatic(BinNodePosi(T)&x, BinNodePosi(T)&hot) {
    BinNodePosi(T) w = x;
    BinNodePosi(T) succ = nullptr;
    if(!HasLChild(w))
        succ = x = w->lChild;
    else if(!HasRChild(w))
        succ = x = w->rChild;
    else {
        w = w->succ();
        swap(x->data, w->data);
        BinNodePosi(T) u = w->parent;
        (u != x ? u->lChild : u->rChild) = succ = w->rChild;
    }
    hot = w->parent;
    if(succ)
        succ->parent = hot;
    release(w);
    return succ;
}

template <typename T> BinNodePosi(T)& BST<T>::search(const T &e) {
    return searchIn(this->_root, e, _hot);
}

template <typename T> BinNodePosi(T)& BST<T>::insert(const T& e) {
    BinNodePosi(T)& x = search(e);
    if(x)
        return x;
    x = new BinNode<T>(e);
    x->parent = _hot;
    this->_size++;
    this->updateHeightAbove(x);  // 更新x 及其历代祖先的高度
    return x;
}

template <typename T> bool BST<T>::remove(const T &e) {
    BinNodePosi(T)& x = search(e);
    if(!x)
        return false;
    removeAtStatic(x, _hot);
    this->_size--;
    this->updateHeightAbove(_hot);
    return true;
}

/**
 * 按照（３＋４）结构联结３个节点和４棵子树
 * @return 返回联结后的局部子树的根节点（即ｂ）
 * 子树根节点与它上层节点的双向联结，均须由上层调用者完成
 * 可用于ＡＶＬ、ＲｅｄＢｌａｃｋ的局部平衡调整
 */
template<typename T>
BinNode<T> *BST<T>::connect34(BinNode<T> * a, BinNode<T> * b, BinNode<T> *c,
        BinNode<T> * T0, BinNode<T> *T1, BinNode<T> *T2, BinNode<T> *T3) {
    a->lChild = T0;
    if(T0)
        T0->parent = a;
    a->rChild = T1;
    if(T1)
        T1->parent = a;
    this->updateHeight(a);
    c->lChild = T2;
    if(T2)
        T2->parent = c;
    c->rChild = T3;
    if(T3)
        T3->parent = c;
    this->updateHeight(c);
    b->lChild = a;
    a->parent = b;
    b->rChild = c;
    c->parent = b;
    this->updateHeight(b);
    return b;
}

/**
 * ＢＳＴ节点旋转变换统一算法（３节点＋４子树）
 * 注意，尽管子树根会正确指向上层节点（如果存在），但反向的联结须由上层调用者完成
 * @param v 非空的孙辈节点
 * @return 返回调整后的局部子树根节点的位置
 */
template<typename T>
BinNode<T> *BST<T>::rotateAt(BinNode<T> *v) {
    BinNodePosi(T) p = v->parent;
    BinNodePosi(T) g = p->parent;
    if(IsLChild(*p)) {
        if(IsLChild(*v)) {  // zig, zig
            p->parent = g->parent;  // 局部根节点向上联结
            return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
        } else {  // zag, zig
            v->parent = g->parent;  // 局部根节点向上联结
            return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
        }
    } else {
        if(IsRChild(*v)) {  // zag, zag
            p->parent = g->parent;  // 局部根节点向上联结
            return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
        } else {  // zig, zag
            v->parent = g->parent;  // 局部根节点向上联结
            return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
        }
    }
}

#endif //DSA_CPP_DENG_MYBST_H
