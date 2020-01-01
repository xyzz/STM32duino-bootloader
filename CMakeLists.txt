cmake_minimum_required(VERSION 3.2.0)

project(bootloader LANGUAGES C ASM)

add_executable(bootloader.elf
    dfu.c
    hardware.c
    main.c
    usb_callbacks.c
    usb_descriptor.c
    usb.c

    usb_lib/usb_core.c
    usb_lib/usb_init.c
    usb_lib/usb_int.c
    usb_lib/usb_mem.c
    usb_lib/usb_regs.c

    stm32_lib/c_only_startup.s
    stm32_lib/cortexm3_macro.s
)

target_compile_options(bootloader.elf PUBLIC
    -mthumb
    -mcpu=cortex-m3

    -flto
    -Os

    -Wall
    -Wextra
    -Wcast-align
    -Wpointer-arith
    -Wredundant-decls
    -Wshadow
)

target_include_directories(bootloader.elf PUBLIC
    stm32_lib
    usb_lib
)

target_compile_definitions(bootloader.elf PUBLIC
    TARGET_BASICPAD_V1
)

target_link_libraries(bootloader.elf PUBLIC
    -nostartfiles
    -nostdlib
    "-T ${PROJECT_SOURCE_DIR}/stm32_lib/c_only_md_high_density.ld"
)

add_custom_target(bootloader.bin ALL
    COMMAND arm-none-eabi-objcopy -j .isr_vector -j .text -j .data -O binary $<TARGET_FILE:bootloader.elf> ${CMAKE_BINARY_DIR}/bootloader.bin
    DEPENDS bootloader.elf
    BYPRODUCTS bootloader.bin
)
