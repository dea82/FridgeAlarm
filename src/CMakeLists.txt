cmake_minimum_required(VERSION 3.2.2)

project("FridgeAlarm" C ASM)

# Try to automatically find avr-gcc
find_program(AVR_CC avr-gcc)
if(NOT AVR_CC)
  message(FATAL_ERROR "Could not find avr-gcc")
endif(NOT AVR_CC)

FILE(GLOB SRC_FILES "*.c" "*.S")

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_C_COMPILER ${AVR_CC})
SET(CMAKE_ASM_COMPILER ${AVR_CC})
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

SET(CSTANDARD "-std=gnu99")
SET(CWARN "-Wall -Wstrict-prototypes")
# TODO Make generic and if debugging target optimization should be?
SET(COPT "-Os -lm -lprintf_flt")
SET(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fdata-sections")
# TODO Change to generic mcu
SET(CMCU "-mmcu=attiny13a -DBAUD_RATE=57600UL")
# TODO Change to generic clock speed 
SET(CDEFS "-DF_CPU=9600000UL")
SET(CFLAGS "${CMCU} ${CSTANDARD} ${CWARN} ${COPT} ${CTUNING} ${CDEFS}")
SET(CMAKE_C_FLAGS ${CFLAGS})

SET(CMAKE_ASM_FLAGS ${CMCU})

#set(CMAKE_SYSTEM_INCLUDE_PATH "/usr/local/avr-6.2/avr/include")
#set(CMAKE_SYSTEM_LIBRARY_PATH "/usr/local/avr-6.2/avr/lib")

include_directories("/usr/local/avr-6.2/avr/include")

add_executable(${PROJECT_NAME} ${SRC_FILES} )

message("* ")
message("* Project Name:\t${PROJECT_NAME}")
message("* Project Source Path:\t${SRC_PATH}")
message("* Project Source Files:\t${SRC_FILES}")
message("* ")