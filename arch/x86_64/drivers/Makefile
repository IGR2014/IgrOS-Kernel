################################################################
#
#	Drivers folder Make script
#
#	File:	Makefile
#	Date:	06 Jun 2019
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


IGROS_DIR_PREFIX	= drivers

IGROS_DIR_PATH		= $(IGROS_DIR_TARGET_BUILD)/$(IGROS_DIR_PREFIX)

# Source files list
SRCFILES		= $(wildcard *.cpp)
# Convert sources names into object files names
OBJFILES		= $(patsubst %.cpp, %.o, $(SRCFILES))
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

# Default build rule for all .cpp files
$(IGROS_DIR_PATH)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

