#ifndef DSA_CPP_DENG_MYBINTREE_H
#define DSA_CPP_DENG_MYBINTREE_H

#include "myBinNode.h"

template <typename T> class BinTree {
protected:
    int _size;
    BinNodePosi(T) _root;
    virtual int updateHeight(BinNodePosi(T) x);    //更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);    //更新节点x及其祖先的高度
    static int removeAt(BinNodePosi(T) x);

public:
    BinTree() : _size(0), _root(nullptr) {}
    ~BinTree() {
        if(0 < _size)
            remove(_root);
    }
    int size() const {
        return _size;
    }
    bool empty() const {
        return !_root;
    }
    BinNodePosi(T) root() const {
        return _root;
    }
    BinNodePosi(T) insertAsRoot(T const& e);
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);    //e作为x的左孩子（原无）插入
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);    //e作为x的右孩子（原无）插入
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &e);    //T作为左子树插入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &e);    //T作为右子树插入
    int remove(BinNodePosi(T) x);    //删除以节点x为根的子树，返回其规模
    BinTree<T>* secede(BinNodePosi(T) x);    //将子树摘除，并将其转换为独立的子树
    template <typename VST> void travLevel(VST& visit) {    //层次遍历
        if(_root)
            _root->travLevel(visit);
    }
    template <typename VST> void travPre(VST& visit) {    //前序遍历
        if(_root)
            _root->travPre(visit);
    }
    template <typename VST> void travIn(VST& visit) {    //中序遍历
        if(_root)
            _root->travIn(visit);
    }
    template <typename VST> void travPost(VST& visit) {    //后序遍历
        if(_root)
            _root->travPost(visit);
    }

/* 比较器、判等器 */
    bool operator<(BinTree<T> const& t) {
        return _root && t._root && lt(_root, t._root);
    }
    bool operator>(BinTree<T> const& t) {
        return _root && t._root && !lt(_root, t._root);
    }
    bool operator==(BinTree<T> const& t) {
        return _root && t._root && (_root == t._root);
    }
    bool operator<=(BinTree<T> const& t) {
        return _root && t._root && (_root <= t._root);
    }
    bool operator>=(BinTree<T> const& t) {
        return _root && t._root && (_root >= t._root);
    }
};

template <typename T> int BinTree<T>::updateHeight(BinNode<T> *x) {
    return x->height = 1 + max(stature(x->lChild), stature(x->rChild));
}

template <typename T> void BinTree<T>::updateHeightAbove(BinNode<T> *x) {
    while(x) {
        updateHeight(x);
        x = x->parent;
    }
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRoot(const T &e) {
    _size = 1;
    return _root = new BinNode<T>(e);
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsLC(BinNode<T> *x, T const &e) {
    x->insertAsLC(e);
    updateHeightAbove(x);
    ++_size;
    return x->lChild;
}

template <typename T> BinNodePosi(T) BinTree<T>::insertAsRC(BinNode<T> *x, T const &e) {
    x->insertAsRC(e);
    updateHeightAbove(x);
    ++_size;
    return x->rChild;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNode<T> *x, BinTree<T> *&S) {
    if((x->lChild = S->_root))
        x->lChild->parent = x;
    _size += S->_size;
    updateHeightAbove(x);
    S->root = nullptr;
    S->_size = 0;
    release(S);
    S = NULL;
    return x;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNode<T> *x, BinTree<T> *&S) {
    if((x->rChild = S->_root))
        x->rChild->parent = x;
    _size += S->_size;
    updateHeightAbove(x);
    S->root = nullptr;
    S->_size = 0;
    release(S);
    S = NULL;
    return x;
}

template <typename T> int BinTree<T>::remove(BinNode<T> *x) {
    FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    int n = removeAt(x);
    _size -= n;
    return n;
}

template <typename T> int BinTree<T>::removeAt(BinNode<T> *x) {
    if(!x)
        return 0;
    int n = 1 + removeAt(x->lChild) + removeAt(x->rChild);
    release(x->data);
    release(x);
    return n;
}

template <typename T> BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {
    FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    BinTree<T>* S = new BinTree<T>();
    S->_root = x;
    S->_root->parent = nullptr;
    S->_size = x->size();
    _size = S->_size;
    return S;
}

#endif //DSA_CPP_DENG_MYBINTREE_H
