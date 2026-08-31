#include "threadpool/thread_pool.hpp"
#include <stdexcept>
namespace threadpool {
ThreadPool::ThreadPool(std::size_t thread_count)
{
    if (thread_count == 0) {
        throw std::invalid_argument(
            "ThreadPool requires at least one worker thread"
        );
    }
    workers_.reserve(thread_count);
    for (std::size_t i = 0; i < thread_count; ++i) {
        workers_.emplace_back([this] {
            while (true) {
                Task task;
                {std::unique_lock<std::mutex> lock(mutex_);
                    condition_.wait(lock, [this] {
                        return stopping_ || !tasks_.empty();
                    });
                    if (stopping_ && tasks_.empty()) {
                        return;
                    }
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();
            }
        });
    }
}
ThreadPool::~ThreadPool()
{
    shutdown();
}
void ThreadPool::shutdown()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopping_) {
            return;
        }
        stopping_ = true;
    }
    condition_.notify_all();
    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}
std::size_t ThreadPool::size() const noexcept
{
return workers_.size();
}
} 