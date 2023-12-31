//
// Created by Shawn Zhao on 2023/7/12.
//

#include "RB_Tree.h"
#include <iostream>

// 构造函数
template<class Type>
RBTree<Type>::RBTree() {
    Nil = BuyNode();
    root = Nil;
    Nil->color = BLACK;
}

template<class Type>
RBTree<Type>::~RBTree<Type>() {
    destroy(root); //销毁创建的非Nil结点
    delete Nil;    //最后删除Nil结点
    Nil = NULL;
}

// 中序遍历
template<class Type>
void RBTree<Type>::InOrder() {
    InOrder(root);
}

//插入
//1.BST方式插入
//2.调整平衡
template<class Type>
bool RBTree<Type>::Insert(const Type &value) {
    RBTNode<Type> *pr = Nil; //pr用来记住父节点
    RBTNode<Type> *s = root; //定义变量s指向根
    while (s != Nil)
    {
        if (value == s->key)
        {
            return false;
        }
        pr = s; //每次记住s的父节点
        if (value < s->key)
        {
            s = s->left;
        }
        else
        {
            s = s->right;
        }
    }
    //循环后s==Nil
    s = BuyNode(value); //申请结点
    if (pr == Nil)      //如果父节点pr是根节点，第一次root指向Nil，所以pr==Nil
    {
        root = s;
        root->parent = pr;
    }
    else //如果父节点不是根节点
    {
        if (value < pr->key)
        {
            pr->left = s;
        }
        else
        {
            pr->right = s;
        }
        s->parent = pr; //设置新结点s的父节点
    }
    //调整平衡
    Insert_Fixup(s);
    return true;
}

//删除key结点(先查找，再调用内部删除)
template<class Type>
void RBTree<Type>::Remove(Type key) {
    RBTNode<Type> *t;
    if ((t = Search(root, key)) != Nil)
    {
        Remove(t);
    }
    else
    {
        std::cout << "Key is not exist." << std::endl;
    }
}

//中序遍历打印结点详细的结点颜色
template<class Type>
void RBTree<Type>::InOrderPrint() {
    InOrderPrint(root);
}

//申请结点结点，将结点的颜色初始化为红色，初始化结点的关键字，其他的初始化为空
template<class Type>
RBTNode<Type>* RBTree<Type>::BuyNode(const Type &x) {
    RBTNode<Type> *s = new RBTNode<Type>();
    assert(s != NULL);
    s->color = RED;
    s->left = s->right = s->parent = Nil;
    s->key = x;
    return s;
}

//中序遍历
template<class Type>
void RBTree<Type>::InOrder(RBTNode<Type> *root) {
    if (root != Nil)
    {
        InOrder(root->left);
        std::cout << root->key << " ";
        InOrder(root->right);
    }
}

/* 左转，对z结点左转
     *       zp                 zp
     *       /                  /
     *     z                   y
     *    / \      ===>       / \
     *   lz  y               z   ry
     *      / \             / \
     *     ly  ry          lz  ly
     */
template<class Type>
void RBTree<Type>::LeftRotate(RBTNode<Type> *z) {
    RBTNode<Type> *y = z->right; //用y指向要转动的z结点
    z->right = y->left;
    if (y->left != Nil) //y所指结点的左结点不为空
    {
        y->left->parent = z;
    }
    y->parent = z->parent;
    if (root == z) //z就是根节点
    {
        root = y;
    }
    else if (z == z->parent->left) //z在左结点
    {
        z->parent->left = y;
    }
    else //z在右结点
    {
        z->parent->right = y;
    }
    y->left = z;
    z->parent = y;
}

/* 右转，对z结点进行右转
    *         zp               zp
    *        /                 /
    *       z                 y
    *      / \    ===>       / \
    *     y   rz           ly   z
    *    / \                   / \
    *   ly  ry                ry  rz
    */
template<class Type>
void RBTree<Type>::RightRotate(RBTNode<Type> *z) {
    RBTNode<Type>* y = z -> left;
    z -> left = y -> right;
    if (y -> right != Nil) {
        y -> right -> parent = z;
    }
    y -> parent = z -> parent;
    if (root == z) {
        root = y;
    }
    else if (z == z -> parent -> left) { // 左节点
        z -> parent -> left = y;
    }
    else { // 右节点
        z -> parent -> right = y;
    }
    y -> right = z;
    z -> parent = y;
}

