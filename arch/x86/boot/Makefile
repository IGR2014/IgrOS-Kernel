################################################################
#
#	Boot folder Make script
#
#	File:	Makefile
#	Date:	17 Apr. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


ASM		?= as
RM		= rm -rf
OBJ		= multiboot.o boot.o idt.o gdt.o port.o exceptions.o interrupts.o

all: $(OBJ)

clean:
	${RM} $(OBJ)

%.o: %.s
	$(ASM) --32 -o $@ $<

