# The MIT License (MIT)
#
# Copyright (c) 2015-2016 Andreas L.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# Program name
PROGRAM = FridgeAlarm

# Build directory
BUILDDIR = output

# Default target
#MCU_TARGET = attiny85

attiny13a: MCU_TARGET=attiny13a
attiny13a: F_CPU := 9600000
#attiny13a: LFUSE ?= 0xFF #FIXME: Change to ...
#attiny13a: HFUSE ?= 0xFF #FIXME: Change to ...
#attiny13a: EFUSE ?= 0xFF #FIXME: Change to ...
attiny13a: createdir $(PROGRAM)_attiny13a.hex
#attiny13a: $(PROGRAM)_attiny85.lst

attiny85: MCU_TARGET = attiny85
attiny85: F_CPU = 8000000
attiny85: LFUSE ?= 0xFF #FIXME: Change to ...
attiny85: HFUSE ?= 0xFF #FIXME: Change to ...
attiny85: EFUSE ?= 0xFF #FIXME: Change to ...
attiny85: $(BUILDDIR)/$(MCU_TARGET)/$(PROGRAM)_$(MCU_TARGET).hex
#attiny85: $(PROGRAM)_attiny85.lst



# List C source files here. (C dependencies are automatically generated.)
SRC = $(wildcard *.c)

# List S (assembler) files here. Mandatory file is boot.S which
# contains the reset and interrupt vector.
ASRC = boot.S $(filter-out boot.S,$(wildcard *.S))

# Define all object files. Important to build assembly first because
# it contains init0 section.
OBJ = $(ASRC:%.S=$(OUTDIR)/%.o) $(SRC:%.c=$(OUTDIR)/%.o)

# Define all listing files.
LST = $(ASRC:%.S=$(OUTDIR)/%.lst) $(SRC:%.c=$(OUTDIR)/%.lst)

# Optimize for size
OPT = s

CSTANDARD = -std=gnu99

DEBUG = stabs

# Compiler flags to generate dependency files.
GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d




# Output format. (can be srec, ihex, binary)
FORMAT = ihex

COMMONFLAGS = -mmcu=$(MCU_TARGET) -g$(DEBUG) -DF_CPU=$(F_CPU) -flto

CFLAGS = $(COMMONFLAGS)
CFLAGS += -O$(OPT)
CFLAGS += $(CSTANDARD)
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
# Put data and functions in separate sections - not linked if not used
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -Wall -Wstrict-prototypes -Werror=missing-prototypes

ASFLAGS = $(COMMONFLAGS)
ASFLAGS += -fdata-sections -ffunction-sections

LDFLAGS = $(COMMONFLAGS)
LDFLAGS += -Wl,-gc-sections # Do not link unused function sections
LDFLAGS += -Wl,--relax
LDFLAGS += -nostartfiles

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -I. $(CFLAGS)# $(GENDEPFLAGS)
ALL_ASFLAGS = -I. -x assembler-with-cpp $(ASFLAGS)

# Commands
CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size

# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before:
MSG_SIZE_AFTER = Size after:
MSG_COFF = Converting to AVR COFF:
MSG_EXTENDED_COFF = Converting to AVR Extended COFF:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:


FORCE:


# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create output dir if not exist
createdir:
	mkdir -p $(BUILDDIR)
size:
	$(SIZE) -C --mcu=$(MCU_TARGET) $(OUTDIR)/$(PROGRAM)_$(MCU_TARGET).elf

# Link: create ELF output file from object files.
.SECONDARY : $(OUTDIR)/$(PROGRAM)_$(MCU_TARGET).elf
.PRECIOUS : $(OBJ)
$(OUTDIR)/%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
$(BUILDDIR)/%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@


# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(BUILDDIR)/%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@
