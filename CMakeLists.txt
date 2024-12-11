cmake_minimum_required(VERSION 3.29)
project(SmartPointers)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

#temp/test
add_executable(test main.cpp)
target_link_libraries(test PUBLIC SmartPointers)

add_subdirectory(SmartPointers)
add_subdirectory(GoogleTests)
add_subdirectory(Benchmarks)
