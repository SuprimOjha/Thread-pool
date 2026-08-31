cmake_minimum_required(VERSION 3.20)

project(threadpool
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(threadpool
    src/thread_pool.cpp
)

target_include_directories(threadpool
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

if (MSVC)
    target_compile_options(threadpool PRIVATE /W4)
else()
    target_compile_options(threadpool PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

add_executable(threadpool_example
    examples/basic.cpp
)

target_link_libraries(threadpool_example
    PRIVATE threadpool
)

add_executable(threadpool_tests
    tests/test_thread_pool.cpp
)

target_link_libraries(threadpool_tests
    PRIVATE threadpool
)

enable_testing()

add_test(
    NAME threadpool_tests
    COMMAND threadpool_tests
)