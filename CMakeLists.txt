cmake_minimum_required(VERSION 3.2.0)

project(bootloader LANGUAGES C ASM)

function(add_bootloader device)
    string(TOUPPER ${device} device_upper)

    add_executable(${device}.elf
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

    target_compile_options(${device}.elf PUBLIC
        -mthumb
        -mcpu=cortex-m3

        -flto
        ${OPT}

        -Wall
        -Wextra
        -Wcast-align
        -Wpointer-arith
        -Wredundant-decls
        -Wshadow
    )

    target_include_directories(${device}.elf PUBLIC
        stm32_lib
        usb_lib
    )

    target_compile_definitions(${device}.elf PUBLIC
        TARGET_${device_upper}
    )

    target_link_libraries(${device}.elf PUBLIC
        -nostartfiles
        -nostdlib
        "-T ${PROJECT_SOURCE_DIR}/stm32_lib/c_only_md_high_density.ld"
    )

    add_custom_target(bootloader-${device}.bin ALL
        COMMAND arm-none-eabi-objcopy -j .isr_vector -j .text -j .data -O binary $<TARGET_FILE:${device}.elf> ${CMAKE_BINARY_DIR}/bootloader-${device}.bin
        DEPENDS ${device}.elf
        BYPRODUCTS bootloader-${device}.bin
    )
endfunction(add_bootloader)

add_bootloader(generic)
add_bootloader(basicpad_v1)
