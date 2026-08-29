# C++20 Thread Pool

A lightweight thread pool implemented from scratch in C++20 using only the C++ standard library.

The project explores multithreading, task scheduling, synchronization, worker management, and efficient execution of concurrent tasks.

## Features

* C++20 implementation
* Fixed-size worker thread pool
* Thread-safe task queue
* Concurrent task execution
* `std::mutex` and `std::condition_variable` synchronization
* Graceful worker shutdown
* Task submission using `std::future`
* No external dependencies
* Cross-platform design

## Architecture

```text
                    Thread Pool
                        |
                +-------+-------+
                |   Task Queue  |
                +-------+-------+
                        |
          +-------------+-------------+
          |             |             |
          v             v             v
       Worker 1      Worker 2      Worker 3
          |             |             |
          +-------------+-------------+
                        |
                  Task Completion
```

Tasks are submitted to a shared queue. Worker threads wait for available tasks and execute them concurrently. Synchronization primitives ensure that access to shared resources remains thread-safe.

## Example

```cpp
#include "ThreadPool.hpp"
#include <iostream>

int main() {
    ThreadPool pool(4);

    auto task1 = pool.submit([] {
        return 10 + 20;
    });

    auto task2 = pool.submit([] {
        return 50 * 2;
    });

    std::cout << task1.get() << '\n';
    std::cout << task2.get() << '\n';

    return 0;
}
```

Output:

```text
30
100
```

## Requirements

* C++20 compatible compiler
* CMake 3.20 or newer

No third-party libraries are required.

## Building

Clone the repository:

```bash
git clone https://github.com/SuprimOjha/threadpool.git
cd threadpool
```

Create a build directory:

```bash
mkdir build
cd build
```

Configure and build:

```bash
cmake ..
cmake --build .
```

## Project Structure

```text
threadpool/
├── include/
│   └── ThreadPool.hpp
├── src/
│   └── ThreadPool.cpp
├── tests/
├── examples/
├── CMakeLists.txt
├── LICENSE
└── README.md
```

## Concepts Explored

This project was built to understand the fundamentals of concurrent systems programming in modern C++.

Key concepts include:

* Thread creation and management
* Mutual exclusion
* Condition variables
* Producer-consumer patterns
* Task queues
* Futures and asynchronous results
* Race-condition prevention
* Graceful shutdown
* Resource management with RAII

## Future Improvements

* Dynamic worker scaling
* Task priorities
* Work stealing
* Task cancellation
* Performance benchmarking
* CPU affinity
* Improved exception handling
* Stress and concurrency testing

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
