################################################################
#
#	x86 folder Make script
#
#	File:	Makefile
#	Date:	17 Apr. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


CXX		?= g++
CXXFLAGS	= -Wall -m32 -g -std=c++11 -Wextra -w -pedantic -Werror -ffreestanding -fno-exceptions -fno-rtti -nostdlib -Os -s
CXXINCLUDE	= -I ${CURDIR} -I include -I ../../
RM		= rm -rf
BIN		?= kernel.bin
OBJ		= boot/multiboot.o boot/boot.o boot/idt.o boot/gdt.o boot/port.o boot/exceptions.o boot/interrupts.o boot.o idt.o gdt.o exceptions.o interrupts.o videoMem.o
LINKER		= ld
LINKERSCRIPT	= link.ld

all: all-before $(BIN)

all-before:
	$(MAKE) -C boot

clean:
	${RM} $(OBJ) $(BIN)

run: $(BIN)
	qemu-system-i386 -kernel $(BIN)

$(BIN): $(OBJ)
	$(LINKER) -n $(OBJ) -o $(BIN) -T $(LINKERSCRIPT) -m elf_i386

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

