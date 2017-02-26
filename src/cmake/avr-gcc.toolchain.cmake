# Find all executables. Makue they are in your PATH
find_program(AVR_CC avr-gcc)
find_program(AVR_CXX avr-g++)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_SIZE_TOOL avr-size)
find_program(AVR_OBJDUMP avr-objdump)

if(NOT AVR_CC)
  message(FATAL_ERROR "avr-gcc not found!")
endif()

# Todo: Check if programs were found. Otherwise there is
# a risk that x86 gcc is called.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_ASM_COMPILER ${AVR_CC})
set(CMAKE_C_COMPILER ${AVR_CC})
set(CMAKE_CXX_COMPILER ${AVR_CXX})
set(CMAKE_LINKER ${AVR_CC})

set(AVR_SIZE_ARGS -B)

function(add_avr_executable EXECUTABLE_NAME MCU_TARGET)

  # MCU shall always be specified!
  if(NOT DEFINED MCU_TARGET)
    message(FATAL_ERROR "MCU has not been specified.")
  endif()

  if(NOT ARGN)
    message(FATAL_ERROR "No source files given for ${EXECUTABLE_NAME}.")
  endif(NOT ARGN)

  # Default to gnu99
  if(NOT DEFINED CSTANDARD)
    set(CSTANDARD "-std=gnu99")
  endif()
    
  SET(CMAKE_C_FLAGS ${CFLAGS})
  
  # set file names
  set(elf_file ${EXECUTABLE_NAME}.elf)
  set(hex_file ${EXECUTABLE_NAME}.hex)
  set(map_file ${EXECUTABLE_NAME}.map)
  set(lst_file ${EXECUTABLE_NAME}.lst)
  set(eeprom_image ${EXECUTABLE_NAME}-eeprom.hex)

  # elf file
  add_executable(${elf_file} EXCLUDE_FROM_ALL ${ARGN})

  message(STATUS "Configuring: ${EXECUTABLE_NAME}")
  message(STATUS "MCU: " ${MCU_TARGET})

  # Optimize for size
  target_compile_options(${elf_file} PRIVATE -Os)
  # Set target MCU
  target_compile_options(${elf_file} PRIVATE -mmcu=${MCU_TARGET})
  target_link_libraries (${elf_file} PRIVATE -mmcu=${MCU_TARGET})
  # Various optimization
  target_compile_options(${elf_file} PRIVATE -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums)
  # Put functions and data in sections and use Link Time Optimization
  target_compile_options(${elf_file} PRIVATE -fdata-sections -ffunction-sections -flto)
  target_link_libraries (${elf_file} PRIVATE -fdata-sections -ffunction-sections -flto)
  # Warnings
  target_compile_options(${elf_file} PRIVATE -Wall -Wextra -Wunused -Wstrict-prototypes -Werror=missing-prototypes -Wunused-macros -Werror)
  # Debug info
  target_compile_options(${elf_file} PRIVATE -gstabs)
  target_link_libraries (${elf_file} PRIVATE -gstabs)
  # Map file
  target_link_libraries (${elf_file} PRIVATE -Wl,-Map,${map_file})
  # Relaxation
  target_link_libraries (${elf_file} PRIVATE -Wl,-gc-sections -mrelax)
  if(WITHOUT_STARTFILE)
    target_link_libraries (${elf_file} PRIVATE -nostartfiles)
  endif()
     
  add_custom_command(
    OUTPUT ${hex_file}
    COMMAND ${AVR_OBJCOPY} -j .text -j .data -O ihex ${elf_file} ${hex_file}
    COMMAND ${AVR_SIZE_TOOL} ${AVR_SIZE_ARGS} ${elf_file}
    DEPENDS ${elf_file}
  )

  # eeprom
  add_custom_command(
    OUTPUT ${eeprom_image}
    COMMAND ${AVR_OBJCOPY} -j .eeprom --set-section-flags=.eeprom=alloc,load
      --change-section-lma .eeprom=0 --no-change-warnings
      -O ihex ${elf_file} ${eeprom_image}
    DEPENDS ${elf_file}
  )

  add_custom_command(
    OUTPUT ${lst_file}
    COMMAND ${AVR_OBJDUMP} -h -S ${elf_file} > ${lst_file}
    DEPENDS ${elf_file}
  )
    

  add_custom_target(
    ${EXECUTABLE_NAME}
    ALL
    DEPENDS ${hex_file} ${eeprom_image} ${lst_file}
  )

  set_target_properties(
    ${EXECUTABLE_NAME}
    PROPERTIES OUTPUT_NAME "${elf_file}"
  )

  # clean
  get_directory_property(clean_files ADDITIONAL_MAKE_CLEAN_FILES)
  set_directory_properties(
    PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${clean_files};${map_file}"
  )

endfunction(add_avr_executable)

function(avr_compile_definitions EXECUTABLE_NAME TARGET_DEFINITIONS)
  target_compile_definitions(${EXECUTABLE_NAME}.elf PRIVATE ${TARGET_DEFINITIONS})
endfunction(avr_compile_definitions)