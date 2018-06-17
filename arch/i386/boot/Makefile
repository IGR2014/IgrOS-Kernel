################################################################
#
#	Boot folder Make script
#
#	File:	Makefile
#	Date:	18 Jun. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


IGROS_DIR_PREFIX	= boot

IGROS_DIR_PATH		= $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)

ASM		= as
RM		= rm -rf
OBJ		= $(IGROS_DIR_PATH)/multiboot.o \
		  $(IGROS_DIR_PATH)/boot.o \
		  $(IGROS_DIR_PATH)/idt.o \
		  $(IGROS_DIR_PATH)/gdt.o \
		  $(IGROS_DIR_PATH)/port.o \
		  $(IGROS_DIR_PATH)/exceptions.o \
		  $(IGROS_DIR_PATH)/interrupts.o \
		  $(IGROS_DIR_PATH)/paging.o


all: all-before $(OBJ)

all-before:
	@mkdir -p $(IGROS_DIR_PATH)

clean:
	@$(RM) $(OBJ)

$(IGROS_DIR_PATH)/%.o: %.s
	@echo "	AS	$<"
	@$(ASM) --32 -o $@ $<

