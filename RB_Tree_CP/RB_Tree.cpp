//
// Created by Shawn Zhao on 2023/7/11.
//

//
// Created by Shawn Zhao on 2023/7/11.
//

#include <iostream>
#include "RB_Tree.h"
enum color {BLACK, RED}; // BLACK = 0, RED = 1


template <class T>
RB_Tree<T>::RB_Tree(T Root_Data): Root_Node(NULL) {
    if (Root_Node == NULL) {
        Root_Node = new RB_Tree_Node<T>(Root_Data);
        Root_Node -> color_tag = BLACK; // 建立root，并且设置为black
    }
}

template <class T>
RB_Tree<T>::~RB_Tree() {}


/************************************************************************/
/* 函数功能：对当前节点进行左旋操作，对具有任意具有右孩子的结点可以进行             */
// 入口参数：左旋的当前节点
// 返回值：无
/************************************************************************/
template <class T>
void RB_Tree<T>::Left_Rotate(RB_Tree_Node<T> *current_Node) {
    RB_Tree_Node<T>* Right_child = current_Node -> Right_child;
    current_Node -> Right_child = Right_child -> Left_child;
    if (Right_child -> Left_child != NULL) {
        Right_child -> Left_child -> Father_Node = current_Node;
    }
    Right_child -> Father_Node = current_Node -> Father_Node;
    if (current_Node -> Father_Node == NULL) {
        Root_Node = Right_child;
    }
    else if (current_Node == current_Node -> Father_Node -> Left_child) {
        current_Node -> Father_Node -> Left_child = Right_child;
    }
    else
        current_Node -> Father_Node -> Right_child = Right_child;
    Right_child -> Left_child = current_Node;
    current_Node -> Father_Node = Right_child;
}

/************************************************************************/
/* 函数功能：对当前节点进行右旋操作                                     */
// 入口参数：右旋的当前节点
// 返回值：无
/************************************************************************/
template <class T>
void RB_Tree<T>::Right_Rotate(RB_Tree_Node<T> *current_Node) {
    RB_Tree_Node<T>* left_child = current_Node -> Left_child;
    current_Node -> Left_child = left_child -> Right_child;
    if (left_child -> Right_child != NULL) {
        left_child -> Right_child -> Father_Node = current_Node;
    }
    left_child -> Father_Node = current_Node -> Father_Node;
    if (current_Node -> Father_Node == NULL) {
        Root_Node = left_child;
    }
    else if (current_Node == current_Node -> Father_Node -> Left_child) {
        current_Node -> Father_Node -> Left_child = left_child;
    }
    else {
        current_Node -> Father_Node -> Right_child = left_child;
    }
    left_child -> Right_child = current_Node;
    current_Node -> Father_Node = left_child;
}

