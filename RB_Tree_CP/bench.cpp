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
        ->ThreadRange(1, 5)
        // ->Repetitions(30) // 确定重复率
        ->UseRealTime();
BENCHMARK_MAIN(); // 使用就要注释掉主函数


/*
这段代码是使用 Google Benchmark 库为名为 `BM_Insert` 的基准测试函数设置参数和选项的部分。

让我们逐行解释代码的含义：

1. `BENCHMARK(BM_Insert)`: 通过 `BENCHMARK` 宏注册名为 `BM_Insert` 的基准测试函数。

2. `->RangeMultiplier(10)`: 使用 `RangeMultiplier` 函数设置参数范围的增长率。这里将参数范围的增长率设置为 10，意味着每次运行测试时，参数的值将乘以 10。

3. `->Ranges({{1, 1000}, {10000, 10000}})`: 使用 `Ranges` 函数设置基准测试的参数范围。这里设置了两个参数范围：第一个参数的范围是从 1 到 1000，第二个参数的范围是从 10000 到 10000。这表示基准测试将在这两个参数范围内进行多次运行。

4. `->ThreadRange(1, 4)`: 使用 `ThreadRange` 函数设置线程范围。这里设置线程范围从 1 到 4，表示基准测试将在 1 到 4 个线程中运行。

5. `->UseRealTime()`: 使用 `UseRealTime` 函数配置基准测试使用真实时间进行测量。这意味着基准测试将使用实际经过的时间作为度量指标，而不是 CPU 时钟周期。

通过设置这些参数和选项，您可以自定义基准测试的范围、线程数和测量方式，以便进行详细的性能分析和比较。
*/

