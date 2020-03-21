################################################################
#
#	klib folder Make script
#
#	File:	Makefile
#	Date:	12 Feb 2020
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


IGROS_DIR_PREFIX	= klib

IGROS_DIR_PATH		= $(IGROS_DIR_BUILD)/$(IGROS_DIR_PREFIX)

# Source files list
SRCFILES		= $(wildcard *.s)
# Convert sources names into object files names
OBJFILES		= $(patsubst %.s, %.o, $(SRCFILES))
# Specify path for object files
OBJ			= $(addprefix $(IGROS_DIR_PATH)/, $(OBJFILES))

# Phony targets
.PHONY: all-before all clean

all: all-before $(OBJ)

all-before:
	@echo "	MKDIR	$(IGROS_DIR_PATH)"
	@mkdir -p $(IGROS_DIR_PATH)

clean:
	@echo "	RM	$(OBJFILES)"
	@$(RM) $(OBJ)

# Default build rule for all .s files
$(IGROS_DIR_PATH)/%.o: %.s
	@echo "	AS	$<"
	@$(AS) -o $@ $< $(ASFLAGS)

