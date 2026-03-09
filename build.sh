#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# 1. Setup Directories
BUILD_DIR="build"

# 2. Cleanup (Optional: uncomment if you want a totally fresh build every time)
# rm -rf $BUILD_DIR

# 3. Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# 4. Configure with CMake (Force Release mode for performance)
echo "--- Configuring Project (Release Mode) ---"
cmake -DCMAKE_BUILD_TYPE=Release ..

# 5. Build the Project
echo "--- Compiling with multiple Threads ---"
make -j$(nproc)

# 6. Run the Benchmark
echo "--- Running SPSC Benchmark ---"
if [ -f "./spsc_bench" ]; then
    ./spsc_bench
else
    echo "Error: spsc_bench binary not found!"
    exit 1
fi
