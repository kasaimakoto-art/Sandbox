add_library(
    infra.codecs
    src/bmp.cpp
)

target_include_directories(
    infra.codecs
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    infra.codecs
    PRIVATE
    domain.graphics2d
)

target_compile_features(
    infra.codecs
    PUBLIC
    cxx_std_17
)