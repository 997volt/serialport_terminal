add_executable(serialport main.c)

target_link_libraries(serialport PRIVATE port)