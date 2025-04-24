# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH ${PICO_SDK_PATH})

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(pico2040 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
# add_subdirectory(moudle)
# add_executable(pico2040 
# moudle/main.c
# moudle/tft_drv.c
# )
add_subdirectory(./moudle/scheduler)
# 获取 moudle 目录下的所有源文件
file(GLOB MODULE_SOURCES "moudle/*.c")

# 添加可执行文件，使用获取的源文件
add_executable(pico2040 
    ${MODULE_SOURCES}
)


pico_set_program_name(pico2040 "pico2040")
pico_set_program_version(pico2040 "0.1")

pico_enable_stdio_uart(pico2040 1)
pico_enable_stdio_usb(pico2040 0)

# Add the standard library to the build
target_link_libraries(pico2040
        pico_stdlib
        )

# Add the standard include files to the build
target_include_directories(pico2040 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  ./moudle
  ./moudle/scheduler
)

# Add any user requested libraries
target_link_libraries(pico2040 
        hardware_i2c
        hardware_pio
        hardware_timer
        hardware_watchdog
        hardware_clocks
        hardware_spi
        hardware_pwm
        scheduler
        )

pico_add_extra_outputs(pico2040)

