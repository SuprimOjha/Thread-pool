#pragma once
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
namespace threadpool {
class ThreadPool {
public:
    explicit ThreadPool(std::size_t thread_count);
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;
    template <typename F, typename... Args>
    auto submit(F&& function, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>;
    void shutdown();
    std::size_t size() const noexcept;
private:
    using Task = std::function<void()>;
    std::vector<std::thread> workers_;
    std::queue<Task> tasks_;
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    bool stopping_{false};
};
template <typename F, typename... Args>
auto ThreadPool::submit(F&& function, Args&&... args)
    -> std::future<std::invoke_result_t<F, Args...>>
{using ReturnType = std::invoke_result_t<F, Args...>;
    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        [function = std::forward<F>(function),
         ... args = std::forward<Args>(args)]() mutable -> ReturnType {
            return std::invoke(
                std::move(function),
                std::move(args)...
            );
        }
    );
    std::future<ReturnType> result = task->get_future();
    {std::lock_guard<std::mutex> lock(mutex_);
        if (stopping_) {
            throw std::runtime_error(
                "Cannot submit task to a stopped ThreadPool"
            );
        }
        tasks_.emplace([task]() {
            (*task)();
        });
    }
    condition_.notify_one();
    return result;
}
} 