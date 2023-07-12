# 红黑树实现

`RB`为其他人实现代码

`RB_Tree_CP` 在参考代码[1](https://blog.csdn.net/code_peak/article/details/120643910)，[2](https://github.com/Neo-ZK/RB_Tree)基础上实现，其中参考代码2中没有实现方法`Fix_Tree_Delete()` 也就是删除后的调整代码。红黑树删除后的调整是根据[wiki](https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91)中定义实现的，但是删除的场景4没有体现！！！

代码中目前尚未理解`Transplant()`方法。

# benchmark

benchmark使用是参考了[链接🔗](https://github.com/dotnwat/persistent-rbtree)，作者在使用红黑树编写后，使用benchmark做测试，

首先是构建benchmark库，之后使用

```cmake
find_package(benchmark REQUIRED)
```

首先是确保可以搜索到库，然后，可以使用 `target_link_libraries` 命令将 Google Benchmark 库链接到您的目标可执行文件中：

```cmake
target_link_libraries(your_target benchmark::benchmark)
```

完整的cmke文件为

```cmake
cmake_minimum_required(VERSION 3.21)
project(RB_Tree_CP)

find_package(benchmark REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_executable(RB_Tree_CP main.cpp RB_Tree_Node.h RB_Tree.h RB_Tree.cpp bench.cpp)

target_link_libraries(RB_Tree_CP benchmark::benchmark)
```

然后在`bench.cpp`中定义文件并且执行运行

```c++
//
// Created by Shawn Zhao on 2023/7/12.
//

#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mutex>
#include <random>
#include <benchmark/benchmark.h>
#include "RB_Tree.h"
#include "RB_Tree.cpp"


struct rng {
    // 定义一个辅助函数，用于生产随机数
    rng() :
            gen(rd()), // 用于生成随机数
            dis(std::numeric_limits<uint64_t>::min(),
                std::numeric_limits<uint64_t>::max())
    {}
    /*
     * 使用 std::numeric_limits<uint64_t>::min()
     * std::numeric_limits<uint64_t>::max()
     * 初始化了一个均匀分布对象 dis，该对象可以生成 uint64_t 类型的随机数。
     * */

    /*
     * next 函数是一个成员函数，用于生成下一个随机数。在函数内部，
     * 使用 std::lock_guard 对互斥锁 lock 进行了加锁操作，
     * 以保证多线程环境下的线程安全性。然后，调用 dis(gen) 来生成一个随机数，并返回生成的随机数。
     * */
    inline auto next() {
        std::lock_guard<std::mutex> lk(lock);
        return dis(gen);
    }

    std::random_device rd; // 初始化一个随机对象
    std::mt19937 gen; // 使用随机种子初始化了一个 Mersenne Twister 引擎对象
    std::uniform_int_distribution<uint64_t> dis;
    std::mutex lock;
};

// 构建树
static auto buildTree(rng& r, std::size_t size)  {
    RBTree<uint64_t> tree; // 创建树
    // 插入size个数据
    while (size) {
        --size;
        const uint64_t key = r.next();
        tree.Insert(key);
    }
    return tree;
}

static RBTree<uint64_t> tree;

static std::condition_variable cond; // 线程同步原语
static bool init_complete = false;
static std::mutex lock;

static void BM_Insert(benchmark::State& state)
{
    const int tree_size = state.range(0);
    const int num_inserts = state.range(1);
    rng r;

    if (state.thread_index() == 0) {
        std::lock_guard<std::mutex> lk(lock);

        // build the shared tree
        tree = buildTree(r, tree_size);


        // notify build is complete
        init_complete = true;
        cond.notify_all();
    }

    // all threads wait until the tree is built
    std::unique_lock<std::mutex> lk(lock);
    cond.wait(lk, [&] { return init_complete; });
    lk.unlock();

//    assert(tree_size > 0);
//    assert(tree.size() == tree_size);

    // generate set of keys to insert
    std::vector<uint64_t> keys;
    keys.reserve(num_inserts);
//    while (keys.size() < num_inserts) {
//        const auto key = r.next();
//        RBTNode<uint64_t> node = tree.Search(root, key)
//        if (tree.Search(tree.root, key) != NULL) {
//            keys.emplace_back(key);
//        }
//    }

    for (auto _ : state) {
        for (const auto& key : keys) {
            benchmark::DoNotOptimize(tree.Insert(key));
        }
    }

    state.SetItemsProcessed(state.iterations() * keys.size());

    if (state.thread_index() == 0) {
        // tree is cleared when the size changes in the initialization phase. Note
        // that if there are more benchmarks in this executable, we might want to
        // figure out a way to clear the tree after the very last run.
    }
}

BENCHMARK(BM_Insert)
        ->RangeMultiplier(10)
        ->Ranges({{1, 1000}, {10000, 10000}})
        ->ThreadRange(1, 4)
        ->UseRealTime();
// BENCHMARK_MAIN();
/*
这段代码是使用 Google Benchmark 库为名为 `BM_Insert` 的基准测试函数设置参数和选项的部分。

让我们逐行解释代码的含义：

1. `BENCHMARK(BM_Insert)`: 通过 `BENCHMARK` 宏注册名为 `BM_Insert` 的基准测试函数。

2. `->RangeMultiplier(10)`: 使用 `RangeMultiplier` 函数设置参数范围的增长率。这里将参数范围的增长率设置为 10，意味着每次运行测试时，参数的值将乘以 10。

3. `->Ranges({{1, 1000}, {10000, 10000}})`: 使用 `Ranges` 函数设置基准测试的参数范围。这里设置了两个参数范围：第一个参数的范围是从 1 到 1000，第二个参数的范围是从 10000 到 10000。这表示基准测试将在这两个参数范围内进行多次运行。

4. `->ThreadRange(1, 4)`: 使用 `ThreadRange` 函数设置线程范围。这里设置线程范围从 1 到 4，表示基准测试将在 1 到 4 个线程中运行。

5. `->UseRealTime()`: 使用 `UseRealTime` 函数配置基准测试使用真实时间进行测量。这意味着基准测试将使用实际经过的时间作为度量指标，而不是 CPU 时钟周期。

BENCHMARK_MAIN() 是 Google Benchmark 库提供的一个宏，用于生成一个包含 main 函数的入口点，以便运行注册的基准测试。

在调用 BENCHMARK_MAIN() 后，Google Benchmark 库会生成一个默认的 main 函数，用于初始化基准测试并运行注册的基准测试函数。

通常的用法是在源文件的最后调用 BENCHMARK_MAIN()，以便生成可执行文件，并在运行时执行注册的基准测试。

这样，您就可以编译并运行包含基准测试的源文件，Google Benchmark 库会自动执行注册的基准测试，并生成性能报告。
*/
```

使用`BENCHMARK_MAIN();`就需要将main函数的入口函数注释掉

## 运行结果

```shell
Run on (8 X 24.1209 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x8)
Load Average: 2.20, 2.77, 2.66
---------------------------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------------------
BM_Insert/1/10000/real_time/threads:1          10.4 ns         10.3 ns     66335777 items_per_second=0/s
BM_Insert/1/10000/real_time/threads:2          5.24 ns         10.4 ns    133930814 items_per_second=0/s
BM_Insert/1/10000/real_time/threads:4          2.66 ns         10.5 ns    267300260 items_per_second=0/s
BM_Insert/1/10000/real_time/threads:5          2.12 ns         10.5 ns    333544525 items_per_second=0/s
BM_Insert/10/10000/real_time/threads:1         10.1 ns         10.1 ns     69087327 items_per_second=0/s
BM_Insert/10/10000/real_time/threads:2         5.07 ns         10.1 ns    138319932 items_per_second=0/s
BM_Insert/10/10000/real_time/threads:4         2.59 ns         10.3 ns    271794524 items_per_second=0/s
BM_Insert/10/10000/real_time/threads:5         2.09 ns         10.4 ns    336127540 items_per_second=0/s
BM_Insert/100/10000/real_time/threads:1        9.97 ns         9.96 ns     69384043 items_per_second=0/s
BM_Insert/100/10000/real_time/threads:2        5.07 ns         10.1 ns    137012528 items_per_second=0/s
BM_Insert/100/10000/real_time/threads:4        2.58 ns         10.3 ns    271672912 items_per_second=0/s
BM_Insert/100/10000/real_time/threads:5        2.07 ns         10.4 ns    338521550 items_per_second=0/s
BM_Insert/1000/10000/real_time/threads:1      10.00 ns         9.99 ns     70580970 items_per_second=0/s
BM_Insert/1000/10000/real_time/threads:2       5.16 ns         10.2 ns    137607436 items_per_second=0/s
BM_Insert/1000/10000/real_time/threads:4       2.59 ns         10.3 ns    261559384 items_per_second=0/s
BM_Insert/1000/10000/real_time/threads:5       2.09 ns         10.4 ns    335760080 items_per_second=0/s
```

