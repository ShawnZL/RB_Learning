//
// Created by Shawn Zhao on 2023/7/11.
//

#include <iostream>
#include "RB_Tree.h"
enum color {black, red}; // black = 0, red = 1

template <class T>
RB_Tree<T>::RB_Tree(T Root_Data): Root_Node(NULL) {
    if (Root_Node == NULL) {
        Root_Node = new RB_Tree_Node<T>(Root_Data);
        Root_Node -> color_tag = black; // 建立root，并且设置为black
    }
}

template <class T>
RB_Tree<T>::~RB_Tree() {}

/************************************************************************/
/* 函数功能：向红黑树中插入一个节点  这个节点设置为红色1，因为插入节点如果设置为黑色，
 * 将会导致到叶子节点路径上黑色节点的个数有变化，从而不满足条件5  */
// 入口参数：插入的数据
// 返回值：无
/************************************************************************/
template <class T>
void RB_Tree<T>::Insert_Node(T insert_data) {
    RB_Tree_Node<T>* temp_Node = Root_Node;
    while (temp_Node != NULL) {
        if (insert_data > temp_Node -> data) {
            // 大于要向右边出发
            if (temp_Node -> Right_child == NULL) {
                // 创建新的节点
                temp_Node -> Right_child = new RB_Tree_Node<T>(insert_data);
                temp_Node -> Right_child -> color_tag = red; // 设置为红色
                temp_Node -> Right_child -> Father_Node = temp_Node;
                if (temp_Node -> color_tag == red) { //当前节点为红色
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
                temp_Node -> Left_child -> color_tag = red;
                temp_Node -> Left_child -> Father_Node = temp_Node;
                if (temp_Node -> color_tag == red) {
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

/************************************************************************/
/* 函数功能：从红黑树中搜寻要删除的数据节点              */
// 入口参数：删除的数据
//
/************************************************************************/
template<class T>
void RB_Tree<T>::Delete_Node(RB_Tree_Node<T>* current_Node) {
    RB_Tree_Node<T>* x = NULL;
    RB_Tree_Node<T>* y = current_Node;
    int color_tag = current_Node -> color_tag;

    if (current_Node->Left_child == nullptr && current_Node->Right_child != nullptr) {
        // 只有右孩子
        x = current_Node -> Right_child;
        Transplant(current_Node, current_Node -> Right_child);
    }
    else if (current_Node->Left_child != nullptr && current_Node->Right_child == nullptr) {
        // 左子树不为空，右子树为空
        x = current_Node -> Left_child;
        Transplant(current_Node, current_Node -> Left_child);
    }
    else if (current_Node->Left_child != nullptr && current_Node->Right_child != nullptr) {
        // 左右子树都不为空
        y = Find_Successor_Node(current_Node);
        color_tag = y -> color_tag;
        x = y->Right_child;
        if (y -> Father_Node == current_Node) {
            x -> Father_Node = y;
            Transplant(current_Node, y);
            y -> Left_child = current_Node -> Left_child;
            y -> Left_child -> Father_Node = y;
            y -> color_tag = current_Node -> color_tag;
        }
        else {
            Transplant(current_Node, y);
            y -> Left_child = current_Node -> Left_child;
            y -> Left_child -> Father_Node = y;
            y -> color_tag = current_Node -> color_tag;
            y -> Right_child = current_Node -> Right_child;
            y -> Right_child -> Father_Node = y;
            Transplant(y, y -> Right_child);
        }
    }

    if (color_tag == black) {
        // 删除节点为黑色，需要调整红黑树
        Fix_Tree_Delete(x);
    }
    delete current_Node;
}

/************************************************************************/
/* 函数功能：插入节点后修整红黑树，保证满足性质                         */
// 入口参数：插入的当前节点，当前节点为red，违反性质4，所以需要进行调整
// 返回值：无
/************************************************************************/
template <class T>
void RB_Tree<T>::Fix_Tree(RB_Tree_Node<T> *current_Node) {
    RB_Tree_Node<T>* temp_current_Node = current_Node;
    RB_Tree_Node<T>* uncle_Node;

    while (true) {
        // 情况 1 为root
        if (temp_current_Node -> Father_Node == NULL) {
            break;
        }
        // 情况 2 为父亲为black
        if (temp_current_Node -> Father_Node -> color_tag != red) {
            // 父亲节点为black，
            break;
        }

        RB_Tree_Node<T>* father_Node = temp_current_Node -> Father_Node;
        RB_Tree_Node<T>* grandfa_Node = father_Node -> Father_Node;
        if (grandfa_Node) {
            if (father_Node == grandfa_Node -> Left_child) {
                uncle_Node = grandfa_Node -> Right_child;
                // 有叔叔节点
                if (uncle_Node) {
                    //情况3 叔叔为红色  将父亲节点和叔叔节点设置为黑色
                    //祖父节点设置为红色 将祖父节点设置为当前节点
                    if (uncle_Node -> color_tag == red) {
                        uncle_Node -> color_tag = black;
                        father_Node -> color_tag = black;
                        grandfa_Node -> color_tag = red;
                        temp_current_Node = grandfa_Node; // 递归处理
                    }
                        // 情况4 叔叔是黑色 且当前节点为右孩子 将父节点当做当前节点 对父亲节点进行左旋
                    else if (temp_current_Node == father_Node -> Right_child) {
                        temp_current_Node = temp_current_Node -> Father_Node;
                        Left_Rotate(temp_current_Node);
                    }
                        // 情况5 叔叔是黑色 且当前节点为左孩子 将父节点设为黑色 祖父节点设为红色 对祖父节点右旋
                    else {
                        father_Node -> color_tag = black;
                        grandfa_Node -> color_tag = red;
                        Right_Rotate(grandfa_Node);
                    }
                }
                    // 没有叔叔节点
                else {
                    if (temp_current_Node == father_Node -> Right_child) {
                        temp_current_Node = temp_current_Node -> Father_Node;
                        Left_Rotate(temp_current_Node); // 递归，调整为情况3
                    }
                    else {
                        father_Node -> color_tag = black;
                        grandfa_Node -> color_tag = red;
                        Right_Rotate(grandfa_Node);
                    }
                }
            }
            else {
                uncle_Node = grandfa_Node -> Left_child;
                if (uncle_Node) {
                    //情况3 叔叔为红色  将父亲节点和叔叔节点设置为黑色
                    //祖父节点设置为红色 将祖父节点设置为当前节点
                    if (uncle_Node -> color_tag == red) {
                        uncle_Node -> color_tag = black;
                        father_Node ->  color_tag = black;
                        grandfa_Node -> color_tag = red;
                        temp_current_Node = grandfa_Node;
                    }
                        //情况4：叔叔是黑色 且当前节点为左孩子 将父节点作为当前节点 对父节点进行左旋
                    else if (temp_current_Node == father_Node -> Left_child) {
                        temp_current_Node = temp_current_Node -> Father_Node;
                        Right_Rotate(temp_current_Node);
                    }
                        //情况5：叔叔是黑色 且当前节点为左孩子 将父节点设为黑色 祖父节点设为红色 对祖父节点右旋
                    else {
                        father_Node -> color_tag = black;
                        grandfa_Node -> color_tag = red;
                        Left_Rotate(grandfa_Node);
                    }
                }
                else {
                    // 没有uncle
                    if (temp_current_Node == father_Node -> Left_child) {
                        temp_current_Node = temp_current_Node -> Father_Node;
                        Right_Rotate(temp_current_Node); // 递归，调整为情况3
                    }
                    else {
                        father_Node -> color_tag = black;
                        grandfa_Node -> color_tag = red;
                        Left_Rotate(grandfa_Node);
                    }
                }
            }
        }
    }
    Root_Node -> color_tag = black;
}

/************************************************************************/
/* 函数功能：删除节点后修整红黑树，保证满足性质                         */
// 入口参数：删除当前节点，当前节点为black
// 返回值：无
/************************************************************************/
template<class T>
void RB_Tree<T>::Fix_Tree_Delete(RB_Tree_Node<T>* current_Node) {
    RB_Tree_Node<T>* sibling = nullptr;
    while (current_Node != Root_Node && current_Node -> color_tag == black) {
        // 左孩子情况
        if (current_Node == current_Node -> Father_Node -> Left_child) {
            sibling = current_Node -> Father_Node -> Right_child;
            if (sibling -> color_tag == red) {
                sibling -> color_tag = black;
                current_Node -> Father_Node -> color_tag = red;
                Left_Rotate(current_Node -> Father_Node);
                sibling = current_Node -> Father_Node-> Right_child;
            }
            // case 2
            if (sibling -> Left_child -> color_tag == black && sibling -> Right_child -> color_tag == black) {
                sibling -> color_tag = red;
                current_Node = current_Node -> Father_Node;
            }
            else { // 情况3，4
                if (sibling -> Right_child -> color_tag == black) {
                    // 染色成为右旋转
                    sibling -> Left_child -> color_tag = black;
                    sibling -> color_tag = red;
                    Right_Rotate(sibling);
                    sibling = current_Node -> Father_Node -> Right_child;
                }
                // case 4 可以直接结束递归
                sibling -> color_tag = sibling -> Father_Node -> color_tag;
                sibling -> Father_Node -> color_tag = black;
                sibling -> Right_child -> color_tag = black;
                Left_Rotate(current_Node -> Father_Node);
                break;
            }
        }
        else {
            // 处理x是父亲节点的右儿子情况
            sibling = current_Node -> Father_Node -> Left_child;
            if (sibling->color_tag == red) {
                sibling -> Father_Node -> color_tag = red;
                sibling->color_tag = black;
                Right_Rotate(current_Node -> Father_Node);
                sibling = current_Node -> Father_Node ->Left_child;
            }
            else if (sibling->Right_child->color_tag == black && sibling->Left_child->color_tag == black) {
                sibling->color_tag = red;
                current_Node = current_Node -> Father_Node;
            }
            else {
                if (sibling->Left_child->color_tag == black) {
                    sibling->Right_child->color_tag = black;
                    sibling->color_tag = red;
                    Left_Rotate(sibling);
                    sibling = current_Node -> Father_Node -> Left_child;
                }
                sibling->color_tag = current_Node -> Father_Node ->color_tag;
                current_Node -> Father_Node ->color_tag = black;
                sibling->Left_child->color_tag = black;
                Right_Rotate(current_Node -> Father_Node);
                break;
            }
        }
    }
    current_Node -> color_tag = black;
}

/************************************************************************/
/* 函数功能：使用replace_Node替换当前节点                                     */
// 入口参数：当前节点
// 返回值：无
/************************************************************************/
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

/************************************************************************/
/* 函数功能：对当前节点进行左旋操作                                     */
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

/************************************************************************/
/* 函数功能：找寻当前节点的中序后继节点                                 */
// 入口参数：当前节点
// 返回值：当前节点的中序后继节点
/************************************************************************/
template<class T>
RB_Tree_Node<T>* RB_Tree<T>::Find_Successor_Node(RB_Tree_Node<T>* current_Node) {
    if (current_Node == nullptr) {
        return nullptr;
    }

    if (current_Node->Right_child != nullptr) {
        // 如果当前节点有右子树，则后继节点为右子树的最左子节点
        current_Node = current_Node->Right_child;
        while (current_Node->Left_child != nullptr) {
            current_Node = current_Node->Left_child;
        }
        return current_Node;
    }
    else {
        // 如果当前节点没有右子树，则向上查找第一个父节点是左子节点的节点
        RB_Tree_Node<T>* parent = current_Node->Father_Node;
        while (parent != nullptr && current_Node == parent->Right_child) {
            current_Node = parent;
            parent = parent->Father_Node;
        }
        return parent;
    }
}

/************************************************************************/
/* 函数功能：清除该节点相关的所有信息                                   */
// 入口参数：当前节点
// 返回值：无
/************************************************************************/
template<class T>
void RB_Tree<T>::erase_Node(RB_Tree_Node<T> *Node_del) {
    if (Node_del -> Father_Node) {
        Node_del -> Father_Node = NULL;
    }
//    Node_del -> color_tag = NULL;
    Node_del -> Father_Node = NULL;
    Node_del -> Left_child = NULL;
    Node_del -> Right_child = NULL;
//    Node_del -> data = NULL;
    delete Node_del;
}

/************************************************************************/
/* 函数功能：打印红黑树                                   */
// 入口参数：当前节点
// 返回值：无
/************************************************************************/
template <class T>
void PrintTree(RB_Tree_Node<T>* root, int level = 0) {
    if (root == nullptr) {
        return;
    }
    // 递归打印右子树
    PrintTree(root->Right_child, level + 1);

    // 打印节点信息
    for (int i = 0; i < level; i++) {
        std::cout << "   ";
    }
    std::cout << "|--" << root->data << (root->color_tag == black ? " (B)" : " (R)") << std::endl;

    // 递归打印左子树
    PrintTree(root->Left_child, level + 1);
}


/************************************************************************/
/* 函数功能：根据值搜索节点                                   */
// 入口参数：当前节点
// 返回值：返回找到节点
/************************************************************************/
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