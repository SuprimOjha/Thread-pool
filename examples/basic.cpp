#include "threadpool/thread_pool.hpp"
#include <chrono>
#include <iostream>
#include <thread>
int main()
{threadpool::ThreadPool pool(4);
    auto result1 = pool.submit([] {
        return 10 + 20;
    });
    auto result2 = pool.submit([] {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(500)
        );
        return 100;
    });
    auto result3 = pool.submit([] {
        return std::string("Thread Pool is working");
    });
    std::cout << "Result 1: " << result1.get() << '\n';
    std::cout << "Result 2: " << result2.get() << '\n';
    std::cout << "Result 3: " << result3.get() << '\n';
    return 0;
}