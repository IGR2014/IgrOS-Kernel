################################################################
#
#	klib folder Make script
#
#	File:	Makefile
#	Date:	14 Feb 2020
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


IGROS_DIR_PREFIX	= klib

IGROS_DIR_PATH		= $(IGROS_DIR_BUILD)/$(IGROS_DIR_PREFIX)

# Source files list
CXXSRCFILES		= $(wildcard *.cpp)
# Convert sources names into object files names
CXXOBJFILES		= $(patsubst %.cpp, %.o, $(CXXSRCFILES))
# Specify path for object files
CXXOBJ			= $(addprefix $(IGROS_DIR_PATH)/, $(CXXOBJFILES))


# Phony targets
.PHONY: all-before all clean


all: all-before $(CXXOBJ)

all-before:
	@echo "	MKDIR	$(IGROS_DIR_PATH)"
	@mkdir -p $(IGROS_DIR_PATH)

clean:
	@echo "	RM	$(CXXOBJFILES)"
	@$(RM) $(CXXOBJ)

# Default build rule for all .cpp files
$(IGROS_DIR_PATH)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