//插入后的调整函数
template<class Type>
void RBTree<Type>::Insert_Fixup(RBTNode<Type> *s) {
    RBTNode<Type> *uncle;           //叔结点（父结点的兄弟结点）
    while (s->parent->color == RED) //父节点的颜色也为红色
    {
        if (s->parent == s->parent->parent->left) //父节点是左结点
        {
            uncle = s->parent->parent->right;

            if (uncle->color == RED) //叔结点为红色
            {
                //父节点和叔结点都变为黑色
                s->parent->color = BLACK;
                uncle->color = BLACK;
                //祖父结点变为红色
                s->parent->parent->color = RED;
                //将s指针指向祖父结点，下一次循环继续判断祖父的父节点是否为红色
                s = s->parent->parent;
            }
            else //没有叔结点，或叔结点为黑色(经过多次循环转换，叔结点可能为黑)
            {
                if (s == s->parent->right) //如果调整的结点在右结点
                {
                    s = s->parent; //先将s指向s的父结点
                    LeftRotate(s); //再左转
                }
                //如果调整的结点在左结点,将s的父节点变为黑色，将祖父的结点变为红色，将s的祖父结点右转
                s->parent->color = BLACK;
                s->parent->parent->color = RED;
                RightRotate(s->parent->parent);
            }
        }
        else {
            uncle = s->parent->parent->left;
            if (uncle->color == RED) {//叔结点为红色
                //父节点和叔结点都变为黑色
                s->parent->color = BLACK;
                uncle->color = BLACK;
                //祖父结点变为红色
                s->parent->parent->color = RED;
                //将s指针指向祖父结点，下一次循环继续判断祖父的父节点是否为红色
                s = s->parent->parent;
            }
            else //没有叔结点，或叔结点为黑色(经过多次循环转换，叔结点可能为黑)
            {
                if (s == s->parent->left) {//如果调整的结点在左结点
                    s = s->parent;  //先将s指向s的父结点
                    RightRotate(s); //再右转
                }
                //如果调整的结点在右结点,将s的父节点变为黑色，将祖父的结点变为红色，将s的祖父结点右转
                s->parent->color = BLACK;
                s->parent->parent->color = RED;
                LeftRotate(s->parent->parent);
            }
        }
    }
    root->color = BLACK; //最后始终将根节点置为黑色
}

//查找key结点
template<class Type>
RBTNode<Type>* RBTree<Type>::Search(RBTNode<Type> *root, Type key) const {
    if (root == Nil) //root为空，或key和根的key相同
    {
        return Nil;
    }

    if (root->key == key)
    {
        return root;
    }
    if (key < root->key)
    {
        return Search(root->left, key);
    }
    else
    {
        return Search(root->right, key);
    }
}

/* 将u的子节点指向u的指针改变指向v，将v的父节点指针改变为指向u的父节点
    *      up
    *        \
    *         u
    *        / \
    *      ul   ur
    *     / \
    *    v  ulr
    *     \
    *     rv
    */
