#ifndef DSA_CPP_DENG_MYSPALY_H
#define DSA_CPP_DENG_MYSPALY_H

#include "myBST.h"

using namespace std;

/**
 * 基于 BST 实现 Splay
 */
template <typename T> class Splay : public BST<T> {
protected:
    BinNodePosi(T) splay(BinNodePosi(T) v);  // 将节点ｖ伸展至树根
public:
    BinNodePosi(T)& insert(const T& e);
    BinNodePosi(T)& search(const T& e);
    bool remove(const T& e);
};

/**
 * Splay 树伸展算法
 * @param v 最近访问需伸展的节点
 * @return 返回被伸展的节点以便上层更新它为新树根
 */
template<typename T>
BinNode<T> *Splay<T>::splay(BinNode<T> *v) {
    if(!v)
        return nullptr;
    BinNodePosi(T) p;
    BinNodePosi(T) g;
    while((p = v->parent) && (g = p->parent)) {
        BinNodePosi(T) gg = g->parent;
        if(IsLChild(*p)) {
            if(IsLChild(*v)) {  // zig, zig
                attachAsLChild(g, p->rChild);
                attachAsLChild(p, v->rChild);
                attachAsRChild(p, g);
                attachAsRChild(v, p);
            } else {  // zag, zig
                attachAsLChild(g, v->rChild);
                attachAsRChild(p, v->lChild);
                attachAsLChild(v, p);
                attachAsRChild(v, g);
            }
        } else {
            if(IsRChild(*v)) {  // zag, zag
                attachAsRChild(g, p->lChild);
                attachAsRChild(p, v->lChild);
                attachAsLChild(p, g);
                attachAsLChild(v, p);
            } else {  // zig, zag
                attachAsRChild(g, v->lChild);
                attachAsLChild(p, v->rChild);
                attachAsLChild(v, g);
                attachAsRChild(v, p);
            }
        }
        // 向上联结
        if(!gg)  // g 可能为树根，即 gg == nullptr
            v->parent = nullptr;
        else {
            if(g == gg->lChild)
                attachAsLChild(gg, v);
            else
                attachAsRChild(gg, v);
        }
        this->updateHeight(g);
        this->updateHeight(p);
        this->updateHeight(v);
    }
    // 双层伸展结束后，必有 g == nullptr，但 p 可能非空
    if((p = v->parent)) {  // 若果真非空，则再做一次单旋
        if(IsLChild(*v)) {
            attachAsLChild(p, v->rChild);
            attachAsRChild(v, p);
        } else {
            attachAsRChild(p, v->lChild);
            attachAsLChild(v, p);
        }
        this->updateHeight(p);
        this->updateHeight(v);
    }
    v->parent = nullptr;
    return v;
}

template<typename T>
BinNode<T> *&Splay<T>::search(const T &e) {
    BinNodePosi(T)& x = searchIn(this->_root, e, this->_hot = nullptr);
    this->_root = splay(x ? x : this->_hot);
    return this->_root;
}

/**
 * @return 无论 e 是否存在于原树中，返回的节点总为 _root 且 _root->data == e
 */
template<typename T>
BinNode<T> *&Splay<T>::insert(const T &e) {
    if(!this->_root) {
        this->_size++;
        return (this->_root = new BinNode<T>(e));
    }
    //  search()已集成了splay()伸展功能,故查找返回后,树根节点
    //  要么等于查找目标(查找成功),要么就是_hot,而且恰为拟插入节点的直接前驱或直接后继
    //  (查找失败)。
    if((e == (search(e))->data))
        return this->_root;
    this->_size++;
    BinNode<T>* t = this->_root;
    if(t->data < e) {
        t->parent = this->_root = new BinNode<T>(e, nullptr, t, t->rChild);
        if(HasRChild(t)) {
            t->rChild->parent = this->_root;
            t->rChild = nullptr;
        }
    } else {
        t->parent = this->_root = new BinNode<T>(e, nullptr, t->lChild, t);
        if(HasLChild(t)) {
            t->lChild->parent = this->_root;
            t->lChild = nullptr;
        }
    }
    //  尽管伸展树并不需要记录和维护节点高度,为与其它平衡二叉搜索树的实现保持统一,这里
    //  还是对节点的高度做了及时的更新。出于效率的考虑,实际应用中可视情况,省略这类更新。
    this->updateHeightAbove(t);
    return this->_root;
}

template<typename T>
bool Splay<T>::remove(const T &e) {
    if(!this->_root || (e != (search(e))->data))
        return false;
    BinNodePosi(T) w = this->_root;  // assert: 经search()后节点e已被伸展至树根
    if(!HasLChild(w)) {
        this->_root = this->_root->rChild;
        if(this->_root)
            this->_root->parent = nullptr;
    } else if(!HasRChild(w)) {
        this->_root = this->_root->lChild;
        if(this->_root)
            this->_root->parent = nullptr;
    } else {
        // 暂时将左子树切除
        BinNodePosi(T) lTree = w->lChild;
        lTree->parent = nullptr;
        w->lChild = nullptr;

        // 只保留右子树
        this->_root = this->_root->rChild;
        this->_root->parent = nullptr;

        // 将此时的树（即原来的右子树）最小值伸展至根，该节点为被删除根的直接后继，此时根的左子树必为空
        search(w->data);

        this->_root->lChild = lTree;
        lTree->parent = this->_root;
    }
    release(w);
    this->_size--;
    if(this->_root)
        this->updateHeight(this->_root);
    return true;
}


/**
 * 为 p 和 rc 建立父（左）子关系
 */
template <typename NodePosi> inline void attachAsLChild(NodePosi p, NodePosi lc) {
    p->lChild = lc;
    if(lc)
        lc->parent = p;
}

/**
 * 为 p 和 rc 建立父（右）子关系
 */
template <typename NodePosi> inline void attachAsRChild(NodePosi p, NodePosi rc) {
    p->rChild = rc;
    if(rc)
        rc->parent = p;
}


#endif //DSA_CPP_DENG_MYSPALY_H
