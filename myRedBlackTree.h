#ifndef DSA_CPP_DENG_MYREDBLACKTREE_H
#define DSA_CPP_DENG_MYREDBLACKTREE_H

#include "myBST.h"

#define IsBlack(p) (!(p) || ((p)->color == RB_BLACK))  // 外部节点视作黑色
#define IsRed(p) (!IsBlack(p))
#define BlackHeightUpdated(x) (\
    (stature((x).lChild) == stature((x).rChild)) && \
        ((x).height == (IsRed(&x) ? stature((x).lChild) : stature((x).lChild) + 1)) \
)  // RedBlack 高度更新条件

using namespace std;

template <typename T> class RedBlack : public BST<T> {
public:
    void solveDoubleRed(BinNodePosi(T) x);  // 双红修正
    void solveDoubleBlack(BinNodePosi(T) x);  // 双黑修正
    int updateHeight(BinNodePosi(T) x);  // 更新节点 x 的高度

    BinNodePosi(T) insert(const T& e);  // 插入（重写）
    bool remove(const T& e);  // 删除（重写）
};

/**
 * 更新红黑树节点（黑）高度
 * 因统一定了 stature(NULL) = -1,故 height 比黑高度少一,好在不致影响到各种算法中的比较判断
 */
template<typename T>
int RedBlack<T>::updateHeight(BinNode<T> *x) {
    x->height = max(stature(x->lChild), stature(x->rChild));  // 孩子节点一般黑高度相同，除非出现双黑
    return IsRed(x) ? x->height : x->height++;
}

template<typename T>
BinNode<T> *RedBlack<T>::insert(const T &e) {
    BinNodePosi(T)& x = search(e);
    if(x)
        return x;
    x = new BinNode<T>(e, this->_hot, nullptr, nullptr, -1);  // 留意 _hot，留意 -1 的使用
    solveDoubleRed(x);
    return x;
}

template<typename T>
void RedBlack<T>::solveDoubleRed(BinNode<T> *x) {
    if(IsRoot(*x)) {  // 递归基，到达根节点则染成黑色，高度上升一层
        this->_root->color = RB_BLACK;
        this->_root->height++;
        return;
    }
    BinNodePosi(T) p = x->parent;
    if(IsBlack(p))
        return;
    BinNodePosi(T) g = p->parent;
    BinNodePosi(T) u = uncle(*x);
    if(IsBlack(u)) {
        if(IsLChild(*x) == IsLChild(*p)) {
            p->color = RB_BLACK;
        } else {  // 父子不同侧时
            x->color = RB_BLACK;
        }
        g->color = RB_RED;
        // 以上虽保证总共两次染色,但因增加了判断而得不偿失
        // 可改进为：在旋转后将根置黑、孩子置红,虽需三次染色但效率更高
        BinNodePosi(T) gg = g->parent;
        BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);  // 旋转，然后让曾祖父连接上旋转后的子树的根节点
        r->parent = gg;
    } else {  // 叔叔为红时
        // 无论父子同不同侧
        p->color = RB_BLACK;
        p->height++;
        u->color = RB_BLACK;
        u->height++;
        if(!IsRoot(*g))
            g->color = RB_RED;
        solveDoubleRed(g);
    }

}

template<typename T>
bool RedBlack<T>::remove(const T &e) {
    BinNodePosi(T)& x = search(e);
    if(!x)
        return false;
    BinNodePosi(T) r = removeAtStatic(x, this->_hot);  // r 为取代 x 者
    if(--this->_size <= 0)
        return true;
    // assert: _hot某一孩子刚被切除,且被r所指节点(可能是NULL)接替。以下检查是否失衡,幵做必要调整
    if(!this->_hot) {  // 若被删除的节点为根节点，则将其置黑，更新其高度
        this->_root->color = RB_BLACK;
        updateHeight(this->_root);
        return true;
    }
    // assert: 以下,原x(现r)必非根,_hot必非空
    if(BlackHeightUpdated(*(this->_hot)))  // 若所有祖先的黑高度依然平衡，则无需调整
        return true;
    if(IsRed(r)) {  // 若 r 为红，只需将其置黑，并更新其高度
        r->color = RB_BLACK;
        r->height++;
        return true;
    }
    // assert: 以下,原x(现r)均为黑色
    solveDoubleBlack(r);
    return true;
}

template<typename T>
void RedBlack<T>::solveDoubleBlack(BinNode<T> *x) {
    // todo
}

#endif //DSA_CPP_DENG_MYREDBLACKTREE_H
