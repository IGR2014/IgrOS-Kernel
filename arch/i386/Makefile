################################################################
#
#	i386 folder Make script
#
#	File:	Makefile
#	Date:	23 Jun. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


export IGROS_TARGET			= i386

export IGROS_DIR_TARGET_BUILD		= $(IGROS_DIR_BUILD)/$(IGROS_TARGET)
export IGROS_DIR_TARGET_RELEASE		= $(IGROS_DIR_RELEASE)/$(IGROS_TARGET)

CXX		= g++
CXXFLAGS	= -m32 \
		  -std=c++11 \
		  -Wall \
		  -Wextra \
		  -w \
		  -pedantic \
		  -Werror \
		  -ffreestanding \
		  -fno-exceptions \
		  -fno-rtti \
		  -fno-pic \
		  -nostdlib \
		  -Os \
		  -s \
		  -mno-red-zone \
		  -mno-3dnow \
		  -mno-sse \
		  -mno-sse2 \
		  -mno-sse3 \
		  -mno-mmx
CXXINCLUDE	= -I $(CURDIR) \
		  -I include -I ../../
RM		= rm -rf
BIN_NAME	= kernel.bin
BIN		= $(IGROS_DIR_TARGET_RELEASE)/$(BIN_NAME)
LINKER		= ld
LINKERFLAGS	= -m elf_i386 \
		  -z max-page-size=0x1000
LINKERSCRIPT	= link.ld
OBJ		= $(IGROS_DIR_TARGET_BUILD)/boot/multiboot.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/boot.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/idt.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/gdt.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/port.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/exceptions.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/interrupts.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot/paging.o \
		  $(IGROS_DIR_TARGET_BUILD)/memory/memset.o \
		  $(IGROS_DIR_TARGET_BUILD)/boot.o \
		  $(IGROS_DIR_TARGET_BUILD)/idt.o \
		  $(IGROS_DIR_TARGET_BUILD)/gdt.o \
		  $(IGROS_DIR_TARGET_BUILD)/exceptions.o \
		  $(IGROS_DIR_TARGET_BUILD)/interrupts.o \
		  $(IGROS_DIR_TARGET_BUILD)/vgaConsole.o \
		  $(IGROS_DIR_TARGET_BUILD)/paging.o \
		  $(IGROS_DIR_TARGET_BUILD)/keyboard.o

all: all-before $(BIN)

all-before:
	@mkdir -p $(IGROS_DIR_TARGET_BUILD)
	@mkdir -p $(IGROS_DIR_TARGET_RELEASE)
	@$(MAKE) -C boot
	@$(MAKE) -C memory

rebuild: clean all

clean:
	@$(RM) $(OBJ) $(BIN)

run: $(BIN)
	@echo "	RUN	$(BIN_NAME)"
	@qemu-system-i386 -kernel $(BIN)

$(IGROS_DIR_TARGET_BUILD)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

$(BIN): $(OBJ)
	@echo "	LD	$(BIN_NAME)"
	@$(LINKER) -n $(OBJ) -o $(BIN) -T $(LINKERSCRIPT) $(LINKERFLAGS)

