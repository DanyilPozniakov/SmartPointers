FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG        v1.6.1
)
FetchContent_MakeAvailable(benchmark)

add_executable(IntrusivePtrBenchmark IntrusivePointer_Benchmark.cpp)

target_link_libraries(IntrusivePtrBenchmark PUBLIC benchmark::benchmark SmartPointers)