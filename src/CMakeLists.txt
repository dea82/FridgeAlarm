cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/avr-gcc.toolchain.cmake")
project("FridgeAlarm" C ASM)
# If project can be build for different target boards.
set(MULTI_TARGET ON)
# List target boards
set(TARGET_BOARDS PRODUCTION PROTOTYPE)

set(WITHOUT_STARTFILE ON)

# Common source files for all targets
set(COMMON_SRC boot.S butt.c buzz.c cont.c door.c eepr.c gpio.c ledc.c main.c pwrd.c wdtc.c)

if(TARGET_BOARD STREQUAL PROTOTYPE)
  set(MCU attiny85)
  add_definitions(-DF_CPU=8000000)
  add_definitions(-DWCET=1)
  add_definitions(-DBAUD_RATE=57600)
  SET(SPECIFIC_SRC uart_asm.S uart.c wcet_asm.S wcet.c)
elseif(TARGET_BOARD STREQUAL PRODUCTION)
  set(MCU attiny13a)
  add_definitions(-DF_CPU=9600000)
endif()

set(SRC_FILES ${COMMON_SRC} ${SPECIFIC_SRC})

# Uncomment if avr-libc is not in your PATH
#include_directories("path/to/avr-libc")

add_avr_executable(${PROJECT_NAME} ${MCU} ${SRC_FILES})
