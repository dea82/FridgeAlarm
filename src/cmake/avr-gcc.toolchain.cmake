# Find all executables. Makue they are in your PATH
find_program(AVR_CC avr-gcc)
find_program(AVR_CXX avr-g++)
find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_SIZE_TOOL avr-size)
find_program(AVR_OBJDUMP avr-objdump)

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

  # Check target board parameters
  if(NOT DEFINED MULTI_TARGET OR MULTI_TARGET)
    if(NOT TARGET_BOARDS)
      message(WARNING "Specify valid target boards in TARGET_BOARS.")
    endif()

    if(NOT TARGET_BOARD)
      # Target board has not been set as expected
      if(TARGET_BOARDS)
	# Print all valid target boards
	message("Valid target boards are:")
	foreach(valid_target_board ${TARGET_BOARDS})
	  message(${valid_target_board})
	endforeach()
      endif()
      message(FATAL_ERROR "TARGET_BOARD has not been specified.")
    endif()
    set(FILENAME_POSTFIX -${TARGET_BOARD})
  endif()

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
   set(elf_file ${EXECUTABLE_NAME}${FILENAME_POSTFIX}.elf)
   set(hex_file ${EXECUTABLE_NAME}${FILENAME_POSTFIX}.hex)
   set(map_file ${EXECUTABLE_NAME}${FILENAME_POSTFIX}.map)
   set(lst_file ${EXECUTABLE_NAME}${FILENAME_POSTFIX}.lst)
   set(eeprom_image ${EXECUTABLE_NAME}${FILENAME_POSTFIX}-eeprom.hex)

  if(DEFINED TARGET_BOARD)
    message(STATUS "Building for target board: ${TARGET_BOARD}")
  endif()
  message(STATUS "MCU: " ${MCU_TARGET})
  get_directory_property( DirDefs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )
  message(STATUS "Defines:")
  foreach( d ${DirDefs} )
    message( STATUS ${d} )
  endforeach()


   # elf file
   add_executable(${elf_file} EXCLUDE_FROM_ALL ${ARGN})

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
      COMMAND
         ${AVR_OBJCOPY} -j .text -j .data -O ihex ${elf_file} ${hex_file}
      COMMAND
         ${AVR_SIZE_TOOL} ${AVR_SIZE_ARGS} ${elf_file}
      DEPENDS ${elf_file}
   )

   # eeprom
   add_custom_command(
      OUTPUT ${eeprom_image}
      COMMAND
         ${AVR_OBJCOPY} -j .eeprom --set-section-flags=.eeprom=alloc,load
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
      PROPERTIES
         OUTPUT_NAME "${elf_file}"
   )

   # clean
   get_directory_property(clean_files ADDITIONAL_MAKE_CLEAN_FILES)
   set_directory_properties(
      PROPERTIES
         ADDITIONAL_MAKE_CLEAN_FILES "${map_file}"
   )

   # upload - with avrdude
   add_custom_target(
      upload_${EXECUTABLE_NAME}
      ${AVR_UPLOADTOOL} -p ${MCU_TARGET} -c ${AVR_PROGRAMMER} ${AVR_UPLOADTOOL_OPTIONS}
         -U flash:w:${hex_file}
         -P ${AVR_UPLOADTOOL_PORT}
      DEPENDS ${hex_file}
      COMMENT "Uploading ${hex_file} to ${MCU_TARGET} using ${AVR_PROGRAMMER}"
   )

   # upload eeprom only - with avrdude
   # see also bug http://savannah.nongnu.org/bugs/?40142
   add_custom_target(
      upload_eeprom
      ${AVR_UPLOADTOOL} -p ${MCU_TARGET} -c ${AVR_PROGRAMMER} ${AVR_UPLOADTOOL_OPTIONS}
         -U eeprom:w:${eeprom_image}
         -P ${AVR_UPLOADTOOL_PORT}
      DEPENDS ${eeprom_image}
      COMMENT "Uploading ${eeprom_image} to ${MCU_TARGET} using ${AVR_PROGRAMMER}"
   )

   # get status
   add_custom_target(
      get_status
      ${AVR_UPLOADTOOL} -p ${MCU_TARGET} -c ${AVR_PROGRAMMER} -P ${AVR_UPLOADTOOL_PORT} -n -v
      COMMENT "Get status from ${MCU_TARGET}"
   )

   # get fuses
   add_custom_target(
      get_fuses
      ${AVR_UPLOADTOOL} -p ${MCU_TARGET} -c ${AVR_PROGRAMMER} -P ${AVR_UPLOADTOOL_PORT} -n
         -U lfuse:r:-:b
         -U hfuse:r:-:b
      COMMENT "Get fuses from ${MCU_TARGET}"
   )

   # set fuses
   add_custom_target(
      set_fuses
      ${AVR_UPLOADTOOL} -p ${MCU_TARGET} -c ${AVR_PROGRAMMER} -P ${AVR_UPLOADTOOL_PORT}
         -U lfuse:w:${AVR_L_FUSE}:m
         -U hfuse:w:${AVR_H_FUSE}:m
         COMMENT "Setup: High Fuse: ${AVR_H_FUSE} Low Fuse: ${AVR_L_FUSE}"
   )

   # get oscillator calibration
   add_custom_target(
      get_calibration
         ${AVR_UPLOADTOOL} -p ${MCU_TARGET} -c ${AVR_PROGRAMMER} -P ${AVR_UPLOADTOOL_PORT}
         -U calibration:r:${MCU_TARGET}_calib.tmp:r
         COMMENT "Write calibration status of internal oscillator to ${MCU_TARGET}_calib.tmp."
   )

   # set oscillator calibration
   add_custom_target(
      set_calibration
      ${AVR_UPLOADTOOL} -p ${MCU_TARGET} -c ${AVR_PROGRAMMER} -P ${AVR_UPLOADTOOL_PORT}
         -U calibration:w:${MCU_TARGET}_calib.hex
         COMMENT "Program calibration status of internal oscillator from ${MCU_TARGET}_calib.hex."
   )

   # disassemble
   add_custom_target(
      disassemble_${EXECUTABLE_NAME}
      ${AVR_OBJDUMP} -h -S ${elf_file} > ${EXECUTABLE_NAME}.lst
      DEPENDS ${elf_file}
   )

endfunction()