template<class Type>
void RBTree<Type>::Transplant(RBTNode<Type> *u, RBTNode<Type> *v) {
    if (u->parent == Nil) //u的父节点为空
    {
        root = v; //直接令根root为v
    }
    else if (u == u->parent->left) //u父节点不为空，且u在左子树
    {
        u->parent->left = v;
    }
    else //u在右子树
    {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

/* 找到最左结点(最小)
     *      xp
     *        \
     *         x
     *        / \
     *      xl   xr
     *     / \
     *   xll  xlr
     */
template<class Type>
RBTNode<Type>* RBTree<Type>::Minimum(RBTNode<Type> *x) {
    if (x->left == Nil)
    {
        return x;
    }
    return Minimum(x->left);
}

//删除红黑树结点z
template<class Type>
void RBTree<Type>::Remove(RBTNode<Type> *z) {
    RBTNode<Type> *x = Nil;
    RBTNode<Type> *y = z;    //y记住传进来的z结点
    Color ycolor = y->color; //
    if (z->left == Nil)      //z只有右孩子
    {
        x = z->right;
        Transplant(z, z->right);
    }
    else if (z->right == Nil) //z只有右孩子
    {
        x = z->left;
        Transplant(z, z->left);
    } // 经过以上俩操作，z节点成功成为孤家寡人
    else //右左孩子和右孩子
    {
        y = Minimum(z->right); //y是z右子树的的最左子树
        ycolor = y->color;
        x = y->right;
        if (y->parent == z) //z的右子结点没有左节点或为Nil
        {/*
     *      z
     *       \
     *        y
     *         \
     *          x
     */
            x->parent = y;
        }
        else //z的右子结点有左节点或为Nil
        {
     /*
     *      z
     *       \
     *        .
     *       /  \
     *      y    .
     *       \
              x
     */
            Transplant(y, y->right);

            y->right = z->right;
            y->right->parent = y;
     /*
     *      y
     *       \
     *        .
     *       /  \
     *      x    .
     *
             z
            /
           zl
     */
        } // y已经抢占了z的右孩子，尚未替代z的位置
        Transplant(z, y);
        //改变指向
        y->left = z->left;
        z->left->parent = y;
        y->color = z->color;
    }
    if (ycolor == BLACK)
    {
        Remove_Fixup(x); // 从最下开始调整
    }
}

//红黑树删除调整
template<class Type>
void RBTree<Type>::Remove_Fixup(RBTNode<Type> *x) {
    // 首先将最小节点y替代原来删除的z，相当于在这个节点上删除了y，y的右孩子x就代替成为新y
    // 此时x为删除后替代的原来节点。
    // x = N w = S 对应wiki
    while (x != root && x->color == BLACK) //当结点x不为根并且它的颜色是黑色
    {
        if (x == x->parent->left) //x在左子树
        {
            RBTNode<Type> *w = x->parent->right; //w是x的兄结点

            if (w->color == RED) //情况 2
            {
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotate(x->parent);
                w = x->parent->right;
            }
            // 经过情况2，所有路径上黑色节点数目没有改变，但是x(N)相当于有了一个黑色兄弟和红色p
            // 转换为情形 4，5，6，p -> r -> x 转变为p -> x 相当于少了一个黑色节点，新parent和新S(w)不平衡
            if (w->left->color == BLACK && w->right->color == BLACK) //情况 3 和 4
            {
                w->color = RED;
                x = x->parent;
                // 此时w少了一个黑色的节点，可以通过再次与parent平衡，转换为parent少一个黑色，继续调整
            }
            else
            {
                if (w->right->color == BLACK) //情况5 转换为情况6
                {
                    // S(w)是黑色，S(w)的左儿子是红色，S(w)的右儿子是黑色
                    w->color = RED;
                    w->left->color = BLACK;
                    RightRotate(w);
                    w = x->parent->right;
                }
                //情况6 讨论
                w->color = w->parent->color;
                w->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(x->parent);
                x = root; //结束循环
            }
        }
        else //x在右子树
        {
            RBTNode<Type> *w = x->parent->left;
            if (w->color == RED) //情况2
            {
                w->parent->color = RED;
                w->color = BLACK;
                RightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->right->color == BLACK) //情况3 4
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK) //情况5
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(w);
                    w = x->parent->left;
                }
                //情况6
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(x->parent);
                x = root; //结束循环
            }
        }
    }
    x->color = BLACK;
}

// 销毁红黑树
template<class Type>
void RBTree<Type>::destroy(RBTNode<Type> *&root) {
    if (root == Nil)
    {
        return;
    }
    if (root->left != Nil)
    {
        destroy(root->left);
    }
    if (root->right != Nil)
    {
        destroy(root->right);
    }
    delete root;
    root = NULL;
}

//中序遍历打印结点详细的结点颜色
template<class Type>
void RBTree<Type>::InOrderPrint(RBTNode<Type> *node) {
    if (node == Nil)
    {
        return;
    }
    if (node->left != NULL)
    {
        InOrderPrint(node->left);
    }
    std::cout << node->key << "(" << ((node->color == BLACK) ? "BLACK" : "RED") << ")"
              << " ";
    if (node->right != Nil)
    {
        InOrderPrint(node->right);
    }
}
