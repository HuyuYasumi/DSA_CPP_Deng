#ifndef DSA_CPP_DENG_MYAVL_H
#define DSA_CPP_DENG_MYAVL_H

#include "myBST.h"
#define Balanced(x) (stature((x).lChild) == stature((x).rChild))  //  理想平衡条件
#define BalFac(x) (stature((x).lChild) - stature((x).rChild))  // 平衡因子
#define AVLBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))  // AVL平衡条件

/**
 * @return 返回节点的左右子树中的较高者，若相等则返回同侧者
 * 在AVL平衡前，借此确定重构方案
 */
#define tallerChild(x) (\
    stature((x).lChild) > stature((x).rChild) ? (x).lChild  : (\
        stature((x).lChild) < stature((x).rChild) ? (x).rChild : (\
            IsLChild((x)) ? (x).lChild : (x).rChild\
        )\
    )\
)

using namespace std;

template <typename T> class AVL : public BST<T> {
public:
    BinNodePosi(T)& insert(const T& e);
    bool remove(const T& e);
};

template <typename T> BinNodePosi(T)& AVL<T>::insert(const T &e) {
    BinNodePosi(T)& x = this->search(e);
    if(x)
        return x;
    x = new BinNode<T>(e, this->_hot);
    this->_size++;
    for(BinNodePosi(T) g = this->_hot; g ; g = g->parent) {
        //  一旦失衡，则（采用”3 +　４“算法）使其复衡
        if(!AVLBalanced(*g)) {
            // 将该子树联结至原父亲
            BinNodePosi(T)& tmp = FromParentTo(*g);
            tmp = this->rotateAt(tallerChild(*tallerChild(*g)));
            break;
        // 由于上面的 break 语句，下面的分支等价于（把 updateHeight(g) 从分支移开）：
        // }
        // updateHeight(g);
        } else {
            this->updateHeight(g);  // 更新其高度（注意，即使未失衡，其高度仍可能增加）
        }
    }
    return x;
}

template <typename T> bool AVL<T>::remove(const T &e) {
    BinNodePosi(T)& x = this->search(e);
    if(!x)
        return false;
    removeAtStatic(x, this->_hot);
    this->_size--;
    for(BinNodePosi(T) g = this->_hot; g ; g = g->parent) {
        //  一旦失衡，则（采用”3 +　４“算法）使其复衡
        if(!AVLBalanced(*g)) {
            // 将该子树联结至原父亲
            BinNodePosi(T)& tmp = FromParentTo(*g);
            g = tmp = this->rotateAt(tallerChild(*tallerChild(*g)));
        }
        this->updateHeight(g);  //  更新其高度（注意，即使ｇ位失衡，高度亦可能降低）
    }
    return true;
}

#endif //DSA_CPP_DENG_MYAVL_H
