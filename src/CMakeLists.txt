cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/avr-gcc.toolchain.cmake")

project("FridgeAlarm" C ASM)

set(MULTI_TARGET ON)

if(NOT DEFINED TARGET_BOARD)
	message(FATAL_ERROR "TARGET_BOARD must be defined!")
else()
	if(TARGET_BOARD STREQUAL PROTOTYPE)
		SET(MCU "attiny85")
		SET(F_CPU 8000000)
		SET(WCET TRUE)
		SET(BAUD_RATE 57600)
	elseif(TARGET_BOARD STREQUAL PRODUCTION)
		SET(MCU "attiny13a")
		SET(F_CPU 9600000)
		SET(WCET FALSE)
		SET(BAUD_RATE 57600)
	else()
		message(FATAL_ERROR "Unknown value of TARGET_BOARD")
	endif()
endif()

message(STATUS "Building for target board: ${TARGET_BOARD}")
message(STATUS "Current MCU is set to: ${MCU}")
message(STATUS "Current F_CPU is set to: ${F_CPU}")


file(GLOB SRC_FILES "*.S" "*.c" )

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

SET(CSTANDARD "-std=gnu99")
SET(CWARN "-Wall -Wstrict-prototypes")
# TODO Make generic and if debugging target optimization should be?
SET(COPT "-Os -lm -lprintf_flt")
SET(CTUNING "-gstabs -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fdata-sections")
# TODO Change to generic mcu
#SET(CMCU "-mmcu=attiny13a -DBAUD_RATE=57600")
# TODO Change to generic clock speed 
SET(CDEFS "-DF_CPU=${F_CPU}")
SET(CFLAGS "-mmcu=${MCU} -DBAUD_RATE=${BAUD_RATE} -DWCET=${WCET} ${CSTANDARD} ${CWARN} ${COPT} ${CTUNING} ${CDEFS}")
SET(CMAKE_C_FLAGS ${CFLAGS})

SET(CMAKE_ASM_FLAGS "${CFLAGS} -x assembler-with-cpp")

#set(CMAKE_SYSTEM_INCLUDE_PATH "/usr/local/avr-6.2/avr/include")
#set(CMAKE_SYSTEM_LIBRARY_PATH "/usr/local/avr-6.2/avr/lib")

include_directories("/usr/local/avr-6.2/avr/include")

add_avr_executable(${PROJECT_NAME} ${SRC_FILES} )