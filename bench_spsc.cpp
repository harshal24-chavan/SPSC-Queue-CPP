#include "spscQueue.cpp"

#include <benchmark/benchmark.h>
#include <cstdint>
#include <pthread.h>

// 1. The Data Structure we are testing
struct SyncTask {
  uint64_t user_id_hash;
  int32_t count_to_add;
  uint16_t endpoint_id;
  uint16_t padding;
};

// Global queue for the benchmark to share between threads
static SPSCQueue<SyncTask> g_queue;

// 3. The Benchmark Function
static void BM_SPSC_Throughput(benchmark::State &state) {
  // --- Thread Pinning for i3-540 ---
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);

  // Pin Producer to Core 0, Consumer to Core 1 (Avoid Hyperthreading
  // contention)
  CPU_SET(state.thread_index() % 2, &cpuset);
  pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

  if (state.thread_index() == 0) {
    // PRODUCER
    SyncTask task{987654321, 1, 1, 0};
    for (auto _ : state) {
      task.user_id_hash++; // Force a unique write every time
      while (!g_queue.push(task)) {
        benchmark::DoNotOptimize(g_queue);
      }
    }
    state.SetItemsProcessed(state.iterations());
  } else {
    // CONSUMER
    SyncTask task;
    for (auto _ : state) {
      while (!g_queue.pop(task)) {
        benchmark::DoNotOptimize(g_queue);
      }
      benchmark::DoNotOptimize(task);
    }
  }
}

// 4. Registration
BENCHMARK(BM_SPSC_Throughput)
    ->Threads(2)    // 1 Producer + 1 Consumer
    ->UseRealTime() // Measures wall-clock throughput
    ->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();
