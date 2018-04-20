################################################################
#
#	i386 folder Make script
#
#	File:	Makefile
#	Date:	20 Apr. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


export IGROS_TARGET			= i386

export IGROS_DIR_TARGET_BUILD		= $(IGROS_DIR_BUILD)/$(IGROS_TARGET)
export IGROS_DIR_TARGET_RELEASE		= $(IGROS_DIR_RELEASE)/$(IGROS_TARGET)

CXX		= g++
CXXFLAGS	= -Wall -m32 -std=c++11 -Wextra -w -pedantic -Werror -ffreestanding -fno-exceptions -fno-rtti -nostdlib -Os -s -mno-red-zone -mno-3dnow -mno-sse -mno-sse2 -mno-sse3 -mno-mmx
CXXINCLUDE	= -I $(CURDIR) -I include -I ../../
RM		= rm -rf
BIN		= kernel.bin
LINKER		= ld
LINKERSCRIPT	= link.ld
OBJ		= $(IGROS_DIR_TARGET_BUILD)/boot/multiboot.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/boot.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/idt.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/gdt.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/port.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/exceptions.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/interrupts.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot.o \
		  $(IGROS_DIR_TARGET_BUILD)/idt.o \
		  $(IGROS_DIR_TARGET_BUILD)/gdt.o \
		  $(IGROS_DIR_TARGET_BUILD)/exceptions.o \
		  $(IGROS_DIR_TARGET_BUILD)/interrupts.o \
		  $(IGROS_DIR_TARGET_BUILD)/videoMem.o

all: all-before $(IGROS_DIR_TARGET_RELEASE)/$(BIN)

all-before:
	@mkdir -p $(IGROS_DIR_TARGET_BUILD)
	@mkdir -p $(IGROS_DIR_TARGET_RELEASE)
	@$(MAKE) -C boot

rebuild: clean all

clean:
	@$(RM) $(OBJ) $(IGROS_DIR_TARGET_RELEASE)/$(BIN)

run: $(IGROS_DIR_TARGET_RELEASE)/$(BIN)
	@echo "	RUN	$(BIN)"
	@qemu-system-i386 -kernel $(IGROS_DIR_TARGET_RELEASE)/$(BIN)

$(IGROS_DIR_TARGET_BUILD)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

$(IGROS_DIR_TARGET_RELEASE)/$(BIN): $(OBJ)
	@echo "	LD	$(BIN)"
	@$(LINKER) -n $(OBJ) -o $(IGROS_DIR_TARGET_RELEASE)/$(BIN) -T $(LINKERSCRIPT) -m elf_i386 -z max-page-size=0x1000

