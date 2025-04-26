cmake_minimum_required(VERSION 3.13)

# 初始化 Pico SDK 路径和配置
set(PICO_SDK_PATH ${PICO_SDK_PATH})
set(PICO_BOARD pico CACHE STRING "Board type")

# 包含 Pico SDK（必须在 project() 前）
include(pico_sdk_import.cmake)

# 检查 SDK 版本
if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# 定义项目名称
project(pico2040 C CXX ASM)

# 初始化 SDK（必须在 add_executable 前）
pico_sdk_init()

# 添加子模块目录（确保路径正确）
add_subdirectory(moudle/scheduler)

# 收集模块源文件（建议明确列出文件，避免 GLOB）
file(GLOB MODULE_SOURCES "moudle/*.c")

# 添加可执行文件（先定义目标）
add_executable(pico2040 
    ${MODULE_SOURCES}
)

# 启用 nano 标准库和体积优化
add_compile_options(
    -Os
    -ffunction-sections
    -fdata-sections
    --specs=nano.specs
    -Wl,-u,_printf_float  # 禁用浮点 printf
)

target_link_options(pico2040 PRIVATE
    -Wl,--gc-sections
    -Wl,--print-memory-usage  # 输出内存使用报告
)
# 全局宏定义（针对整个项目）
add_definitions(
    -DPICO_FLASH_SIZE_BYTES=0x1000000  # 16MB Flash
    -DG_LOG_ENABLED=1 
    -DEVT_LOG_ENABLED=0
)

# 标准设置
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 配置项目元信息
pico_set_program_name(pico2040 "pico2040")
pico_set_program_version(pico2040 "0.1")

# 启用/禁用标准输入输出
pico_enable_stdio_usb(pico2040 1)   # 启用 USB 作为 stdio
pico_enable_stdio_uart(pico2040 0)  # 禁用 UART 作为 stdio

# 添加头文件路径
target_include_directories(pico2040 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/..
    ./moudle
    ./moudle/scheduler
)

# 链接必要的库
target_link_libraries(pico2040
    pico_stdlib
    tinyusb_device
    tinyusb_board
    hardware_i2c
    hardware_pio
    hardware_timer
    hardware_watchdog
    hardware_clocks
    hardware_spi
    hardware_pwm
    scheduler  # 来自 add_subdirectory(moudle/scheduler)
)

set(PICO_UF2_COMPRESS ON)
set(CMAKE_BUILD_TYPE Release)
# pico_add_extra_outputs(pico2040)

# 生成 UF2 等输出文件
pico_add_extra_outputs(pico2040)

