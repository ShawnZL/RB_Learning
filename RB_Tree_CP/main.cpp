#include <iostream>

#include "RB_Tree.h"
#include "RB_Tree_New.cpp"

int main(int argc, char* argv[])
{
    RB_Tree<int>* m_RB_Tree = new RB_Tree<int>(1);
    int a[10]={1,134,21,235,318,12,34,3,99,198};
    for (int i = 1; i < 10;i++)
    {
        m_RB_Tree->Insert_Node(a[i]);
        std::cout<<"after insert "<<a[i]<<"\n";
    }

    PrintTree(m_RB_Tree->Root_Node);
    for(int i=0;i<10;i++)
    {
        RB_Tree_Node<int>* x = m_RB_Tree ->SearchByKey(m_RB_Tree -> Root_Node, a[i]);
        m_RB_Tree -> Delete_Node(x);
        std::cout<<"after delete "<<a[i]<<": "<<std::endl;
        PrintTree(m_RB_Tree->Root_Node);
    }
    return 0;
}
