################################################################
#
#	arch folder Make script
#
#	File:	Makefile
#	Date:	09 Oct 2019
#
#	Copyright (c) 2017 - 2020, Igor Baklykov
#	All rights reserved.
#
#


export	IGROS_DIR_BUILD		= $(CURDIR)/../build
export	IGROS_DIR_RELEASE	= $(CURDIR)/../release
export	IGROS_ARCH		?= i386

export	CXX			?= g++
export	CXXFLAGS		= -std=c++11				\
				  -Wall					\
				  -Wextra				\
				  -w					\
				  -pedantic				\
				  -Werror				\
				  -ffreestanding			\
				  -fno-exceptions			\
				  -fno-rtti				\
				  -nostdlib				\
				  -fno-pic				\
				  -fno-pie				\
				  -Os					\
				  -s					\
				  -mno-3dnow				\
				  -mno-sse				\
				  -mno-sse2				\
				  -mno-sse3				\
				  -mno-mmx				\
				  -DIGROS_ARCH_$(IGROS_ARCH)		\
				  -DIGROS_ARCH="\"$(IGROS_ARCH)"\"

ifeq ($(IGROS_ARCH), i386)
	CXXFLAGS		+= -m32					\
				   -march=i386
else ifeq ($(IGROS_ARCH), x86_64)
	CXXFLAGS		+= -m64					\
				   -march=x86-64			\
				   -mno-red-zone			\
				   -mcmodel=large
endif

export	CXXINCLUDE		= -I $(CURDIR)/$(IGROS_ARCH)/include/	\
				  -I $(CURDIR)/../include/

export	AS			= as
export	ASFLAGS

ifeq ($(IGROS_ARCH), i386)
	ASFLAGS			= --32
else ifeq ($(IGROS_ARCH), x86_64)
	ASFLAGS			= --64
else
	ASFLAGS			= 
endif

export	LD			= ld
export	LDFLAGS			= -n					\
				  -m elf_$(IGROS_ARCH)			\
				  -z max-page-size=0x1000

export	RM			= rm -rf

export	BIN_NAME		= kernel.bin

IGROS_DIR_ISO			= /iso
IGROS_DIR_BOOT			= $(IGROS_DIR_ISO)/boot
IGROS_DIR_GRUB			= $(IGROS_DIR_BOOT)/grub
IGROS_GRUB_CONFIG		= $(IGROS_DIR_GRUB)/grub.cfg
IGROS_ISO			= os-$(IGROS_ARCH).iso


.PHONY: all clean distclean rebuild run deploy

all: $(IGROS_DIR_BUILD) $(IGROS_DIR_RELEASE)
	@echo "	MAKE	multiboot"
	@$(MAKE) -C $(CURDIR)/../multiboot
	@echo "	MAKE	klib"
	@$(MAKE) -C $(CURDIR)/../klib
	@echo "	MAKE	mem"
	@$(MAKE) -C $(CURDIR)/../mem
	@echo "	MAKE	$(IGROS_ARCH)"
	@$(MAKE) -C $(IGROS_ARCH)

clean:
	@echo "	CLEAN	multiboot"
	@$(MAKE) -C $(CURDIR)/../multiboot clean
	@echo "	CLEAN	klib"
	@$(MAKE) -C $(CURDIR)/../klib clean
	@echo "	CLEAN	mem"
	@$(MAKE) -C $(CURDIR)/../mem clean
	@echo "	CLEAN	$(IGROS_ARCH)"
	@$(MAKE) -C $(IGROS_ARCH) clean

distclean:
	@echo "	DIST	$(IGROS_ARCH)"
	@$(MAKE) -C $(IGROS_ARCH) distclean

rebuild: clean all

run: all
	@echo "	RUN	$(IGROS_ARCH)"
	@$(MAKE) -C $(IGROS_ARCH) run

deploy: all
	@echo "	DEPLOY	$(BIN_NAME)"
	@echo "	MKDIR	$(IGROS_DIR_ISO)"
	@mkdir -p $(IGROS_DIR_RELEASE)/$(IGROS_DIR_ISO)
	@echo "	MKDIR	$(IGROS_DIR_BOOT)"
	@mkdir -p $(IGROS_DIR_RELEASE)/$(IGROS_DIR_BOOT)
	@echo "	CP	$(BIN_NAME) $(IGROS_DIR_BOOT)"
	@cp $(IGROS_DIR_RELEASE)/$(IGROS_ARCH)/$(BIN_NAME) $(IGROS_DIR_RELEASE)/$(IGROS_DIR_BOOT)
	@echo "	MKDIR	$(IGROS_DIR_GRUB)"
	@mkdir -p $(IGROS_DIR_RELEASE)/$(IGROS_DIR_GRUB)
	@echo "	GEN	$(IGROS_GRUB_CONFIG)"
	@echo 'set timeout=0'			 > $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo 'set default=0'			>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo ''				>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo 'menuentry "IgrOS-Kernel" {'	>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo '	multiboot /boot/kernel.bin'	>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo '	boot'				>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo '}'				>> $(IGROS_DIR_RELEASE)/$(IGROS_GRUB_CONFIG)
	@echo "	GEN	$(IGROS_ISO)"
	@grub-mkrescue -o $(IGROS_DIR_RELEASE)/$(IGROS_ISO) $(IGROS_DIR_RELEASE)/$(IGROS_DIR_ISO) 2>/dev/null
	@echo "	RM	$(IGROS_DIR_ISO)"
	@rm -rf $(IGROS_DIR_RELEASE)/$(IGROS_DIR_ISO)

$(IGROS_DIR_BUILD):
	@echo "	MKDIR	$(IGROS_DIR_BUILD)/$(IGROS_ARCH)"
	@mkdir -p $(IGROS_DIR_BUILD)/$(IGROS_ARCH)

$(IGROS_DIR_RELEASE):
	@echo "	MKDIR	$(IGROS_DIR_RELEASE)/$(IGROS_ARCH)"
	@mkdir -p $(IGROS_DIR_RELEASE)/$(IGROS_ARCH)

