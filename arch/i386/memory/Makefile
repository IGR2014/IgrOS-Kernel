################################################################
#
#	Memory folder Make script
#
#	File:	Makefile
#	Date:	04 Jun. 2018
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


IGROS_DIR_PREFIX	= memory

IGROS_DIR_PATH		= $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)

ASM			= as
RM			= rm -rf

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
	@echo "	RM	$(OBJ)"
	@$(RM) $(OBJ)

$(IGROS_DIR_PATH)/%.o: %.s
	@echo "	AS	$<"
	@$(ASM) --32 -o $@ $<

