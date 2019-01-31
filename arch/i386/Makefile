################################################################
#
#	i386 folder Make script
#
#	File:	Makefile
#	Date:	08 Aug. 2018
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


export IGROS_DIR_TARGET_BUILD		= $(IGROS_DIR_BUILD)/$(IGROS_ARCH)
export IGROS_DIR_TARGET_RELEASE		= $(IGROS_DIR_RELEASE)/$(IGROS_ARCH)

BIN					= $(IGROS_DIR_TARGET_RELEASE)/$(BIN_NAME)

LDSCRIPT				= link.ld

OBJ		= $(IGROS_DIR_TARGET_BUILD)/boot/multiboot.o	\
		  $(IGROS_DIR_TARGET_BUILD)/boot/boot.o		\
		  $(IGROS_DIR_TARGET_BUILD)/boot/idt.o		\
		  $(IGROS_DIR_TARGET_BUILD)/boot/gdt.o		\
		  $(IGROS_DIR_TARGET_BUILD)/boot/port.o		\
		  $(IGROS_DIR_TARGET_BUILD)/boot/exceptions.o	\
		  $(IGROS_DIR_TARGET_BUILD)/boot/interrupts.o	\
		  $(IGROS_DIR_TARGET_BUILD)/boot/paging.o	\
		  $(IGROS_DIR_TARGET_BUILD)/boot/cr.o	\
		  $(IGROS_DIR_TARGET_BUILD)/memory/memset.o	\
		  $(IGROS_DIR_TARGET_BUILD)/boot.o		\
		  $(IGROS_DIR_TARGET_BUILD)/idt.o		\
		  $(IGROS_DIR_TARGET_BUILD)/gdt.o		\
		  $(IGROS_DIR_TARGET_BUILD)/exceptions.o	\
		  $(IGROS_DIR_TARGET_BUILD)/interrupts.o	\
		  $(IGROS_DIR_TARGET_BUILD)/vgaConsole.o	\
		  $(IGROS_DIR_TARGET_BUILD)/paging.o		\
		  $(IGROS_DIR_TARGET_BUILD)/keyboard.o		\
		  $(IGROS_DIR_TARGET_BUILD)/pit.o

all: all-before $(BIN)

all-before:
	@$(MAKE) -C boot
	@$(MAKE) -C memory

rebuild: clean all

clean:
	@$(RM) $(OBJ) $(BIN)

run: $(BIN)
	@echo "	RUN	$(BIN_NAME)"
	@qemu-system-i386 -kernel $(BIN)

$(IGROS_DIR_TARGET_BUILD)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

$(BIN): $(OBJ)
	@echo "	LD	$(BIN_NAME)"
	@$(LD) -n $(OBJ) -o $(BIN) -T $(LDSCRIPT) $(LDFLAGS)

