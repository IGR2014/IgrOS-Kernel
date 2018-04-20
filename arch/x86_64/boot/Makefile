################################################################
#
#	Boot folder Make script
#
#	File:	Makefile
#	Date:	20 Apr. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


IGROS_DIR_PREFIX	= boot

ASM	?= as
RM	= rm -rf
OBJ	= $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)/multiboot.o \
	  $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)/gdt.o \
	  $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)/longMode.o \
	  $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)/boot.o \
	  $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)/port.o

.PHONY: all-before all clean

all: all-before $(OBJ)

all-before:
	@mkdir -p $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)

clean:
	@$(RM) $(OBJ)

$(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)/%.o: %.s
	@echo "	AS	$<"
	@$(ASM) --64 -o $@ $<

