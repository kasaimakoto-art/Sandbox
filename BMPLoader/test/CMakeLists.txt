
add_executable(
    tests
    image_tests.cpp
    bmp_tests.cpp
)

target_link_libraries(
    tests
    PRIVATE
    gtest_main
    infra.codecs
    domain.graphics2d
)

add_test(NAME all COMMAND tests)

include(CTest)
include(GoogleTest)
gtest_discover_tests(tests)
