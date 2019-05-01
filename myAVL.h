#ifndef DSA_CPP_DENG_MYAVL_H
#define DSA_CPP_DENG_MYAVL_H

#include "myBST.h"
#define Balanced(x) (stature((x).lChild) == stature((x).rChild))  //  理想平衡条件
#define BalFac(x) (stature((x).lChild) - stature((x).rChild))  // 平衡因子
#define AVLBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < -2))  // AVL平衡条件

/**
 * @return 返回节点的左右子树中的较高者，若相等则返回同侧者
 * 在AVL平衡前，借此确定重构方案
 */
#define tallerChild(x) (\
    stature((x).lChild) > stature((x).rChild) ? (x).lChild  : (\
        stature((x).lChild) < stature((x).rChild) ? (x).rChild : (\
            IiLChild(*(x)) ? (x).lChild : (x).rChild\
        )\
    )\
)

using namespace std;

template <typename T> class AVL : public BST<T> {
public:
    BinNodePosi(T)& insert(const T& e);
    bool remove(const T& e);
};
#endif //DSA_CPP_DENG_MYAVL_H
