#include "threadpool/thread_pool.hpp"
#include <atomic>
#include <cassert>
#include <iostream>
int main()
{threadpool::ThreadPool pool(4);
    auto result = pool.submit([] {
        return 42;
    });
    assert(result.get() == 42);
    std::atomic<int> counter{0};
    for (int i = 0; i < 1000; ++i) {
        pool.submit([&counter] {
            counter.fetch_add(1, std::memory_order_relaxed);
        });
    }
    pool.shutdown();
    assert(counter == 1000);
    std::cout << "All tests passed.\n";
    return 0;
}