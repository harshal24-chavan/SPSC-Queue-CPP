cmake_minimum_required(VERSION 3.14) # FetchContent works best with 3.14+
project(FastSPSC VERSION 1.0.0 LANGUAGES CXX)

# 1. C++17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Force Release mode for max performance (225M ops/s)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# 3. CPU-Specific Optimizations
# -march=native uses the full power of your i3-540
add_compile_options(-O3 -Wall -Wextra -march=native)

# 4. FetchContent: Automatically download Google Benchmark
include(FetchContent)

FetchContent_Declare(
  google_benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.3
)

# Disable unnecessary parts of the benchmark library to speed up the build
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(google_benchmark)

# 5. Header-Only Library Setup
add_library(spsc_queue INTERFACE)
target_include_directories(spsc_queue INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 6. Benchmark Executable
add_executable(spsc_bench tests/bench_spsc.cpp)

# 7. Linking
# Note: we use benchmark::benchmark (the namespaced target)
find_package(Threads REQUIRED)
target_link_libraries(spsc_bench 
    PRIVATE 
    spsc_queue 
    benchmark::benchmark 
    Threads::Threads
)
