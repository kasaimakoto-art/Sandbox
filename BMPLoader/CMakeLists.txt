cmake_minimum_required(VERSION 3.20)
project(MyProjectName
    VERSION 1.0.0
    DESCRIPTION "ForCMakeTest"
    LANGUAGES CXX
    )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(src)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_subdirectory(test)

