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

#endif //DSA_CPP_DENG_MYREDBLACKTREE_H
