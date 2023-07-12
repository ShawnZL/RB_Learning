//
// Created by Shawn Zhao on 2023/7/12.
//

#ifndef RB_TREE_CP_RB_TREE_H
#define RB_TREE_CP_RB_TREE_H
#include "RB_Tree_Node.h"
#include <iostream>

//红黑树类型
template <class Type>
class RBTree
{
public:
    RBTree();
    ~RBTree();
    void InOrder();
    bool Insert(const Type &value);
    void Remove(Type key);  //删除key结点(先查找，再调用内部删除)
    void InOrderPrint(); //中序遍历打印结点详细的结点颜色

protected:
    RBTNode<Type>* BuyNode(const Type &x = Type()); //申请结点结点，将结点的颜色初始化为红色，初始化结点的关键字，其他的初始化为空
    void InOrder(RBTNode<Type> *root); //中序遍历
    void LeftRotate(RBTNode<Type> *z);
    void RightRotate(RBTNode<Type> *z);
    //插入后的调整函数
    void Insert_Fixup(RBTNode<Type> *s);
    //查找key结点
    RBTNode<Type> *Search(RBTNode<Type> *root, Type key) const;
    void Transplant(RBTNode<Type> *u, RBTNode<Type> *v);
    RBTNode<Type> *Minimum(RBTNode<Type> *x);
    //删除红黑树结点z
    void Remove(RBTNode<Type> *z);
    //红黑树删除调整
    void Remove_Fixup(RBTNode<Type> *x);
    //销毁红黑树
    void destroy(RBTNode<Type> *&root);
    //中序遍历打印结点详细的结点颜色
    void InOrderPrint(RBTNode<Type> *node);
private:
    RBTNode<Type> *root; //根指针
    RBTNode<Type> *Nil;  //外部结点，表示空结点，黑色的
};

#endif //RB_TREE_CP_RB_TREE_H
