################################################################
#
#	Main Kernel Make script
#
#	File:	Makefile
#	Date:	28 Jun 2020
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


# Config directory path
IGROS_CONFIG_DIR		= config
IGROS_CONFIG_MAKE_DIR		= $(IGROS_CONFIG_DIR)/make

# Arch define, build and release folders
export IGROS_ARCH		?= i386
export IGROS_DIR_BUILD		= $(CURDIR)/build/$(IGROS_ARCH)
export IGROS_DIR_RELEASE	= $(CURDIR)/release/$(IGROS_ARCH)

# Include files patchs
export CXXINCLUDE		= -I$(CURDIR)/include/					\
				  -I$(CURDIR)/include/arch/$(IGROS_ARCH)/

# Import config Makefile
IGROS_CONFIG			= $(IGROS_CONFIG_MAKE_DIR)/Makefile-$(IGROS_ARCH)-$(CXX).in
include $(IGROS_CONFIG)

# Source files list
CXXSRCFILES			= $(wildcard *.cpp)
# Convert sources names into object files names
CXXOBJFILES			= $(patsubst %.cpp,%.o,$(CXXSRCFILES))
# Specify path for object files
CXXOBJ				= $(addprefix $(IGROS_DIR_BUILD)/, $(CXXOBJFILES))

#
LDSRCFILES			= $(wildcard *.cpp)
LDSRCFILES			+= $(subst arch/$(IGROS_ARCH)/,,$(wildcard arch/$(IGROS_ARCH)/boot/*.s))
LDSRCFILES			+= $(subst arch/$(IGROS_ARCH)/,,$(wildcard arch/$(IGROS_ARCH)/klib/*.s))
LDSRCFILES			+= $(subst arch/$(IGROS_ARCH)/,,$(wildcard arch/$(IGROS_ARCH)/*.cpp))
LDSRCFILES			+= $(wildcard multiboot/*.cpp)
LDSRCFILES			+= $(wildcard klib/*.cpp)
LDSRCFILES			+= $(wildcard mem/*.cpp)
LDSRCFILES			+= $(wildcard drivers/*.cpp)
#
LDOBJFILESTMP			= $(patsubst %.s,%.o,$(LDSRCFILES))
LDOBJFILES			= $(patsubst %.cpp,%.o,$(LDOBJFILESTMP))
#
OBJ				= $(addprefix $(IGROS_DIR_BUILD)/, $(LDOBJFILES))

# Compiled binary
BIN_NAME			= kernel.bin
BIN				= $(IGROS_DIR_RELEASE)/$(BIN_NAME)

# Linker script
LDSCRIPT			= config/link/link-$(IGROS_ARCH).ld

# Distribution iso parameters
IGROS_DIR_ISO			= iso
IGROS_DIR_ISO_BOOT		= $(IGROS_DIR_ISO)/boot
IGROS_DIR_ISO_GRUB		= $(IGROS_DIR_ISO_BOOT)/grub
IGROS_GRUB_CONFIG		= $(IGROS_DIR_ISO_GRUB)/grub.cfg
IGROS_ISO			= os-$(IGROS_ARCH).iso


# Phony targets
.PHONY: all-before all clean distclean rebuild run test deploy


# Default build rule for all .cpp files
$(IGROS_DIR_BUILD)/%.o: %.cpp
	@echo "	C++	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXINCLUDE)

# Default make target
all: all-before $(BIN)

# Print variables before build
all-before:
	@echo "	CXX:	$(CXX)"
	@echo "	Script:	$(IGROS_CONFIG)"

# Clean build
clean:
	@echo "	CLEAN	$(IGROS_DIR_BUILD)"
	@$(MAKE) -C multiboot clean
	@$(MAKE) -C arch/$(IGROS_ARCH) clean
	@$(MAKE) -C klib clean
	@$(MAKE) -C mem clean
	@$(MAKE) -C drivers clean
	@echo "	RM	$(IGROS_DIR_BUILD)"
	@rm -rf $(IGROS_DIR_BUILD)

# Clean only existing kernel binary
distclean:
	@echo "	RM	$(BIN)"
	@rm -rf $(BIN)

# Rebuild everything
rebuild: clean all

# Run kernel
run: all
	@echo "	RUN	$(IGROS_ARCH) ($(IGROS_ARCH))"

# Test kernel
test: all
	@echo "	TEST	$(IGROS_ARCH) ($(IGROS_ARCH))"

# Deploy kernel iso
deploy: all
	@echo "	DEPLOY	$(BIN_NAME) ($(IGROS_ARCH))"
	@echo "	MKDIR	$(IGROS_DIR_ISO)"
	@mkdir -p $(IGROS_DIR_RELEASE)/$(IGROS_DIR_ISO)
	@echo "	MKDIR	$(IGROS_DIR_ISO_BOOT)"
	@mkdir -p $(IGROS_DIR_RELEASE)/$(IGROS_DIR_ISO_BOOT)
	@echo "	CP	$(BIN_NAME) $(IGROS_DIR_ISO_BOOT)"
	@cp $(IGROS_DIR_RELEASE)/$(BIN_NAME) $(IGROS_DIR_RELEASE)/$(IGROS_DIR_ISO_BOOT)
	@echo "	MKDIR	$(IGROS_DIR_ISO_GRUB)"
	@mkdir -p $(IGROS_DIR_RELEASE)/$(IGROS_DIR_ISO_GRUB)
	@echo "	GEN	$(IGROS_GRUB_CONFIG)"
	@echo 'set timeout=0'			 > $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo 'set default=0'			>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo ''				>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo 'menuentry "IgrOS-Kernel" {'	>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo '	multiboot /boot/kernel.bin'	>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo '	boot'				>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo '}'				>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo "	GEN	$(IGROS_ISO)"
	@grub-mkrescue -o $(IGROS_DIR_RELEASE)/../$(IGROS_ISO) $(IGROS_DIR_RELEASE)/$(IGROS_DIR_ISO) 2>/dev/null
	@echo "	RM	$(IGROS_DIR_ISO)"
	@rm -rf $(IGROS_DIR_RELEASE)/$(IGROS_DIR_ISO)

# Create build directory
$(IGROS_DIR_BUILD):
	@echo "	MKDIR	$(IGROS_DIR_BUILD)"
	@mkdir -p $(IGROS_DIR_BUILD)

# Create release directory
$(IGROS_DIR_RELEASE):
	@echo "	MKDIR	$(IGROS_DIR_RELEASE)"
	@mkdir -p $(IGROS_DIR_RELEASE)

# Link kernel binary
$(BIN): $(IGROS_DIR_BUILD) $(IGROS_DIR_RELEASE) $(CXXOBJ)
	@echo "	MAKE	multiboot"
	@$(MAKE) -C multiboot
	@echo "	MAKE	$(IGROS_ARCH)"
	@$(MAKE) -C arch/$(IGROS_ARCH)
	@echo "	MAKE	klib"
	@$(MAKE) -C klib
	@echo "	MAKE	mem"
	@$(MAKE) -C mem
	@echo "	MAKE	drivers"
	@$(MAKE) -C drivers
	@echo "	LD	$(BIN_NAME)"
	@$(LD) $(OBJ) -o $(BIN) -T $(LDSCRIPT) $(LDFLAGS)

