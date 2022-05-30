#include <benchmark/benchmark.h>
#include <cstdio>
#include <Utilities.h>

static void BM_extractPathSize(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    extractPathSize("MKDIR12/few/ew/cdas/q",5);
  }
}

static void BM_createPath(benchmark::State& state) {
    
    for(auto _ : state)
    {
        char temp[] {"/rosi/posi/cosi"};
        // std::cout << temp << std::endl;
        createPath(temp, "/tmp");
    }
}

// Register the function as a benchmark
BENCHMARK(BM_extractPathSize);
BENCHMARK(BM_createPath);
// Run the benchmark
BENCHMARK_MAIN();