add_library(
    domain.common
    src/event_sink.cpp
)

target_include_directories(
    domain.common
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(
    domain.common
    PUBLIC
    cxx_std_17
)