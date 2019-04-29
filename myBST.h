#ifndef DSA_CPP_DENG_MYBST_H
#define DSA_CPP_DENG_MYBST_H

#include "myBinTree.h"

template <typename T> class BST : public BinTree<T> {
public:
    //  以virtual修饰，强制要求所有子类（BTS）变种根据自身特性对其重写
    virtual BinNodePosi(T) & search(const T& e);    // 搜索
    virtual BinNodePosi(T) insert(const T& e);    // 插入
    virtual bool remove(const T& e);    // 删除

protected:
    BinNodePosi(T) _hot;    //BST::search()最后访问的非空（除非树空）的节点位置

    BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
            BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));    // 按照“3+4”结构联接3个节点和四颗子树

    BinNodePosi(T) rotateAt(BinNodePosi(T) x);    // 对x及其父母、祖先做统一旋转调整
};

#endif //DSA_CPP_DENG_MYBST_H
