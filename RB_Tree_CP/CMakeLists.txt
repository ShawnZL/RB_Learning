cmake_minimum_required(VERSION 3.21)
project(RB_Tree_CP)

find_package(benchmark REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_executable(RB_Tree_CP main.cpp RB_Tree_Node.h RB_Tree.h RB_Tree.cpp bench.cpp)

target_link_libraries(RB_Tree_CP benchmark::benchmark)
