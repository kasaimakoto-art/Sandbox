cmake_minimum_required(VERSION 3.10)
project(MyProjectName
    VERSION 1.0.0
    DESCRIPTION "ForCMakeTest"
    LANGUAGES CXX
    )

set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(src)
message("プロジェクトの名前： ${PROJECT_NAME}")
message("プロジェクトのソースディレクトリ： ${PROJECT_SOURCE_DIR}")
message("プロジェクトのビルドディレクトリ： ${PROJECT_BINARY_DIR}")
