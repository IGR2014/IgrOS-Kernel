################################################################
#
#	Boot folder Make script
#
#	File:	Makefile
#	Date:	20 Nov. 2017
#
#	Copyright (c) 2017, Igor Baklykov
#	All rights reserved.
#


ASM		?= as
RM		= rm -rf
OBJ		= multiboot.o boot.o idt.o port.o exceptions.o interrupts.o

all: $(OBJ)

clean:
	${RM} $(OBJ)

%.o: %.s
	$(ASM) --32 -o $@ $<
