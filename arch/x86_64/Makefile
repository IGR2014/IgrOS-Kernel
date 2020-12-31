################################################################
#
#	x86_64 folder Make script
#
#	File:	Makefile
#	Date:	30 Jun 2020
#
#	Copyright (c) 2017 - 2021, Igor Baklykov
#	All rights reserved.
#
#


# Source files list
CXXSRCFILES			= $(wildcard *.cpp)
# Convert sources names into object files names
CXXOBJFILES			= $(patsubst %.cpp, %.o, $(CXXSRCFILES))
# Specify path for object files
CXXOBJ				= $(addprefix $(IGROS_DIR_BUILD)/, $(CXXOBJFILES))


# Phony targets
.PHONY: all clean run


all: $(CXXOBJ)
	@echo "	MAKE	boot"
	@$(MAKE) -C boot

clean:
	@echo "	CLEAN	boot"
	@$(MAKE) -C boot clean
	@echo "	RM	$(CXXOBJFILES)"
	@$(RM) $(CXXOBJ)

run: $(BIN)
	@echo "	RUN	$(BIN_NAME)"
	@qemu-system-x86_64 -kernel $(BIN) -m 1G

# Default build rule for all .cpp files
$(IGROS_DIR_BUILD)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

