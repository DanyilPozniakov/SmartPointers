cmake_minimum_required(VERSION 3.29)

project(SmartPointers_TEST)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(IntrusivePtrTest IntrusivePointer_Test.cpp)

target_link_libraries(IntrusivePtrTest PUBLIC gtest gtest_main SmartPointers)

include(GoogleTest)

gtest_discover_tests(IntrusivePtrTest)