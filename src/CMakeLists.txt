cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/avr-gcc.toolchain.cmake")
project("FridgeAlarm" C ASM)

set(WITHOUT_STARTFILE ON)

# Common source files for all targets
set(COMMON_SRC boot.S butt.c buzz.c conf.c cont.c door.c eepr.c ledc.c main.c pwrd.c util.c wdtc.c)

# Uncomment if avr-libc is not in your PATH
include_directories("/usr/local/avr-gcc-6.3.0/avr/include/")

add_avr_executable(PRODUCTION attiny13a ${COMMON_SRC})
avr_compile_definitions(PRODUCTION -DF_CPU=9600000)

add_avr_executable(PROTOTYPE_WITHOUT_WCET attiny85 ${COMMON_SRC})
avr_compile_definitions(PROTOTYPE_WITHOUT_WCET -DF_CPU=8000000)

add_avr_executable(PROTOTYPE_WITH_WCET attiny85 ${COMMON_SRC} uart_asm.S uart.c wcet_asm.S wcet.c)
avr_compile_definitions(PROTOTYPE_WITH_WCET -DF_CPU=8000000)
avr_compile_definitions(PROTOTYPE_WITH_WCET -DWCET=1)
avr_compile_definitions(PROTOTYPE_WITH_WCET -DBAUD_RATE=57600)