################################################################
#
#	Memory folder Make script
#
#	File:	Makefile
#	Date:	04 Jun. 2018
#
#	Copyright (c) 2018, Igor Baklykov
#	All rights reserved.
#


IGROS_DIR_PREFIX	= memory

IGROS_DIR_PATH		= $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)

ASM		= as
RM		= rm -rf
OBJ		= $(IGROS_DIR_PATH)/memset.o


all: all-before $(OBJ)

all-before:
	@mkdir -p $(IGROS_DIR_PATH)

clean:
	@$(RM) $(OBJ)

$(IGROS_DIR_PATH)/%.o: %.s
	@echo "	AS	$<"
	@$(ASM) --32 -o $@ $<

