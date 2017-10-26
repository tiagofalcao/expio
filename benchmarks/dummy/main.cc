#include "benchmark/benchmark.h"

static void BM_Dummy(benchmark::State &state) {
  int count = 0;
  for (auto _ : state) {
    count++;
  }
}
BENCHMARK(BM_Dummy)->Range(8, 8 << 10);

BENCHMARK_MAIN();