///红黑树插入调整函数
///我们将插入结点染成红色,可能违反了性质4,所以要进行调整
///调整的过程其实就是根据不同的情况进行分类讨论,不断转换的过程
///最后转成可以通过染色和旋转恢复性质的情况
template<class T>
void RB_Tree<T>::Fix_Tree(RB_Tree_Node<T> *current_Node)
{
    ///在下面的代码中current结点总是违反性质4的那个结点
    while(current_Node -> Father_Node -> color_tag==RED) ///x是红色,它的父结点也是红色就说明性质4被违反,要持续调整
    {
        ///下面的过程按x->p是其祖父结点的左孩子还是右儿子进行分类讨论
        if(current_Node -> Father_Node == current_Node -> Father_Node -> Father_Node -> Left_child) ///父结点是其祖父结点的左孩子
        {
            RB_Tree_Node<T>* uncle = current_Node -> Father_Node -> Father_Node -> Right_child;  ///表示z的叔结点
            ///下面按y的颜色进行分类讨论
            if(uncle -> color_tag == RED)
            {///如果uncle是红色并current的祖父结点一定是黑色的,这时我们通过下面的染色过程
                ///在保证黑高不变的情况下(性质5),将z在树中上移两层,current=current->Father->Father
                current_Node -> Father_Node -> color_tag = BLACK;
                uncle -> color_tag = BLACK;
                current_Node -> Father_Node -> Father_Node -> color_tag = RED;
                current_Node = current_Node -> Father_Node -> Father_Node;///如果上移到根节点或某个父结点不为红的结点就可以结束循环了
            }
            else   ///叔结点为黑色
            { ///此时要根据z是其父结点的左孩子还是右孩子进行分类讨论
                ///如果z是左孩子则可以直接可以通过染色和右旋来恢复性质
                ///如果z是右孩子则可以先左旋来转成右孩子的情况
                if(current_Node == current_Node -> Father_Node -> Right_child)
                {
                    current_Node = current_Node -> Father_Node;
                    Left_Rotate(current_Node); ///直接左旋
                }
                ///重新染色,再右旋就可以恢复性质
                current_Node -> Father_Node -> color_tag = BLACK;
                current_Node -> Father_Node -> Father_Node -> color_tag=RED;
                Right_Rotate(current_Node -> Father_Node -> Father_Node);
            }
        }
        else///父结点是祖父结点的右孩子
        {
            RB_Tree_Node<T>*  uncle = current_Node -> Father_Node -> Father_Node -> Left_child;  ///叔结点
            if(uncle -> color_tag == RED)
            {
                current_Node -> Father_Node -> color_tag = BLACK;
                uncle -> color_tag = BLACK;
                current_Node -> Father_Node -> Father_Node -> color_tag = RED;
                current_Node = current_Node -> Father_Node -> Father_Node;
            }
            else
            {///右儿子的时候可以直接左旋,重新调色恢复性质
                ///左儿子可以先右旋成右儿子再处理
                if(current_Node == current_Node -> Father_Node -> Left_child)
                {
                    current_Node = current_Node -> Father_Node;
                    Right_Rotate(current_Node);
                }
                current_Node -> Father_Node -> color_tag = BLACK;
                current_Node -> Father_Node -> Father_Node -> color_tag = RED;
                Left_Rotate(current_Node -> Father_Node -> Father_Node);
            }
        }
    }
    ///将根节点染成黑色，是必要的步骤；处理两种情况
    ///1.第一次插入根结点被染成红色的情况
    ///2.和在上面的循环中根节点可能被染成红色的情况
    Root_Node -> color_tag = BLACK;
}

///红黑树的插入
///RB插入函数与普通的BST的插入函数只是稍微有点不同
///我们将原来的null换成了Nul结点,然后对新加入的结点,染成红色
///然后调用RBInserootFixUp函数进行调整,使得红黑树的性质不被破坏
template<class T>
void RB_Tree<T>::Insert_Node(T insert_data) {
    RB_Tree_Node<T>* temp_Node = Root_Node;
    while (temp_Node != NULL) {
        if (insert_data > temp_Node -> data) {
            // 大于要向右边出发
            if (temp_Node -> Right_child == NULL) {
                // 创建新的节点
                temp_Node -> Right_child = new RB_Tree_Node<T>(insert_data);
                temp_Node -> Right_child -> color_tag = RED; // 设置为红色
                temp_Node -> Right_child -> Father_Node = temp_Node;
                if (temp_Node -> color_tag == RED) { //当前节点为红色
                    Fix_Tree(temp_Node -> Right_child); // 调整插入节点
                }
                break;
            }
            else {
                temp_Node = temp_Node -> Right_child;
            }
        }
        else {
            if (temp_Node -> Left_child == NULL) {
                temp_Node -> Left_child = new RB_Tree_Node<T>(insert_data);
                temp_Node -> Left_child -> color_tag = RED;
                temp_Node -> Left_child -> Father_Node = temp_Node;
                if (temp_Node -> color_tag == RED) {
                    Fix_Tree(temp_Node -> Left_child); // 调整插入节点
                }
                break;
            }
            else {
                temp_Node = temp_Node -> Left_child;
            }
        }
    }
}

