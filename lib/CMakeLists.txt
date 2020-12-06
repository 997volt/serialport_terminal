add_library(
    port 
    port/port.h 
    port/port.c
)

target_include_directories(port PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions(port PRIVATE PORT_VERSION=1.3)