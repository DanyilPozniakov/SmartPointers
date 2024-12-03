cmake_minimum_required(VERSION 3.29)
project(SmartPointers)

set(CMAKE_CXX_STANDARD 23)

add_executable(SmartPointers main.cpp
        SharedPointer/SharedPointer.h)
