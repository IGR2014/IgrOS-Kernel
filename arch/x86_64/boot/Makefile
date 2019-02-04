################################################################
#
#	Boot folder Make script
#
#	File:	Makefile
#	Date:	21 Apr. 2018
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


IGROS_DIR_PREFIX	= boot

IGROS_DIR_PATH		= $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)

ASM	= as
RM	= rm -rf
OBJ	= $(IGROS_DIR_PATH)/multiboot.o \
	  $(IGROS_DIR_PATH)/gdt.o \
	  $(IGROS_DIR_PATH)/longMode.o \
	  $(IGROS_DIR_PATH)/boot.o \
	  $(IGROS_DIR_PATH)/port.o

.PHONY: all-before all clean

all: all-before $(OBJ)

all-before:
	@mkdir -p $(IGROS_DIR_PATH)

clean:
	@$(RM) $(OBJ)

$(IGROS_DIR_PATH)/%.o: %.s
	@echo "	AS	$<"
	@$(ASM) --64 -o $@ $< -g

