add_library(
    presentation.settings
    src/arguments.cpp
)

target_include_directories(
    presentation.settings
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(
    presentation.settings
    PUBLIC
    cxx_std_17
)