//
// Created by Shawn Zhao on 2023/7/10.
//
#include "RB_Tree_Node.h"
#include <iostream>
template<class T>
RB_Tree_Node<T>::RB_Tree_Node(T data_in) : data(data_in), color_tag(0), Left_child(NULL), Right_child(NULL), Father_Node(NULL){
    data = data_in;
    color_tag = 0;
}

// 析构函数
template<class T>
RB_Tree_Node<T>::~RB_Tree_Node<T>(void){}