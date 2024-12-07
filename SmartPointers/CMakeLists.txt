add_library(SmartPointers INTERFACE SharedPointer.h IntrusivePtr.h)
target_include_directories(SmartPointers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})