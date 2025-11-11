add_executable(
    app.entrypoint
    src/main.cpp
)

set_target_properties(
    app.entrypoint
    PROPERTIES
    OUTPUT_NAME "BMPLoader"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(
    app.entrypoint
    PRIVATE
    infra.codecs
    presentation.settings
)

target_compile_features(
    app.entrypoint
    PUBLIC
    cxx_std_17
)