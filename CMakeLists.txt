cmake_minimum_required(VERSION 3.10)
project(FastSPSC VERSION 1.0.0 LANGUAGES CXX)

# 1. Force C++17 Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2. Optimization: Ensure Release builds use -O3 for max throughput
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "Setting build type to 'Release' as none was specified.")
endif()

# 3. Add Compiler Flags for your i3-540
# -march=native tells GCC to use instructions specific to your CPU
add_compile_options(-O3 -Wall -Wextra -march=native)

# 4. Find Dependencies
# On Arch, 'benchmark' package provides these config files
find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)

# 5. Header-Only Library Setup
# We treat the SPSCQueue as an interface library so other targets can link to it
add_library(spsc_queue INTERFACE)
target_include_directories(spsc_queue INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 6. Benchmark Executable
add_executable(spsc_bench tests/bench_spsc.cpp)

# 7. Linking
# Link against our queue, Google Benchmark, and pthread
target_link_libraries(spsc_bench 
    PRIVATE 
    spsc_queue 
    benchmark::benchmark 
    Threads::Threads
)

# 8. (Optional) Install Rules
install(TARGETS spsc_bench DESTINATION bin)
