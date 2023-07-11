#include <random>
#include"RB_Tree.h"

//生成100000个不重复的随机数用来初始化红黑树，然后将数字排序删除到只剩最后10个数字，最终输出最后10个有序数字
int main()
{
    vector<int> temp;
    for (int i = 0; i < 100000; ++i)
    {
        temp.push_back(i + 1);
    }
    // 使用随机数引擎作为随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(temp.begin(), temp.end(), gen);
    int j = temp.size();
    RBTree t(temp);
    sort(temp.begin(), temp.end());
    for (int i = 0; i < j-10;++i)
    {
        t.DeleteNode(temp[i]);
    }
    t.MidOrderTraversal();
}
