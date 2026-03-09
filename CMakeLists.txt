cmake_minimum_required(VERSION 3.10)
project(SPSC_Queue)

set(CMAKE_CXX_STANDARD 20)

# Add the executable and all source files
add_executable(SPSC_Queue main.cpp)

# Link the threading library
find_package(Threads REQUIRED)
target_link_libraries(ts_project Threads::Threads)
