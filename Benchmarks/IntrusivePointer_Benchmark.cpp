#include <benchmark/benchmark.h>
#include <IntrusivePtr.h>
#include <memory>

class TestClass : public RefCounter
{
public:
    explicit TestClass(int value = 0) : value(value) {}
    int value = 0;
};

class TestClass2
{
public:
    explicit TestClass2(int value = 0) : value(value) {}
    int value = 0;
};

static void BM_CreateDestroy_Intrusive(benchmark::State& state) {
    for (auto _ : state) {
        auto p = make_intrusive<TestClass>();
        benchmark::DoNotOptimize(p);
    }
}

static void BM_CreateDestroy_Shared(benchmark::State& state) {
    for (auto _ : state) {
        auto p = std::make_shared<TestClass2>();
        benchmark::DoNotOptimize(p);
    }
}

static void BM_Copy_Intrusive(benchmark::State& state) {
    auto p = make_intrusive<TestClass>();
    for (auto _ : state) {
        IntrusivePtr<TestClass> copy = p;
        benchmark::DoNotOptimize(copy);
    }
}

static void BM_Copy_Shared(benchmark::State& state) {
    auto p = std::make_shared<TestClass2>();
    for (auto _ : state) {
        std::shared_ptr<TestClass2> copy = p;
        benchmark::DoNotOptimize(copy);
    }
}

static void BM_Dereference_Intrusive(benchmark::State& state) {
    IntrusivePtr<TestClass> p = make_intrusive<TestClass>();
    for (auto _ : state) {
        auto& ref = *p;
        benchmark::DoNotOptimize(ref);
    }
}

static void BM_Dereference_Shared(benchmark::State& state) {
    std::shared_ptr<TestClass2> p = std::make_shared<TestClass2>();
    for (auto _ : state) {
        auto& ref = *p;
        benchmark::DoNotOptimize(ref);
    }
}

static void BM_MassCreateDestroy_Intrusive(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<IntrusivePtr<TestClass>> vec;
        vec.reserve(1000);
        for (int i = 0; i < 1000; ++i) {
            vec.push_back(make_intrusive<TestClass>());
        }
        benchmark::DoNotOptimize(vec);
    }
}

static void BM_MassCreateDestroy_Shared(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::shared_ptr<TestClass2>> vec;
        vec.reserve(1000);
        for (int i = 0; i < 1000; ++i) {
            vec.push_back(std::make_shared<TestClass2>());
        }
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(BM_MassCreateDestroy_Intrusive);
BENCHMARK(BM_MassCreateDestroy_Shared);


BENCHMARK(BM_Dereference_Intrusive);
BENCHMARK(BM_Dereference_Shared);

BENCHMARK(BM_Copy_Intrusive);
BENCHMARK(BM_Copy_Shared);

BENCHMARK(BM_CreateDestroy_Intrusive);
BENCHMARK(BM_CreateDestroy_Shared);

BENCHMARK_MAIN();
