//
// Created by Shawn Zhao on 2023/7/11.
//

#ifndef RB_RB_TREE_H
#define RB_RB_TREE_H
#include<iostream>
#include<vector>
using namespace std;
enum Color
{
    red,
    black
};
struct RBNode{
    RBNode(int d,Color c):data(d),color(c),lchild(nullptr),rchild(nullptr){}
    ~RBNode(){
        delete lchild;
        delete rchild;
    }
    int data;
    Color color;
    RBNode *lchild;
    RBNode *rchild;
};
class RBTree{
public:
    RBTree(const vector<int> &);
    void insert(int);
    void MidOrderTraversal() const;
    void DeleteNode(int);

private:
    RBNode *head;
    bool obey;//表示被插入节点的父节点是个红色节点即违反不能连续两个红色节点的条件
    bool newInsert;//表示新插入了一个节点，用来判断被插入节点的父节点是红黑从而判断是否违反不能连续两个红色节点的条件
    bool blackH_change;//表示删除过程中黑高改变
    void R_Rotate(RBNode *&);
    void L_Rotate(RBNode *&);
    void insert(RBNode *&, int);
    void LeftLoseToBalance(RBNode *&);
    void RightLoseToBalance(RBNode *&);
    void MidOrderTraversal(RBNode *) const;
    void DeleteNode(RBNode *&, int);
};
RBTree::RBTree(const vector<int> &v):head(nullptr),obey(false),newInsert(false),blackH_change(false)
{
    for (const auto &i : v)
    {
        if (!head)
            head = new RBNode(i, black);
        else
            insert(head, i);
    }
}
void RBTree::R_Rotate(RBNode *&rp)
{
    auto l = rp->lchild;
    Color tem_color = rp->color;
    rp->color = l->color;
    l->color = tem_color;
    rp->lchild = l->rchild;
    l->rchild = rp;
    rp = l;
}
void RBTree::L_Rotate(RBNode *&rp)
{
    auto r = rp->rchild;
    Color tem_color = rp->color;
    rp->color = r->color;
    r->color = tem_color;
    rp->rchild = r->lchild;
    r->lchild = rp;
    rp = r;
}
//左面失去平衡说明总体需要右旋，说明该节点的左子树出现两个连续的红色节点，需要判断其左红色节点的下一个红色节点是其右孩子节点还是左孩子节点
void RBTree::LeftLoseToBalance(RBNode *&rp)
{
    auto l = rp->lchild;
    if (l->lchild && l->lchild->color == red)
        R_Rotate(rp);
    else
    {
        L_Rotate(rp->lchild);
        R_Rotate(rp);
    }
}
//右面失去平衡同理
void RBTree::RightLoseToBalance(RBNode *&rp)
{
    auto r = rp->rchild;
    if (r->rchild && r->rchild->color == red)
        L_Rotate(rp);
    else
    {
        R_Rotate(rp->rchild);
        L_Rotate(rp);
    }
}
void RBTree::insert(RBNode *&rp, int key)
{
    if (rp->data > key)
    {
        if (!rp->lchild)
        {
            rp->lchild = new RBNode(key, red);
            newInsert = true;
        }
        else
            insert(rp->lchild, key);
        if (obey)
        {
            if (rp->rchild && rp->rchild->color == red)
            {
                rp->lchild->color = rp->rchild->color = black;
                if (rp != head)
                {
                    rp->color = red;
                    newInsert = true;
                    obey = false;
                    return;
                }
                else
                    obey = newInsert = false;
            }
            else
            {
                LeftLoseToBalance(rp);
                obey = newInsert = false;
            }
        }
        if (newInsert)
        {
            if (rp->color == red)
                obey = true;
            else
                obey = newInsert = false;
        }
    }
    else
    {
        if (!rp->rchild)
        {
            rp->rchild = new RBNode(key, red);
            newInsert = true;
        }
        else
            insert(rp->rchild, key);
        if (obey)
        {
            if (rp->lchild && rp->lchild->color == red)
            {
                rp->lchild->color = rp->rchild->color = black;
                if (rp != head)
                {
                    rp->color = red;
                    newInsert = true;
                    obey = false;
                    return;//避免该节点重复判断！
                }
                else
                    obey = newInsert = false;
            }
            else
            {
                RightLoseToBalance(rp);
                newInsert = obey = false;
            }
        }
        if (newInsert)
        {
            if (rp->color == red)
                obey = true;
            else
                obey = newInsert = false;
        }
    }
}
void RBTree::insert(int key)
{
    insert(head, key);
}
void RBTree::MidOrderTraversal() const
{
    MidOrderTraversal(head);
    cout << endl;
}
void RBTree::MidOrderTraversal(RBNode *p) const
{
    if(!p)
        return;
    else
    {
        MidOrderTraversal(p->lchild);
        cout<<p->data<<" ";
        MidOrderTraversal(p->rchild);
    }
}
void RBTree::DeleteNode(int key)
{
    DeleteNode(head, key);
}
void RBTree::DeleteNode(RBNode *&rp, int key)//rp==reference to pointer
{
    if (rp->data == key)
    {
        if (!rp->rchild && !rp->lchild)
        {
            if (rp->color == red)//被删除的节点为红色叶节点，直接删除即可，黑高不改变
            {
                auto ready = rp;
                rp = nullptr;
                delete ready;
                blackH_change = false;
            }
            else                //被删除的节点为黑色叶节点，删除后将黑高改变指示变为true用于往前递推调整红黑树至平衡
            {
                auto ready = rp;
                rp = nullptr;
                delete ready;
                blackH_change = true;
            }
        }
            //只有左孩子或右孩子则该节点必为黑色节点且左右孩子节点必为红色节点，把值对应替换然后正常删除红色孩子节点即可
        else if (!rp->rchild)
        {
            auto ready = rp->lchild;
            rp->data = ready->data;
            rp->lchild = nullptr;
            delete ready;
            blackH_change = false;
        }
        else if (!rp->lchild)
        {
            auto ready = rp->rchild;
            rp->data = ready->data;
            rp->rchild = nullptr;
            delete ready;
            blackH_change = false;
        }
            //既有左孩子又有右孩子，用节点左子树最大的节点替换，然后重新遍历删除被替换的节点，随后将树高指示设为false，因此往回递推过程中
            //不会再重新判断红黑树的平衡性
        else
        {
            auto l = rp->lchild;
            while (l->rchild)
            {
                l = l->rchild;
            }
            auto tep = l->data;
            DeleteNode(head, tep);
            blackH_change = false;
            rp->data = tep;
        }
    }
    else if (rp->data > key)
    {
        DeleteNode(rp->lchild, key);
        if(blackH_change)
        {
            auto s = rp->rchild;
            if (s->color == black)      //兄弟节点是黑色
            {
                if (!s->lchild && !s->rchild)//兄弟节点为空
                {
                    if (rp->color == red)//如果父节点为红色，则p和s调换颜色即可
                    {
                        rp->color = black;
                        s->color = red;
                        blackH_change = false;
                    }
                    else                //如果父节点为黑色，则s设置红色，将p作为新的破坏红黑树特性的节点，往回遍历！！！！
                    {
                        s->color = red;
                        blackH_change = (rp == head) ? false : true;
                    }
                }
                else
                {
                    if (s->lchild && s->lchild->color == red)//兄弟节点左孩子存在且左孩子为红色
                    {
                        R_Rotate(rp->rchild);
                        L_Rotate(rp);
                        rp->rchild->color = black;
                        blackH_change = false;
                    }
                        //兄弟节点右孩子存在且右孩子为红色 或者是 左右都不为空且左右都为红色
                    else if ((s->rchild && s->rchild->color == red) || (s->lchild && s->rchild && s->lchild->color ==red && s->rchild->color == red))
                    {
                        L_Rotate(rp);
                        rp->rchild->color = black;
                        blackH_change = false;
                    }
                        //兄弟节点左右都不为空且左右都为黑色
                    else if (s->lchild && s->rchild && s->lchild->color == black && s->rchild->color == black)
                    {
                        if (rp->color == red)//如果父节点为红色 即红黑树得到平衡
                        {
                            s->color = red;
                            rp->color = black;
                            blackH_change = false;
                        }
                        else if (rp == head)
                        {
                            s->color = red;
                            blackH_change = false;
                        }
                        else
                        {
                            s->color = red;
                            blackH_change = true;
                        }
                    }
                }
            }
            else //兄弟节点为红色
            {
                L_Rotate(rp);
                auto new_s = rp->lchild->rchild;
                //如果新兄弟节点左右为空或者左右均为黑色
                if ((!new_s->lchild && !new_s->rchild)|| (new_s->lchild && new_s->rchild && new_s->rchild->color==black && new_s->lchild->color==black))
                {
                    rp->lchild->color = black;
                    new_s->color = red;
                    blackH_change = false;
                }
                    //最多旋转三次
                else if (new_s->lchild && new_s->lchild->color==red)
                {
                    R_Rotate(rp->lchild->rchild);
                    L_Rotate(rp->lchild);
                    rp->lchild->rchild->color = black;
                    blackH_change = false;
                }
                else if ((new_s->rchild && new_s->rchild->color==red) || (new_s->lchild && new_s->rchild && new_s->rchild->color==red && new_s->lchild->color==red))
                {
                    L_Rotate(rp->lchild);
                    rp->lchild->rchild->color = black;
                    blackH_change = false;
                }

            }
        }

    }
    else
    {
        DeleteNode(rp->rchild, key);
        if (blackH_change)
        {
            auto s = rp->lchild;
            if (s->color == black)
            {
                if (!s->lchild && !s->rchild)
                {
                    if (rp->color == red)
                    {
                        rp->color = black;
                        s->color = red;
                        blackH_change = false;
                    }
                    else
                    {
                        s->color = red;
                        blackH_change = (rp == head) ? false : true;
                    }
                }
                else
                {
                    if (s->rchild && s->rchild->color == red)
                    {
                        L_Rotate(rp->lchild);
                        R_Rotate(rp);
                        rp->lchild->color = black;
                        blackH_change = false;
                    }
                    else if ((s->lchild && s->lchild->color == red) || (s->lchild && s->rchild && s->lchild->color ==red && s->rchild->color == red))
                    {
                        R_Rotate(rp);
                        rp->lchild->color = black;
                        blackH_change = false;
                    }
                    else if (s->lchild && s->rchild && s->lchild->color == black && s->rchild->color == black)
                    {
                        if (rp->color == red)
                        {
                            s->color = red;
                            rp->color = black;
                            blackH_change = false;
                        }
                        else if (rp == head)
                        {
                            s->color = red;
                            blackH_change = false;
                        }
                        else
                        {
                            s->color = red;
                            blackH_change = true;
                        }
                    }
                }
            }
            else
            {
                R_Rotate(rp);
                auto new_s = rp->rchild->lchild;
                if ((!new_s->lchild && !new_s->rchild)|| (new_s->lchild && new_s->rchild && new_s->rchild->color==black && new_s->lchild->color==black))
                {
                    rp->rchild->color = black;
                    new_s->color = red;
                    blackH_change = false;
                }
                    //最多旋转三次
                else if (new_s->rchild && new_s->rchild->color==red)
                {
                    L_Rotate(rp->rchild->lchild);
                    R_Rotate(rp->rchild);
                    rp->rchild->lchild->color = black;
                    blackH_change = false;
                }
                else if((new_s->lchild && new_s->lchild->color==red) || (s->lchild && s->rchild && s->lchild->color ==red && s->rchild->color == red) )
                {
                    R_Rotate(rp->rchild);
                    rp->rchild->lchild->color = black;
                    blackH_change = false;
                }
            }
        }
    }
}

#endif //RB_RB_TREE_H
