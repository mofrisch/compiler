#include <benchmark/benchmark.h>

static void ADD(benchmark::State& state) {
    // Perform setup here

    for (auto _ : state) {
        // This code gets timed
        }
}
// Register the function as a benchmark
BENCHMARK(ADD);
// Run the benchmark
BENCHMARK_MAIN();