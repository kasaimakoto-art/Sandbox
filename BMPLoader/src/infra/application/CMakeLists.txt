add_library(
    infra.application
    src/event_bus.cpp
)

target_include_directories(
    infra.application
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(
    infra.application
    PUBLIC
    cxx_std_17
)