///红黑树替换函数,replace替换current,与BST的类似
///只负责更改父结点的指向,左右儿子需要自己更改
template<class T>
void RB_Tree<T>::Transplant(RB_Tree_Node<T> *current_Node, RB_Tree_Node<T> *replace_Node) {
    if (current_Node -> Father_Node == NULL) {
        // 当前节点为root
        Root_Node = replace_Node;
    }
    else if (current_Node == current_Node -> Father_Node -> Left_child) {
        current_Node -> Father_Node -> Left_child = replace_Node;
    }
    else {
        current_Node -> Father_Node -> Right_child = replace_Node;
    }
    replace_Node -> Father_Node = current_Node -> Father_Node;
}

///红黑树后继查找函数
///按二叉搜索树的性质一直往左走
template<class T>
RB_Tree_Node<T>* RB_Tree<T>::Find_Successor_Node(RB_Tree_Node<T>* current_Node) {
    if (current_Node -> Left_child == NULL)
        return current_Node;
    return Find_Successor_Node(current_Node -> Left_child);
}

///红黑树删除调整函数
///这个函数主要要解决的问题是current被染成红黑色,或是双重黑色的问题
///对于第一种情况只要简单的去掉current的红色就行了。
///对于第二种情况我们分情况讨论，将双重黑色的结点在树中上升
///直到转成情况1,或是上升为根结点
template<class T>
void RB_Tree<T>::Fix_Tree_Delete(RB_Tree_Node<T> *current_Node) {
    while(current_Node != Root_Node && current_Node -> color_tag == BLACK)
    {
        if(current_Node == current_Node -> Father_Node -> Left_child)///按current是其父结点的左/右孩子分情况讨论
        {///下面的过程要按其兄弟结点的颜色进行分类讨论
            RB_Tree_Node<T>* uncle = current_Node -> Father_Node -> Right_child; ///其兄弟结点
            ///Case 1
            if(uncle -> color_tag == RED)///如果兄弟结点是红色
            {///此时父结点一定是黑色；在保证黑高的情况下
                ///我们通过染色和旋转转成下面兄弟结点为黑色的情况
                uncle -> color_tag = BLACK;
                current_Node -> Father_Node -> color_tag = RED;
                Left_Rotate(current_Node -> Father_Node);
                uncle = current_Node -> Father_Node -> Right_child;
            }
            ///Case 2
            if(uncle -> Left_child -> color_tag == BLACK && uncle -> Right_child -> color_tag == BLACK)
            {///通过染色将current上移一层
                uncle -> color_tag = RED;
                current_Node = current_Node -> Father_Node; ///将x在树中上移一层,如果x->p是根结点或x->p原来是红色则结束循环,否则转成情况1
            }
            else ///情况3,4
            {
                ///Case 3
                if(uncle -> Right_child -> color_tag == BLACK)
                {///染色和右旋成情况4
                    uncle -> color_tag = RED;
                    uncle -> Left_child -> color_tag = BLACK;
                    Right_Rotate(uncle);
                    uncle = current_Node -> Father_Node -> Right_child;
                }
                ///Case 4
                ///情况4可以直接结束递归
                uncle -> color_tag = uncle -> Father_Node -> color_tag;
                uncle -> Father_Node -> color_tag = BLACK;
                uncle -> Right_child -> color_tag = BLACK; ///需要将w的右儿子染成黑色以保证黑高
                Left_Rotate(current_Node -> Father_Node);
                break;
            }
        }
        else ///处理x是父结点的右儿子的情况
        {
            RB_Tree_Node<T>* uncle = current_Node -> Father_Node -> Left_child;
            if(uncle -> color_tag == RED)
            {
                uncle -> Father_Node -> color_tag = RED;
                uncle -> color_tag = BLACK;
                Right_Rotate(current_Node -> Father_Node);
                uncle = current_Node -> Father_Node -> Left_child;
            }
            else if(uncle -> Left_child -> color_tag == BLACK && uncle -> Right_child -> color_tag == BLACK)
            {
                uncle -> color_tag = RED;
                current_Node = current_Node -> Father_Node;
            }
            else
            {
                if(uncle -> Left_child -> color_tag == BLACK)
                {
                    uncle -> Right_child -> color_tag = BLACK;
                    uncle -> color_tag = RED;
                    Left_Rotate(uncle);
                    uncle = current_Node -> Father_Node -> Left_child;
                }
                uncle -> color_tag = current_Node -> Father_Node -> color_tag;
                current_Node -> Father_Node ->color_tag = BLACK;
                uncle -> Left_child -> color_tag = BLACK;
                Right_Rotate(current_Node -> Father_Node);
                break;
            }
        }
    }
    current_Node-> color_tag = BLACK;
}

