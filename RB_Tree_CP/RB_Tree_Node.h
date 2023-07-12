//
// Created by Shawn Zhao on 2023/7/11.
//

#ifndef RB_TREE_CP_RB_TREE_NODE_H
#define RB_TREE_CP_RB_TREE_NODE_H
typedef enum
{
    RED = 0,
    BLACK
} Color;
template<class Type>
class RBTNode
{
public:
    Color color;     //颜色
    Type key;        //关键字
    RBTNode *left;   //左孩子
    RBTNode *right;  //右孩子
    RBTNode *parent; //父结点
};
#endif //RB_TREE_CP_RB_TREE_NODE_H
