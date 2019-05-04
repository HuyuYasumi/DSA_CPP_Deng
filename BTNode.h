#ifndef DSA_CPP_DENG_BTNODE_H
#define DSA_CPP_DENG_BTNODE_H

#include "myVector.h"

/**
 * B-树节点模板类
 */
template <typename T> struct BTNode {
protected:
    BTNode* parent;
    // 各相邻孩子之间的关键码组织为一个向量
    Vector<T> key;
    // 同一节点的所有孩子组织为一个向量
    Vector<BTNode*> child;

public:
    /**
     * 构造函数
     * 注意：BTNode 只能作为根节点创建，而且初始时只有０个关键码，１个空的孩子指针
     */
    BTNode() {
        parent = nullptr;
        child.insert(0, nullptr);
    }
    BTNode(T e, BTNode* lc = nullptr, BTNode* rc = nullptr) {
        key.insert(0, e);
        child.insert(0, lc);
        child.insert(1, rc);
        if(lc)
            lc->parent = this;
        if(rc)
            rc->parent = this;
    }
};

#endif //DSA_CPP_DENG_BTNODE_H
