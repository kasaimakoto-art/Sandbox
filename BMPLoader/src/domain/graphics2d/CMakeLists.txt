add_library(
    domain.graphics2d
    src/image.cpp
    src/pixel_buffer.cpp
    src/pixel.cpp
)

target_include_directories(
    domain.graphics2d
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(
    domain.graphics2d
    PUBLIC
    cxx_std_17
)