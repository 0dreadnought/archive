cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(protocol C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(protocol
  main.c
  protocol.c
)

pico_enable_stdio_usb(protocol 1)
pico_enable_stdio_uart(protocol 0)

pico_add_extra_outputs(protocol)

target_link_libraries(protocol pico_stdlib)
