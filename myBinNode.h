#ifndef DSA_CPP_DENG_MYBINNODE_H
#define DSA_CPP_DENG_MYBINNODE_H

#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p) ? (p)->height : -1)
typedef enum {RB_RED, RB_BLACK} RBColor;

template <typename T> struct BinNode {
    T data;
    BinNodePosi(T) parent;
    BinNodePosi(T) lChild;
    BinNodePosi(T) rChild;
    int height;
    int npl;    //Null Path Length(左式堆，也可直接用height代替)
    RBColor color;

/* 构造函数 */
    BinNode() : parent(nullptr), lChild(nullptr), rChild(nullptr), height(0), npl(1),
            color(RB_RED) {}
    BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr,
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
        return data < bn.data;
    }
    bool operator>(BinNodePosi(T) bn) {
        return data > bn.data;
    }
    bool operator==(BinNodePosi(T) bn) {
        return data == bn.data;
    }
    bool operator<=(BinNodePosi(T) bn) {
        return data <= bn.data;
    }
    bool operator>=(BinNodePosi(T) bn) {
        return data >= bn.data;
    }
};

#endif //DSA_CPP_DENG_MYBINNODE_H
