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
BUILDROOT = output

# List C source files here. (C dependencies are automatically generated.)
SRC = $(wildcard *.c)

# List S (assembler) files here. Mandatory file is boot.S which
# contains the reset and interrupt vector which must be built first!.
ASRC = boot.S $(filter-out boot.S,$(wildcard *.S))

BUILDDIR = $(BUILDROOT)/$(MCU_TARGET)

# Define all object files. Important to build assembly first because
# it contains init0 section.
OBJ = $(ASRC:%.S=$(BUILDDIR)/%.o) $(SRC:%.c=$(BUILDDIR)/%.o)

# Define all listing files.
LST = $(ASRC:%.S=$(BUILDDIR)/%.lst) $(SRC:%.c=$(BUILDDIR)/%.lst)

# Flash file name and path
FLASHFILE = $(BUILDDIR)/$(PROGRAM)_$(MCU_TARGET).hex

# List file name and path
LISTFILE = $(BUILDDIR)/$(PROGRAM)_$(MCU_TARGET).lst

# Optimize for size
OPT = s

CSTANDARD = -std=gnu99

DEBUG = stabs

DEPDIR = $(BUILDDIR)/.d

# Compiler flags to generate dependency files. Name it .Td and rename it after
# successful compile to not pollute with corrupted dependency file.
GENDEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# -flto - link time optimization, removes function jumps etc.
# -fdata-sections -ffunction-sections: Put data and functions
# in separate sections - not linked if not used
COMMONFLAGS = -I. -mmcu=$(MCU_TARGET) -g$(DEBUG) $(GENDEPFLAGS) \
-DF_CPU=$(F_CPU) -flto -fdata-sections -ffunction-sections

CFLAGS = $(COMMONFLAGS)
CFLAGS += -O$(OPT)
CFLAGS += $(CSTANDARD)
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes -Werror=missing-prototypes

ASFLAGS = $(COMMONFLAGS)
ASFLAGS += -x assembler-with-cpp

LDFLAGS = $(COMMONFLAGS)
LDFLAGS += -Wl,-gc-sections # Do not link unused function sections
LDFLAGS += -Wl,--relax
LDFLAGS += -nostartfiles

# Commands
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size

all: check-inputs createdir build size

check-inputs:
ifndef MCU_TARGET
	$(error MCU_TARGET must be specified!)
endif
ifndef F_CPU
	$(error F_CPU must be specified!)
endif

build: $(FLASHFILE) $(LISTFILE)


# Create output dir if not exist
createdir:
	@mkdir -p $(DEPDIR)

# Create final output files (.hex, .eep) from ELF output file.
%.hex:  %.elf
	@echo "Creating load file for Flash:" $@
	@$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

# UNUSED - application does not need eeprom file
%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
$(BUILDDIR)/%.lst: $(BUILDDIR)/%.elf
	@echo "Creating Extended Listing:" $(BUILDDIR)/$@
	@$(OBJDUMP) -h -S $< > $@

# Link: create ELF output file from object files.
# ELF file will not be deleted
.SECONDARY : $(BUILDDIR)/$(PROGRAM)_$(MCU_TARGET).elf
# Object files will not be deleted if make is killed or interrupted.
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo "Linking:" $@
	@$(CC) $(CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

# Compile: create object files from C source files.
$(BUILDDIR)/%.o : %.c $(DEPDIR)/%.d
	@echo "Compiling:" $<
	@$(CC) -c $(CFLAGS) $< -o $@
	@mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

# Assemble: create object files from assembler source files.
$(BUILDDIR)/%.o : %.S $(DEPDIR)/%.d
	@echo "Assembling:" $<
	@$(CC) -c $(ASFLAGS) $< -o $@
	@mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

# Dependency files. These are generated during compile of .c and .S.
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

# Print size of output file
size:
	@echo
	@$(SIZE) -C --mcu=$(MCU_TARGET) $(BUILDDIR)/$(PROGRAM)_$(MCU_TARGET).elf

clean:
	@rm -rf $(BUILDROOT)
	@echo "Clean successfully!"
