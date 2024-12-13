cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(pico_sdk_import.cmake)

project(nokia C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

include_directories(src)
include_directories(include)

add_executable(nokia
    main.cpp
    src/nokia5110.cpp
    src/shapes.cpp
    src/dwm_pico_5110_LCD.c
)

target_link_libraries(nokia
    pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else
    pico_stdlib
    hardware_spi
)


pico_enable_stdio_usb(nokia 1)
pico_enable_stdio_uart(nokia 1)

pico_add_extra_outputs(nokia)

target_link_libraries(nokia pico_stdlib)
