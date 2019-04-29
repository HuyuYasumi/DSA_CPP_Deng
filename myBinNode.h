#ifndef DSA_CPP_DENG_MYBINNODE_H
#define DSA_CPP_DENG_MYBINNODE_H

#include <cstdlib>
#include "myStack.h"
#include "myQueue.h"

#define BinNodePosi(T) BinNode<T>*
#define max(x, y) (x > y ? x : y)
#define stature(p) ((p) ? (p)->height : -1)
#define IsRoot(x) (!(x).parent)
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lChild))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x)->lChild)
#define HasRChild(x) ((x)->rChild)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
#define sibling(p) ((IsLChild(*p)) ? (p)->parent->rChild : (p)->parent->lChild)    //返回兄弟
#define uncle(x) (sibling((x).parent))    //返回叔叔
#define FromParentTo(x) (IsRoot(x) ? _root : \
        (IsLChild(x) ? (x).parent->lChild : (x).parent->rChild))    //来自父亲的指针

typedef enum {RB_RED, RB_BLACK} RBColor;

template <typename T> struct BinNode {
    T data;
    BinNodePosi(T) parent;
    BinNodePosi(T) lChild;
    BinNodePosi(T) rChild;
    int height;
    int npl;    //Null Path Length(空节点通路长度，左式堆，也可直接用height代替)
    RBColor color;

/* 构造函数 */
    BinNode() : parent(nullptr), lChild(nullptr), rChild(nullptr), height(0), npl(1),
            color(RB_RED) {}
    explicit BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr,
            BinNodePosi(T) rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED)
            : data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) {}

/* 操作接口 */
    int size();    //统计当前节点后代总数，亦即以其为根的子树的规模
    BinNodePosi(T) insertAsLC(T const&);
    BinNodePosi(T) insertAsRC(T const&);
    BinNodePosi(T) succ();
    template <typename VST> void travLevel(VST&);    //子树层次遍历
    template <typename VST> void travPre(VST&);    //子树前序遍历
    template <typename VST> void travIn(VST&);    //子树中序遍历
    template <typename VST> void travPost(VST&);    //子树后序遍历

/* 比较器、判等器 */
    bool operator<(BinNodePosi(T) bn) {
        return data < bn->data;
    }
    bool operator>(BinNodePosi(T) bn) {
        return data > bn->data;
    }
    bool operator==(BinNodePosi(T) bn) {
        return data == bn->data;
    }
    bool operator<=(BinNodePosi(T) bn) {
        return data <= bn->data;
    }
    bool operator>=(BinNodePosi(T) bn) {
        return data >= bn->data;
    }

    template <typename VST> static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S) {
        while(x) {
            visit(x);
            S.push(x->rChild);
            x = x->lChild;
        }
    }

    template <typename VST> void travPre_I2(BinNodePosi(T) x, VST& visit) {
        Stack<BinNodePosi(T)> S;
        while(true) {
            visitAlongLeftBranch(x, visit, S);
            if(S.empty())
                break;
            x = S.pop();
        }
    }

    void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S) {
        while(x) {
            S.push(x);
            x = x->lChild;
        }
    }

    template <typename VST> void travIn_I1(BinNodePosi(T)x, VST visit) {
        Stack<BinNodePosi(T)> S;
        while (true) {
            goAlongLeftBranch(x, S);
            if(S.empty()) break;
            visit(x);
            x = S.pop();
            x = x->rChild;
        }
    }

    void gotoHLVFL(Stack<BinNodePosi(T)>& S) {
        while(BinNodePosi(T) x = S.pop()) {
            if(HasLChild(x)) {
                if(HasRChild(x))
                    S.push(x->rChild);
                S.push(x->lChild);
            } else {
                S.push(x->rChild);
            }
        }
        S.pop();
    }

    template <typename VST> void travPost_I(BinNodePosi(T) x, VST visit) {
        Stack<BinNodePosi(T)> S;
        S.push(x);
        while(!S.empty()) {
            if(S.top() != x->parent)
                gotoHLVFL(S);
            x = S.pop();
            visit(x);
        }
    }
};

template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const & e) {
    return lChild = new BinNode(e, this);
}

template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const & e) {
    return rChild = new BinNode(e, this);
}

template <typename T>
template <typename VST>
void BinNode<T>::travIn(VST& visit) {
    travIn_I1(this, visit);
}

template <typename T> BinNodePosi(T) BinNode<T>::succ() {
    BinNodePosi(T) x = this;
    if(rChild) {
        x = rChild;
        while(HasLChild(x))
            x = x->lChild;
    } else {
        while(IsRChild(*x))
            x = x->parent;
        x = x->parent;
    }
    return x;
}

template <typename T> int BinNode<T>::size() {
    int l = 0, r = 0;
    if(lChild != nullptr)
        l = lChild->size();
    if(rChild != nullptr)
        r = rChild->size();
    return l + r + 1;
}

template<typename T>
template<typename VST>
void BinNode<T>::travLevel(VST & visit) {
    Queue<BinNodePosi(T)> Q;
    Q.enqueue(this);
    while(!Q.empty()) {
        BinNodePosi(T) x = Q.dequeue();
        visit(x);
        if(HasLChild(x))
            Q.enqueue(x->lChild);
        if(HasRChild(x))
            Q.enqueue(x->rChild);
    }
}

template<typename T>
template<typename VST>
void BinNode<T>::travPre(VST &visit) {
    travPre_I2(this, visit);
}

template<typename T>
template<typename VST>
void BinNode<T>::travPost(VST & visit) {
    travPost_I(this, visit);
}

#endif //DSA_CPP_DENG_MYBINNODE_H