//红黑树删除函数
///类似于二叉树删除函数,不过在删除完成以后需要调用调整函数恢复性质
///总的过程也是按z的左右儿子情况进行分类.
///1.z只有左儿子/右儿子
///2.z有左右儿子,z的后继结点是z的右儿子
///3.z有左右儿子,z的后继结点不是z的右儿子
template <class T>
void RB_Tree<T>::Delete_Node(RB_Tree_Node<T> *current_Node) {
    ///在下面的过程中y总是指向树中会被删除的结点或是会被替代的结点
    ///x总是指向要替代z或y的结点

    RB_Tree_Node<T>* x = NULL;
    RB_Tree_Node<T>* y = current_Node ;
    int ycolor=y -> color_tag; ///记录y原来的颜色
    if(current_Node -> Left_child == NULL) ///只有右儿子
    {
        x = current_Node -> Right_child;
        Transplant(current_Node, current_Node -> Right_child);// current->right 替换 current
    }
    else if(current_Node -> Right_child == NULL) ///只有左儿子
    {
        x = current_Node -> Left_child;
        Transplant(current_Node, current_Node -> Left_child);
    }
    else  ///左右儿子都有
    {
        y = Find_Successor_Node(current_Node -> Right_child); ///查找z的后继
        ycolor = y -> color_tag;
        x = y -> Right_child; ///因为后面y会被染成z原来的颜色,所以违反性质的就是y的右儿子
        if(y -> Father_Node == current_Node) ///y是z的孩子结点
        {
            x -> Father_Node = y;///这种情况下,y为x的父结点

            Transplant(current_Node,y); ///y取代current
            y -> Left_child = current_Node -> Left_child; ///current的左孩子改变指向
            y -> Left_child -> Father_Node = y;
            y -> color_tag = current_Node -> color_tag; ///更改y的颜色,这样的话从y以上红黑树的性质都不会违反
        }
        else ///y不是z的孩子结点的情况
        {
            Transplant(current_Node, y); ///y取代current
            y -> Left_child = current_Node -> Left_child; ///z的左孩子改变指向
            y -> Left_child -> Father_Node = y;
            y -> color_tag = current_Node -> color_tag; ///更改y的颜色,这样的话从y以上红黑树的性质都不会违反
            y -> Right_child = current_Node -> Right_child;
            y -> Right_child -> Father_Node = y;
            Transplant(y, y -> Right_child);
        }
    }
    //delete z;//
    ///如果y原来的颜色是黑色,那么就意味着有一个黑色结点被覆盖了,
    ///红黑树的性质可能会被破坏(性质4或5),需要调整
    if(ycolor==BLACK)
    {
        Fix_Tree_Delete(x);
    }
}


///通过关键字搜索对应结点
template<class T>
RB_Tree_Node<T>* RB_Tree<T>::SearchByKey(RB_Tree_Node<T> *current_Node, T search_data) {
    while (current_Node != NULL) {
        if (current_Node->data == search_data)
            return current_Node;
        else if (current_Node->data > search_data) {
            current_Node = current_Node -> Left_child;
        }
        else if (current_Node->data < search_data) {
            current_Node = current_Node -> Right_child;
        }
    }
    return NULL;
}

/************************************************************************/
/* 函数功能：打印红黑树                                   */
// 入口参数：当前节点
// 返回值：无
/************************************************************************/
template <class T>
void RB_Tree<T>::PrintTree(RB_Tree_Node<T>* root, int level) {
    if (root == nullptr) {
        return;
    }
    // 递归打印右子树
    PrintTree(root->Right_child, level + 1);

    // 打印节点信息
    for (int i = 0; i < level; i++) {
        std::cout << "   ";
    }
    std::cout << "|--" << root->data << (root->color_tag == BLACK ? " (B)" : " (R)") << std::endl;

    // 递归打印左子树
    PrintTree(root->Left_child, level + 1);
}
