################################################################
#
#	klib folder Make script
#
#	File:	Makefile
#	Date:	22 May 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


IGROS_DIR_PREFIX	= klib

IGROS_DIR_PATH		= $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)

# Source files list
AS_SRCFILES		= $(wildcard *.s)
# Convert sources names into object files names
AS_OBJFILES		= $(patsubst %.s, %.o, $(AS_SRCFILES))
# Specify path for object files
AS_OBJ			= $(addprefix $(IGROS_DIR_PATH)/, $(AS_OBJFILES))

# Source files list
CXX_SRCFILES		= $(wildcard *.cpp)
# Convert sources names into object files names
CXX_OBJFILES		= $(patsubst %.cpp, %.o, $(CXX_SRCFILES))
# Specify path for object files
CXX_OBJ			= $(addprefix $(IGROS_DIR_PATH)/, $(CXX_OBJFILES))

# Phony targets
.PHONY: all-before all clean

all: all-before $(AS_OBJ) $(CXX_OBJ)

all-before:
	@echo "	MKDIR	$(IGROS_DIR_PATH)"
	@mkdir -p $(IGROS_DIR_PATH)

clean:
	@echo "	RM	$(AS_OBJFILES) $(CXX_OBJFILES)"
	@$(RM) $(AS_OBJ) $(CXX_OBJ)

# Default build rule for all .s files
$(IGROS_DIR_PATH)/%.o: %.s
	@echo "	AS	$<"
	@$(AS) -o $@ $< $(ASFLAGS)

# Default build rule for all .cpp files
$(IGROS_DIR_PATH)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

