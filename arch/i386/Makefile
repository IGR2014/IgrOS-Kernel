################################################################
#
#	i386 folder Make script
#
#	File:	Makefile
#	Date:	10 Oct 2019
#
#	Copyright (c) 2017 - 2019, Igor Baklykov
#	All rights reserved.
#
#


export IGROS_DIR_TARGET_BUILD		= $(IGROS_DIR_BUILD)/$(IGROS_ARCH)
export IGROS_DIR_TARGET_RELEASE		= $(IGROS_DIR_RELEASE)/$(IGROS_ARCH)

BIN					= $(IGROS_DIR_TARGET_RELEASE)/$(BIN_NAME)

LDSCRIPT				= link.ld

OBJ		= $(IGROS_DIR_TARGET_BUILD)/boot/multiboot.o		\
		  $(IGROS_DIR_TARGET_BUILD)/boot/boot.o			\
		  $(IGROS_DIR_TARGET_BUILD)/boot/idt.o			\
		  $(IGROS_DIR_TARGET_BUILD)/boot/gdt.o			\
		  $(IGROS_DIR_TARGET_BUILD)/boot/port.o			\
		  $(IGROS_DIR_TARGET_BUILD)/boot/isr.o			\
		  $(IGROS_DIR_TARGET_BUILD)/boot/irq.o			\
		  $(IGROS_DIR_TARGET_BUILD)/boot/exceptions.o		\
		  $(IGROS_DIR_TARGET_BUILD)/boot/paging.o		\
		  $(IGROS_DIR_TARGET_BUILD)/boot/cr.o			\
		  $(IGROS_DIR_TARGET_BUILD)/klib/memset.o		\
		  $(IGROS_DIR_TARGET_BUILD)/../klib/kstring.o		\
		  $(IGROS_DIR_TARGET_BUILD)/../klib/kprint.o		\
		  $(IGROS_DIR_TARGET_BUILD)/../klib/kmath.o		\
		  $(IGROS_DIR_TARGET_BUILD)/../multiboot/multiboot.o	\
		  $(IGROS_DIR_TARGET_BUILD)/../mem/mmap.o		\
		  $(IGROS_DIR_TARGET_BUILD)/drivers/vmem.o		\
		  $(IGROS_DIR_TARGET_BUILD)/drivers/keyboard.o		\
		  $(IGROS_DIR_TARGET_BUILD)/drivers/pit.o		\
		  $(IGROS_DIR_TARGET_BUILD)/idt.o			\
		  $(IGROS_DIR_TARGET_BUILD)/gdt.o			\
		  $(IGROS_DIR_TARGET_BUILD)/isr.o			\
		  $(IGROS_DIR_TARGET_BUILD)/irq.o			\
		  $(IGROS_DIR_TARGET_BUILD)/exceptions.o		\
		  $(IGROS_DIR_TARGET_BUILD)/paging.o			\
		  $(IGROS_DIR_TARGET_BUILD)/../../kmain.o


.PHONY:	all-before all rebuild clean distclean run

all: all-before $(BIN)

all-before:
	@echo "	MAKE	boot"
	@$(MAKE) -C boot
	@echo "	MAKE	klib"
	@$(MAKE) -C klib
	@echo "	MAKE	drivers"
	@$(MAKE) -C drivers

clean:
	@echo "	CLEAN	boot"
	@$(MAKE) -C boot clean
	@echo "	CLEAN	klib"
	@$(MAKE) -C klib clean
	@echo "	CLEAN	drivers"
	@$(MAKE) -C drivers clean
	@echo "	RM	$(OBJ)"
	@$(RM) $(OBJ)

distclean:
	@echo "	RM	$(BIN_NAME)"
	@$(RM) $(BIN)

run: $(BIN)
	@echo "	RUN	$(BIN_NAME)"
	@qemu-system-i386 -kernel $(BIN) -m 1G

# Default build rule for all .cpp files
$(IGROS_DIR_TARGET_BUILD)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

$(BIN): $(OBJ)
	@echo "	LD	$(BIN_NAME)"
	@$(LD) $(OBJ) -o $(BIN) -T $(LDSCRIPT) $(LDFLAGS)

